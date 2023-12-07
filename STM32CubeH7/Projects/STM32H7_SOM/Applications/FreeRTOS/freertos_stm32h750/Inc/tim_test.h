#ifndef __TIM_TEST_H
#define __TIM_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_timer_cnt(char *wBuf, size_t wBufferLen, const char *pcCommandString);
void tim_task(void * argument);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_TEST_H */
