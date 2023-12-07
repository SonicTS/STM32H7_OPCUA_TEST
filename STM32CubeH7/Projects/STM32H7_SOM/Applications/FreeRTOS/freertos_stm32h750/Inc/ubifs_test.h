#ifndef __UBIFS_TEST_H
#define __UBIFS_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_ubipart_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsmount_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsumount_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsls_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsload_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifswrite_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsmkdir_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsrmdir_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsunlink_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
BaseType_t cmd_ubifsstress_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);

#ifdef __cplusplus
}
#endif

#endif /* __SDRAM_TEST_H */
