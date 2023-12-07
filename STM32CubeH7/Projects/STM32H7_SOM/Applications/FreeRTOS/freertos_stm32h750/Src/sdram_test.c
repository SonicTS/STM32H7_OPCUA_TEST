#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "console.h"
#include "dma_alloc.h"
#include "sdram_test.h"

BaseType_t cmd_sdram_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *  args = pcCommandString + 11;
	uint32_t i;
	uint32_t count;
	uint32_t pattern;
	uint32_t * p = (uint32_t *)(0xD0000000 + DMA_REGION_SIZE);
#ifdef TIME_PERFORM_TEST
	TickType_t xLastWakeTime1;
	TickType_t xLastWakeTime2;
#endif

	sscanf(args, "%lx %lx", &pattern, &count);
	count /= 4;

#ifdef TIME_PERFORM_TEST
	xLastWakeTime1 = xTaskGetTickCount();
#endif
	for (i = 0; i < count; i++) {
		p[i] = pattern;
	}
	for (i = 0; i < count; i++) {
		if (p[i] != pattern) {
			sprintf(wBuf, "sdram_test_error at addr = 0x%lx read value = 0x%lx", (uint32_t)&p[i], p[i]);
			return pdFALSE;
		}
	}
#ifdef TIME_PERFORM_TEST
	xLastWakeTime2 = xTaskGetTickCount();
	sprintf(wBuf, "Time perform test %ld", xLastWakeTime2 - xLastWakeTime1);
#else
	*wBuf = 0;
#endif
	return pdFALSE;
}
