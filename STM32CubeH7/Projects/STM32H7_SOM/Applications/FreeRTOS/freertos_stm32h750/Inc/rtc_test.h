#ifndef __RTC_TEST_H
#define __RTC_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_rtc_get_date(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_rtc_set_date(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __RTC_TEST_H */
