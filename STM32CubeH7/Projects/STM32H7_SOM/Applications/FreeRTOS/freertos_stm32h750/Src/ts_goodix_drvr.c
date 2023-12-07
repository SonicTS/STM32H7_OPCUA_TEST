#include "stm32h7xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "console.h"
#include "i2c_drvr.h"
#include "gui.h"
#include "ts_goodix_drvr.h"

#define GOODIX_CONTACT_SIZE      8
#define GOODIX_MAX_CONTACTS      10
#define GOODIX_CONFIG_MAX_LENGTH 240
#define GOODIX_CONFIG_911_LENGTH 186
#define GOODIX_CONFIG_967_LENGTH 228

/* Register defines */
#define GOODIX_REG_COMMAND       0x8040
#define GOODIX_CMD_SCREEN_OFF    0x05

#define GOODIX_READ_COOR_ADDR    0x814E
#define GOODIX_REG_CONFIG_DATA   0x8047
#define GOODIX_REG_ID            0x8140
#define GOODIX_REG_HW_INFO       0x4220

#define RESOLUTION_LOC   1
#define MAX_CONTACTS_LOC 5
#define TRIGGER_LOC      6
#define GTP_INT_RISING   0
#define GTP_INT_FALLING  1
#define GTP_INT_TRIGGER  GTP_INT_FALLING

static ts_goodix_handler * hts = NULL;
static SemaphoreHandle_t   hsinput;

static int ts_goodix_config(ts_goodix_handler * h)
{
	uint8_t config[GOODIX_CONFIG_MAX_LENGTH];
	int error;
	int i, check_sum = 0;

	error = i2c_read(h->hi2c, h->i2c_addr, 1, GOODIX_REG_CONFIG_DATA, config, h->cfg_len);
	if (error) {
#ifdef DEBUG
		console_puts("Error reading config\n\r");
#endif
		return error;
	}

	config[RESOLUTION_LOC] = (uint8_t) h->max_width;
	config[RESOLUTION_LOC + 1] = (uint8_t) (h->max_width >> 8);
	config[RESOLUTION_LOC + 2] = (uint8_t) h->max_height;
	config[RESOLUTION_LOC + 3] = (uint8_t) (h->max_height >> 8);
	config[MAX_CONTACTS_LOC] = GOODIX_MAX_CONTACTS;
	config[0] = 'A';

	config[TRIGGER_LOC] &= 0xfe;
	if (GTP_INT_TRIGGER == GTP_INT_FALLING)
		config[TRIGGER_LOC] |= 0x01;

	for (i = 0; i < h->cfg_len - 3; i++) check_sum += config[i];
	config[h->cfg_len - 2] = (~check_sum) + 1;
	config[h->cfg_len - 1] = config[h->cfg_len - 1] + 1;

	error = i2c_write(h->hi2c, h->i2c_addr, 1, GOODIX_REG_CONFIG_DATA, config, h->cfg_len);
	if (error) {
#ifdef DEBUG
		console_puts("Config update failed\n\r");
#endif
		return error;
	}
	return 0;
}

static int ts_goodix_process_input(ts_goodix_handler * hts, uint8_t * idata)
{
	int touch_num;

	if (i2c_read(hts->hi2c, hts->i2c_addr, 1, GOODIX_READ_COOR_ADDR, idata, GOODIX_CONTACT_SIZE + 1) != HAL_OK) {
#ifdef DEBUG
		console_puts("I2C transfer error\n\r");
#endif
		return -1;
	}
	if (!(idata[0] & 0x80)) {
#ifdef DEBUG
		console_puts("Illegal value in GOODIX_READ_COOR_ADDR\n\r");
#endif
		return -1;
	}
	touch_num = idata[0] & 0x0F;
	if (touch_num > MAX_CONTACTS_LOC) return -1;
	if (touch_num > 1) {
		idata += 1 + GOODIX_CONTACT_SIZE;
		if (i2c_read(hts->hi2c, hts->i2c_addr, 1, GOODIX_READ_COOR_ADDR + 1 + GOODIX_CONTACT_SIZE,
				idata, GOODIX_CONTACT_SIZE * (touch_num - 1)) != HAL_OK) {
#ifdef DEBUG
			console_puts("I2C transfer error (touch_num > 1)\n\r");
#endif
			return -1;
		}
	}
	return touch_num;
}

static void ts_goodix_report_touch(uint8_t * coor_data, uint16_t * input_x, uint16_t * input_y)
{
	* input_x = (coor_data[2] << 8) | coor_data[1];
	* input_y = (coor_data[4] << 8) | coor_data[3];
}

