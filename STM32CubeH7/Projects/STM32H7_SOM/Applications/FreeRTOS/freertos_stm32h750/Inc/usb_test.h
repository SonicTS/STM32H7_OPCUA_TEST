#ifndef __USB_TEST_H
#define __USB_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"

BaseType_t cmd_usbfatls_test(char *wBuf, size_t wBufferLen, const char *pcCommandString);
void usb_task(void * argument);

#ifdef __cplusplus
}
#endif

#endif /* __USB_TEST_H */
