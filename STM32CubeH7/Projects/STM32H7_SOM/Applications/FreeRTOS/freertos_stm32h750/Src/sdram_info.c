#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "console.h"
#include "uboot_env.h"
#include "sdram_info.h"

uint32_t get_sdram_size(void)
{
	char * mem;
	uint32_t size;

	mem = uboot_getenv("mem");
	sscanf(mem, "%ld", &size);
	size *= 1024;
	return size;
}

BaseType_t cmd_sdram_info(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	uint32_t size;

	size = get_sdram_size();
	sprintf(wBuf, "Amount of SDRAM is %ldMB", size / (1024*1024));
	return pdFALSE;
}
