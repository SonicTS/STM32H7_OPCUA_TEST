#include <string.h>
#include "projdefs.h"
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "crc.h"
#include "console.h"
#include "env.h"

static uint32_t real_envsize(env_t * environment)
{
	uint32_t	size = 0;
	uint32_t	l;
	char *		envbuf;

	envbuf = environment->data;
	do {
		l = strlen(envbuf) + 1;
		size += l;
		envbuf += l;
	} while (*envbuf != 0 && size < ENV_SIZE);
	return size;
}

static int saveenv(env_t * environment)
{
	uint32_t	flash_addr;
	uint64_t	data_addr;
	uint32_t	i;
	uint32_t	res = 0;
	char		tbuf[128];
	uint32_t	SECTORError;
	FLASH_EraseInitTypeDef EraseInitStruct;

	flash_addr = CFG_ENV_BASE_ADDR;
	data_addr = (uint64_t)((int32_t)environment);
	environment->crc = crc32(0, environment->data, ENV_SIZE);

#ifdef DEBUG
	{
		char * str;
		int l;
		str = environment->data;
		do {
			console_puts(str); console_puts("\n\r");
			l = strlen(str);
			str += (l+1);
		} while (*str != 0);
	}
#endif

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Banks         = FLASH_BANK_1;
	EraseInitStruct.Sector        = 3;
	EraseInitStruct.NbSectors     = 1;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK) {
		console_puts("Error while erase sector\n\r");
		while (1);
	}

	i = 0;
	do {
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, flash_addr, data_addr) == HAL_OK)  {
			data_addr += 32;
			flash_addr += 32;
			i += 32;
		}
		else {
			sprintf(tbuf, "Error=%lx while writing to flash i=%ld\n\r", HAL_FLASH_GetError(), i);
			console_puts(tbuf);
			while (1);
		}
	} while (i < CONFIG_ENV_SIZE);

	return res;
}

char * fw_getenv(char * name)
{
	env_t *	env_flash = ((void * )(CFG_ENV_BASE_ADDR));
	char *	env_data = env_flash->data;
	size_t	var_size;
	size_t	envlen;
	char *	var_value;

	do {
		envlen = strlen(env_data);
		var_value = strchr(env_data, '=');
		if (var_value) {
			var_size = var_value - env_data;
			var_value += 1;
			if (strncmp(env_data, name, var_size) == 0) {
				return env_data;
			}
			else {
				env_data += envlen + 1;
			}
		}
	} while (*env_data != 0);
	return NULL;
}

int fw_setenv(char * name, char * value)
{
	env_t *	nenv = NULL;
	env_t *	oenv = ((void * )(CFG_ENV_BASE_ADDR));
	char  *	ovar;
	char  *	mvar;
	size_t	envsize;
	size_t	lvalue;

	nenv = pvPortMalloc(CONFIG_ENV_SIZE);
	if (!nenv) return 1;

	bzero(nenv, sizeof(env_t));
	ovar = fw_getenv(name);
	if (ovar) {
		envsize = ovar - oenv->data;
		memcpy(nenv->data, oenv->data, envsize);
		mvar = (nenv->data) + envsize;
		lvalue = strlen(value);
		envsize += strlen(ovar);
		if (lvalue) {
			while (*ovar != '=') *mvar++ = *ovar++;
			*mvar++ = '=';
			strcpy(mvar, value);
			mvar += (lvalue + 1);
		}
		while (*ovar != 0) ovar++;
		ovar += 1;
		memcpy(mvar, ovar, (real_envsize(oenv) - envsize));
	}
	else {
		ovar = oenv->data;
		mvar = nenv->data;
		do {
			while (*ovar != 0) *mvar++ = *ovar++;
			*mvar++ = *ovar++;
		} while (*ovar != 0);
		strcpy(mvar, name);
		mvar += strlen(name);
		*mvar++ =  '=';
		strcpy(mvar, value);
		mvar += strlen(value);
		*mvar++ = 0;
	}
	saveenv(nenv);
	vPortFree(nenv);

	return 0;
}
