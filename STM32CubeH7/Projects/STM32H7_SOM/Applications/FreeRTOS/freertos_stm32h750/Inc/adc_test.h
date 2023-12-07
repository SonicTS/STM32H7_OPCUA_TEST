#ifndef __ADC_TEST_H
#define __ADC_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_adc_se(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __ADC_TEST_H */
