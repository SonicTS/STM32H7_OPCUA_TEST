#ifndef __PWM_TEST_H
#define __PWM_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_pwm_start(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_pwm_stop(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __PWM_TEST_H */
