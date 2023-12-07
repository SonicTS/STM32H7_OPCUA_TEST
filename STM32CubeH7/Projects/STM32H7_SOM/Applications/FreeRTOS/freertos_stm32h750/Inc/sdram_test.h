#ifndef __SDRAM_TEST_H
#define __SDRAM_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_sdram_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_TEST_H */