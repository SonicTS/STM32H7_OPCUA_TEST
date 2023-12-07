#ifndef __SDRAM_INFO_H
#define __SDRAM_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

uint32_t get_sdram_size(void);
BaseType_t cmd_sdram_info(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_INFO_H */
