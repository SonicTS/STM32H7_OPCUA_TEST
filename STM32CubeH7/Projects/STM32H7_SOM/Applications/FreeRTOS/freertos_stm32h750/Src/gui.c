#include "stm32h7xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "console.h"
#include "i2c_drvr.h"
#include "ts_goodix_drvr.h"
#include "gui.h"
#include "dma_alloc.h"
#include "lcd.h"

#define TS_I2C_BUS       1
#define TS_I2C_ADDR      0x14
#define TS_MAX_WIDTH     480
#define TS_MAX_HEIGHT    272
#define TS_GPIO_PORT_RST GPIOI
#define TS_GPIO_PIN_RST  GPIO_PIN_11
#define TS_GPIO_PORT_INT GPIOG
#define TS_GPIO_PIN_INT  GPIO_PIN_14

static void ts_reset(uint8_t i2c_addr)
{
	GPIO_InitTypeDef gpio_reset;
	GPIO_InitTypeDef gpio_int;

	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();

	gpio_reset.Pin = TS_GPIO_PIN_RST;
	gpio_reset.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_reset.Pull = GPIO_PULLUP;
	gpio_reset.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TS_GPIO_PORT_RST, &gpio_reset);

	gpio_int.Pin = TS_GPIO_PIN_INT;
	gpio_int.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_int.Pull = GPIO_PULLUP;
	gpio_int.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TS_GPIO_PORT_INT, &gpio_int);

	HAL_GPIO_WritePin(TS_GPIO_PORT_RST, TS_GPIO_PIN_RST, GPIO_PIN_RESET);
	vTaskDelay(20 / portTICK_PERIOD_MS);

	/* HIGH: 0x14, LOW: 0x5d */
	if (i2c_addr == 0x14) {
		HAL_GPIO_WritePin(TS_GPIO_PORT_INT, TS_GPIO_PIN_INT, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(TS_GPIO_PORT_INT, TS_GPIO_PIN_INT, GPIO_PIN_RESET);
	}
	vTaskDelay(1 / portTICK_PERIOD_MS);

	HAL_GPIO_WritePin(TS_GPIO_PORT_RST, TS_GPIO_PIN_RST, GPIO_PIN_SET);
	vTaskDelay(6 / portTICK_PERIOD_MS);
}

void gui_ts_main(void * argument)
{
	ts_goodix_handler * hts;
	int touch_num;
	int i;
	uint8_t  ntest = 0;
	uint16_t input_x[5];
	uint16_t input_y[5];
	uint16_t current_x[5];
	uint16_t current_y[5];
	char output[5 * 80];
	char * buf;

#if (configGUI_INITIALIZED == 1)
	/*##-1- LCD Initialization #################################################*/
	/* Initialize the LCD */
	BSP_LCD_Init();

	/* Foreground Layer Initialization */
	BSP_LCD_LayerDefaultInit(1, DMA_FB_START + (LCD_MAX_WIDTH * LCD_MAX_HIGHT * 4));
	/* Set Foreground Layer */
	BSP_LCD_SelectLayer(1);
	/* Clear the LCD Foreground layer */
	BSP_LCD_Clear(0xffffffff);
	/* Enable the color Key for foreground layer */
	BSP_LCD_SetColorKeying(1, 0xff);
	BSP_LCD_SetLayerVisible(1, DISABLE);

	/* Background Layer Initialization */
	BSP_LCD_LayerDefaultInit(0, DMA_FB_START);
	/* Set Foreground Layer */
	BSP_LCD_SelectLayer(0);

	/* Enable the LCD */
	BSP_LCD_DisplayOn();

	/* Clear the LCD Background layer */
	BSP_LCD_Clear(0xffffffff);
	BSP_LCD_SetBrightness(100);
#else
	BSP_LCD_Driver_Init(DMA_FB_START);
#endif

	test_picture();

	bzero(current_x, sizeof(current_x));
	bzero(current_y, sizeof(current_y));
	ts_reset(TS_I2C_ADDR);
	hts = ts_goodix_init(TS_I2C_BUS, TS_I2C_ADDR, TS_MAX_WIDTH, TS_MAX_HEIGHT, TS_GPIO_PORT_INT, TS_GPIO_PIN_INT);
	if (hts == 0) {
		console_puts("Initialization of TS failed.\n\r");
		vTaskDelete(NULL);
		for (;;);
	}

	for (;;) {
		do {
			touch_num = ts_goodix_read_input(hts, input_x, input_y);
		} while (touch_num <= 0);
		buf = output;
		for (i = 0; i < touch_num; i++) {
			if ((input_x[i] != current_x[i]) || (input_y[i] != current_y[i])) {
				current_x[i] = input_x[i];
				current_y[i] = input_y[i];
				buf += sprintf(buf, "F = %d X = %d Y = %d\n\r", i+1, input_x[i], input_y[i]);
			}
		}
		if (buf != output) {
			console_puts(output);
			ntest ^= 1;
			if (ntest) {
				test_picture2();
			} else {
				test_picture();
			}
		}
	}
}

/**
  * This function handles External lines 15 to 10 interrupt request.
  */
void EXTI15_10_IRQHandler(void)
{
	/* EXTI line interrupt detected */
	if(__HAL_GPIO_EXTI_GET_IT(TS_GPIO_PIN_INT) != RESET) {
		__HAL_GPIO_EXTI_CLEAR_IT(TS_GPIO_PIN_INT);
		ts_goodix_gpio_exti_callback(TS_GPIO_PIN_INT);
	}
}
