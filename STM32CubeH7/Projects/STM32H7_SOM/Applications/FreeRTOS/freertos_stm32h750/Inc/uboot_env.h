#ifndef __UBOOT_ENV_H
#define __UBOOT_ENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

char * uboot_getenv(const char * varname);
int uboot_setenv(const char *varname, const char *value);
int uboot_saveenv(void);
BaseType_t cmd_uboot_getenv(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_uboot_setenv(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_uboot_saveenv(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __UBOOT_ENV_H */
