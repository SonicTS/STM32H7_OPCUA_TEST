#ifndef __RELIABLE_UPDATE_H
#define __RELIABLE_UPDATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_reliable_upgrade(char *wBuf, size_t wBufferLen, const char *pcCommandString);

void reliable_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __RELIABLE_UPDATE_H */
