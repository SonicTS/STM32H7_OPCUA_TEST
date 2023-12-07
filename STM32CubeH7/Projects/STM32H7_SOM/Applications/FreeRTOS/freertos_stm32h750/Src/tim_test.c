#include "stm32h7xx_hal.h"
#ifdef HAL_TIM_MODULE_ENABLED
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "console.h"
#include "tim_test.h"

#define MHz (1000000U)
#define NINTR (1000U)

static uint32_t cnt_intr = 0;
static SemaphoreHandle_t hstim;
static TIM_HandleTypeDef htim;

void tim_task(void * argument)
{
	RCC_ClkInitTypeDef clkconfig;
	uint32_t pFLatency;
	uint32_t apb1_prescaler;
	uint32_t tim_clk;

	__HAL_RCC_TIM7_CLK_ENABLE();
	/* Get clock configuration */
	HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

	/* Get APB1 prescaler */
	apb1_prescaler = clkconfig.APB1CLKDivider;

	/* Compute TIM7 clock */
	if (apb1_prescaler == RCC_HCLK_DIV1) {
		tim_clk = HAL_RCC_GetPCLK1Freq();
	}
	else {
		tim_clk = 2*HAL_RCC_GetPCLK1Freq();
	}

	/* Initialize TIM7 */
	htim.Instance = TIM7;
	htim.Init.Period = (MHz / NINTR) - 1U;
	htim.Init.Prescaler = (uint32_t)((tim_clk / MHz) - 1U);
	htim.Init.ClockDivision = 0;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	if (HAL_TIM_Base_Init(&htim) != HAL_OK) {
		console_puts("tim_task: HAL_TIM_Base_Init error\n\r");
		return;
	}
	hstim = xSemaphoreCreateBinary();

	/*Configure the TIM7 IRQ priority */
	HAL_NVIC_SetPriority(TIM7_IRQn, 0x0F ,0U);

	/* Enable the TIM7 global Interrupt */
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Start_IT(&htim);

	for (;;) {
		xSemaphoreTake(hstim, portMAX_DELAY);
		cnt_intr++;
	}
}

BaseType_t cmd_timer_cnt(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	sprintf(wBuf, "%ld", cnt_intr);
	return pdFALSE;
}

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim);
	xSemaphoreGiveFromISR(hstim, NULL);
}
#endif
