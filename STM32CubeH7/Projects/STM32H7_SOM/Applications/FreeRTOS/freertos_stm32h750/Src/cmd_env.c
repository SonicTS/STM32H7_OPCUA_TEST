#include <string.h>
#include "projdefs.h"
#include "crc.h"
#include "env.h"
#include "cmd_env.h"

static env_t *	env_flash = ((void * )(CFG_ENV_BASE_ADDR));
static char *	env_data = NULL;

BaseType_t cmd_printenv(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	uint32_t	envlen;
	char *		cmd_arg;
	char *		env_var;

	cmd_arg = (char *)pcCommandString + 9;
	if (env_data == NULL) {
		env_data = env_flash->data;
		if (crc32(0, env_flash->data, ENV_SIZE) != env_flash->crc) {
			strcpy(wBuf, "Bad crc of the environment.");
			env_data = NULL;
			return pdFALSE;
		}
	}
	envlen = strlen(env_data);

	if (strcmp(cmd_arg,"all") == 0) {
		if (envlen < wBufferLen) {
			strcpy(wBuf, env_data);
		}
		else {
			strcpy(wBuf, "Size of the environment variable is longer than supported.");
		}
		env_data += envlen + 1;
		if (*env_data) {
			return pdTRUE;
		}
		else {
			env_data = NULL;
			return pdFALSE;
		}
	}
	else {
		env_var = fw_getenv(cmd_arg);
		if (env_var) {
			strcpy(wBuf, env_var);
		}
		else {
			strcpy(wBuf, "Variable is not defined");
		}
		return pdFALSE;
	}
}

BaseType_t cmd_setenv(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	char * cmd_arg;
	char * var_name;
	char * value;

	if (crc32(0, env_flash->data, ENV_SIZE) != env_flash->crc) {
		strcpy(wBuf, "Bad crc of the environment.");
		return pdFALSE;
	}

	cmd_arg = (char *)pcCommandString + 7;
	value = strchr(cmd_arg, '=');
	if (value == NULL) {
		strcpy(wBuf, "Illegal argument. Argument is of the form name=value");
	}
	else {
		var_name = (char *)cmd_arg;
		* value = 0;
		value += 1;
		fw_setenv(var_name, value);
		*wBuf = 0;
	}

	return pdFALSE;
}
