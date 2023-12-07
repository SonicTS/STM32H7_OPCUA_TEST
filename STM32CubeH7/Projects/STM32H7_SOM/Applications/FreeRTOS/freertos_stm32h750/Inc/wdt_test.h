#ifndef __WDT_TEST_H
#define __WDT_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_wdt_stop(char *wBuf, size_t wBufferLen, const char *pcCommandString);
void wdt_task(void * argument);

#ifdef __cplusplus
}
#endif

#endif /* __WDT_TEST_H */
