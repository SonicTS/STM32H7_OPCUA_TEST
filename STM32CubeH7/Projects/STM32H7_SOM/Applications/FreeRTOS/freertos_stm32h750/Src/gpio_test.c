#include "stm32h7xx_hal.h"
#ifdef HAL_GPIO_MODULE_ENABLED
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "string.h"
#include "console.h"
#include "gpio_test.h"

#define LED_GPIO_PORT  GPIOI
#define LED_GPIO_PIN   GPIO_PIN_8
#define LED1_GPIO_PORT GPIOB
#define LED1_GPIO_PIN  GPIO_PIN_0
#define LED2_GPIO_PORT GPIOB
#define LED2_GPIO_PIN  GPIO_PIN_1
#define BTN1_GPIO_PORT GPIOH
#define BTN1_GPIO_PIN  GPIO_PIN_2
#define BTN1_GPIO_IRQ  EXTI2_IRQn
#define BTN2_GPIO_PORT GPIOH
#define BTN2_GPIO_PIN  GPIO_PIN_3
#define BTN2_GPIO_IRQ  EXTI3_IRQn

#define led_gpio_clk_enable()  __HAL_RCC_GPIOI_CLK_ENABLE()
#define led1_gpio_clk_enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define led2_gpio_clk_enable() __HAL_RCC_GPIOB_CLK_ENABLE()
#define btn1_gpio_clk_enable() __HAL_RCC_GPIOH_CLK_ENABLE()
#define btn2_gpio_clk_enable() __HAL_RCC_GPIOH_CLK_ENABLE()

static SemaphoreHandle_t hsbtnsx;
static uint32_t flg_btn1 = 0;
static uint32_t flg_btn2 = 0;

static void gpio_init(GPIO_TypeDef * port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed)
{
	GPIO_InitTypeDef gpio_init_structure;

	gpio_init_structure.Pin = pin;
	gpio_init_structure.Mode = mode;
	gpio_init_structure.Pull = pull;
	gpio_init_structure.Speed = speed;
	HAL_GPIO_Init(port, &gpio_init_structure);
}

void gpio_test_led(void * argument)
{
	/* Configure LED */
	led_gpio_clk_enable();
	gpio_init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
	for (;;) {
		/* Toggle LED every 500 ms */
		HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
		vTaskDelay(2000/portTICK_PERIOD_MS);
	}
}

void gpio_test_buttons(void * argument)
{
	hsbtnsx = xSemaphoreCreateBinary();

	/* Configure LED1 */
	led1_gpio_clk_enable();
	gpio_init(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET);

	/* Configure LED2 */
	led2_gpio_clk_enable();
	gpio_init(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_GPIO_PIN, GPIO_PIN_RESET);

	/* Configure button 1 */
	btn1_gpio_clk_enable();
	gpio_init(BTN1_GPIO_PORT, BTN1_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_NVIC_ClearPendingIRQ(BTN1_GPIO_IRQ);
	HAL_NVIC_SetPriority(BTN1_GPIO_IRQ, 0x0F, 0);

	/* Configure button 2 */
	btn2_gpio_clk_enable();
	gpio_init(BTN2_GPIO_PORT, BTN2_GPIO_PIN, GPIO_MODE_IT_RISING_FALLING, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH);
	HAL_NVIC_ClearPendingIRQ(BTN2_GPIO_IRQ);
	HAL_NVIC_SetPriority(BTN2_GPIO_IRQ, 0x0F, 0);
	HAL_NVIC_EnableIRQ(BTN1_GPIO_IRQ);
	HAL_NVIC_EnableIRQ(BTN2_GPIO_IRQ);

	for (;;) {
		xSemaphoreTake(hsbtnsx, portMAX_DELAY);
		if (flg_btn1 == 1) {
			taskENTER_CRITICAL();
			flg_btn1 = 0;
			taskEXIT_CRITICAL();
			HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN);
		}
		if (flg_btn2 == 1) {
			taskENTER_CRITICAL();
			flg_btn2 = 0;
			taskEXIT_CRITICAL();
			HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_GPIO_PIN);
		}
	}
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t gpio_pin)
{
	switch (gpio_pin) {
	case BTN1_GPIO_PIN:
		flg_btn1 = 1;
		xSemaphoreGiveFromISR(hsbtnsx, NULL);
		break;
	case BTN2_GPIO_PIN:
		flg_btn2 = 1;
		xSemaphoreGiveFromISR(hsbtnsx, NULL);
		break;
	default:
		console_puts("HAL_GPIO_EXTI_Callback unknown pin\r\n");
		break;
	}
}

/******************************************************************************/
/*                 STM32H7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32h7xx.s).                                               */
/******************************************************************************/

void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BTN1_GPIO_PIN);
}

void EXTI3_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BTN2_GPIO_PIN);
}
#endif
