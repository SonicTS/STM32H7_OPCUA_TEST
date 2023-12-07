#ifndef __CMD_ENV_H
#define __CMD_ENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "projdefs.h"

BaseType_t cmd_printenv(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_setenv(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __CMD_ENV_H */