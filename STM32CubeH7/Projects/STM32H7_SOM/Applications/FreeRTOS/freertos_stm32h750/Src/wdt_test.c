#include "stm32h7xx_hal.h"
#ifdef HAL_IWDG_MODULE_ENABLED
#include "FreeRTOS.h"
#include "task.h"
#include "console.h"
#include "wdt_test.h"

#define WDT_PERIOD (2 * 1000)

static uint32_t wdt_stop_flag = 0;

void wdt_task(void * argument)
{
	IWDG_HandleTypeDef iwdg_handle;

	/* Configure & Start the IWDG peripheral
	 * Set counter reload value to obtain 762ms IWDG TimeOut.
	 * Counter Reload Value = (LsiFreq(Hz) * Timeout(ms)) / (prescaler * 1000)
	 */
	iwdg_handle.Instance = IWDG1;
	iwdg_handle.Init.Prescaler = IWDG_PRESCALER_16;
	iwdg_handle.Init.Reload = (32000 * WDT_PERIOD) / (16 * 1000); /* 2 s. */
	iwdg_handle.Init.Window = IWDG_WINDOW_DISABLE;
	HAL_IWDG_Init(&iwdg_handle);
	for (;;) {
		vTaskDelay((WDT_PERIOD - 100)/portTICK_PERIOD_MS);
		if (wdt_stop_flag == 0) {
			HAL_IWDG_Refresh(&iwdg_handle);
		}
	}
}

BaseType_t cmd_wdt_stop(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	wdt_stop_flag = 1;
	*wBuf = 0;
	return pdFALSE;
}
#endif
