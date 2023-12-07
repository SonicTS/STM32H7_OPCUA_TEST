#ifndef __CMD_LCDTEST_H
#define __CMD_LCDTEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "projdefs.h"

BaseType_t cmd_lcdtest(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __CMD_LCDTEST_H */
