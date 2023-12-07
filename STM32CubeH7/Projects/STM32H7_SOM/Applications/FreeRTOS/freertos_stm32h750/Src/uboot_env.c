#include "stm32h7xx_hal.h"
#include <string.h>
#include "FreeRTOS.h"
#include "console.h"
#include "uboot_abi.h"
#include "uboot_env.h"

char * uboot_getenv(const char * varname)
{
	return uboot_abi_getenv(varname);
}

int uboot_setenv(const char *varname, const char *value)
{
	return uboot_abi_setenv(varname, value);
}

int uboot_saveenv(void)
{
	return uboot_abi_saveenv();
}

BaseType_t cmd_uboot_getenv(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	char * cmd_arg;
	char * varvalue;

	cmd_arg = (char *)pcCommandString + 13;
	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot ABI is not available!");
		return pdFALSE;
	}

	varvalue = uboot_getenv(cmd_arg);
	*wBuf = 0;
	if (varvalue) {
		 strcpy(wBuf, varvalue);
	}
	return pdFALSE;
}

BaseType_t cmd_uboot_setenv(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	char * cmd_arg;
	char * var_name;
	char * value;

	cmd_arg = (char *)pcCommandString + 13;
	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot ABI is not available!");
		return pdFALSE;
	}

	value = strchr(cmd_arg, '=');
	if (value == NULL) {
		strcpy(wBuf, "Illegal argument. Argument is of the form name=value");
	}
	else {
		var_name = (char *)cmd_arg;
		* value = 0;
		value += 1;
		uboot_setenv(var_name, value);
		*wBuf = 0;
	}

	return pdFALSE;
}

BaseType_t cmd_uboot_saveenv(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot ABI is not available!");
		return pdFALSE;
	}

	if (uboot_saveenv() == 0) {
		* wBuf = 0;
	}
	return pdFALSE;
}