static IRQn_Type ts_goodix_getirq_by_line(uint32_t line)
{
	switch (line) {
	case GPIO_PIN_0:
		return EXTI0_IRQn;
	case GPIO_PIN_1:
		return EXTI1_IRQn;
	case GPIO_PIN_2:
		return EXTI2_IRQn;
	case GPIO_PIN_3:
		return EXTI3_IRQn;
	case GPIO_PIN_4:
		return EXTI4_IRQn;
	case GPIO_PIN_5:
	case GPIO_PIN_6:
	case GPIO_PIN_7:
	case GPIO_PIN_8:
	case GPIO_PIN_9:
		return EXTI9_5_IRQn;
	case GPIO_PIN_10:
	case GPIO_PIN_11:
	case GPIO_PIN_12:
	case GPIO_PIN_13:
	case GPIO_PIN_14:
	case GPIO_PIN_15:
		return EXTI15_10_IRQn;
	default:
		for (;;);
	}
	return 0;
}

static void ts_goodix_enable_irq(int32_t line)
{
	IRQn_Type irqn = ts_goodix_getirq_by_line(line);

	HAL_NVIC_SetPriority(irqn, 0x0F, 0);
	HAL_NVIC_EnableIRQ(irqn);
}

static void ts_goodix_disable_irq(int32_t line)
{
	IRQn_Type irqn = ts_goodix_getirq_by_line(line);

	HAL_NVIC_DisableIRQ(irqn);
}

int ts_goodix_read_input(ts_goodix_handler * hts, uint16_t * x, uint16_t * y)
{
	uint8_t  idata[1 + GOODIX_CONTACT_SIZE * GOODIX_MAX_CONTACTS];
	int touch_num;
	int i;
	uint8_t end_cmd = 0;

	ts_goodix_enable_irq(hts->pin_int);
	xSemaphoreTake(hsinput, portMAX_DELAY);
	ts_goodix_disable_irq(hts->pin_int);
	touch_num = ts_goodix_process_input(hts, idata);
	for (i = 0; i < touch_num; i++) {
		ts_goodix_report_touch(&idata[1 + GOODIX_CONTACT_SIZE * i], x + i, y + i);
	}
	i2c_write(hts->hi2c, hts->i2c_addr, 1, GOODIX_READ_COOR_ADDR, &end_cmd, 1);
	return touch_num;
}

ts_goodix_handler * ts_goodix_init(uint8_t bus, uint8_t i2c_addr, uint16_t max_width, uint16_t max_height,
		GPIO_TypeDef * port, uint32_t pin)
{
	GPIO_InitTypeDef gpio_int;
	uint32_t hwinfo = 0;
	char regid_buf[5];
	int res;

	if (hts) {
		return hts;
	}

	hts = pvPortMalloc(sizeof(ts_goodix_handler));
	if (hts == NULL) {
		console_puts("Could not allocate memory\n\r");
		return NULL;
	}

	if ((hts->hi2c = i2c_init(bus, 100)) == 0) {
		console_puts("I2C init error\n\r");
		goto Fail;
	}

	res = i2c_read(hts->hi2c, i2c_addr, 1, GOODIX_REG_HW_INFO, (uint8_t *)&hwinfo, 4);
	if (res != 0) {
		console_puts("i2c_read error\n\r");
		goto Fail;
	}
	if (hwinfo != 0x00900600) {
		console_puts("Illegal hwinfo\n\r");
		goto Fail;
	}
	hts->i2c_addr = i2c_addr;
	res = i2c_read(hts->hi2c, i2c_addr, 1, GOODIX_REG_ID, (uint8_t *)regid_buf, 4);
	regid_buf[4] = 0;
	sscanf(regid_buf, "%hu", &hts->devid);

	switch (hts->devid) {
	case 911:
	case 9271:
	case 9110:
	case 927:
	case 928:
	case 970:
		hts->cfg_len = GOODIX_CONFIG_911_LENGTH;
		break;
	case 912:
	case 967:
		hts->cfg_len = GOODIX_CONFIG_967_LENGTH;
		break;
	default:
		hts->cfg_len = GOODIX_CONFIG_MAX_LENGTH;
	}
	hts->max_width = max_width;
	hts->max_height = max_height;
	hts->pin_int = pin;

	gpio_int.Pin = pin;
	gpio_int.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_int.Pull = GPIO_PULLUP;
	gpio_int.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(port, &gpio_int);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	vTaskDelay(50 / portTICK_PERIOD_MS);	/* T5: 50ms */
	HAL_GPIO_ReadPin(port, pin);

	gpio_int.Pin = pin;
	gpio_int.Mode = GPIO_MODE_OUTPUT_OD;
	gpio_int.Pull = GPIO_PULLUP;
	gpio_int.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(port, &gpio_int);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);

	gpio_int.Pin = pin;
	gpio_int.Mode = GPIO_MODE_IT_FALLING;
	gpio_int.Pull = GPIO_PULLUP;
	gpio_int.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(port, &gpio_int);

	vTaskDelay(51 / portTICK_PERIOD_MS);
	ts_goodix_config(hts);

	hsinput = xSemaphoreCreateBinary();
	return hts;
Fail:
	vPortFree(hts);
	hts = NULL;
	return NULL;
}

/**
  * EXTI line detection callbacks
  * gpio_pin: Specifies the pins connected EXTI line
  */
void ts_goodix_gpio_exti_callback(uint32_t gpio_pin)
{
	xSemaphoreGiveFromISR(hsinput, NULL);
}
#endif
