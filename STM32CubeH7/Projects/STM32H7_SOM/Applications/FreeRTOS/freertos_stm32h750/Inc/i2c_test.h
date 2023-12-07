#ifndef __I2C_TEST_H
#define __I2C_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "projdefs.h"

BaseType_t cmd_i2c_scan(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_i2c_read(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_i2c_write(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_TEST_H */