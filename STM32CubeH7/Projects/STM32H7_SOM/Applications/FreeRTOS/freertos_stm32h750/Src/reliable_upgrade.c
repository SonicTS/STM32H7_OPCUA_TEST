#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "console.h"
#include "usb_test.h"
#include "usb_host.h"
#include "stdio.h"
#include "ff.h"
#include "ffconf.h"
#include "uboot_abi.h"

BaseType_t cmd_reliable_upgrade(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	FRESULT res = FR_OK;
	FIL fp;
	const char *path = pcCommandString + sizeof(__func__) - 4;
	char *imbuf;
	unsigned int rsize = 4 * 1024, rd, tot_read = 0;

	if (uboot_abi_check()) {
		strcpy(wBuf, "U-Boot ABI is not available!");
		return pdFALSE;
	}

	res = f_open (&fp, path, FA_READ);
	if (res == 0) {
		char *uvar;
		int set;

		uvar = uboot_abi_getenv("upgrade_available");
		if (uvar == NULL || strcmp(uvar, "1") == 0) {
			console_printf("ERROR: Upgrade has been done already, please reboot\n");
			return pdFALSE;
		}

		uvar = uboot_abi_getenv("boot2_active");
		if (uvar != NULL && strcmp(uvar, "1") == 0) {
			set = 1;
			uboot_abi_setenv("boot1_valid", "0");
		} else {
			set = 2;
			uboot_abi_setenv("boot2_valid", "0");
		}

		uboot_abi_saveenv();

		imbuf = pvPortMalloc(128 * 1024);
		while (f_read(&fp, imbuf + tot_read, rsize, &rd) == FR_OK) {
			tot_read += rd;
			console_printf("\"%s\" 1 read %d bytes\n", path, tot_read);
			if (rd < rsize) {
				break;
			}
		}
		console_printf("\"%s\" read %d bytes\n", path, tot_read);
		console_printf("ubi write to %s\n", set == 1 ? "rtos1" : "rtos2");

		ubi_volume_write(set == 1 ? "rtos1" : "rtos2", imbuf, tot_read);

		f_close(&fp);
		vPortFree(imbuf);

		uboot_abi_setenv("upgrade_available", "1");
		uboot_abi_setenv("bootcount", "0");
		uboot_abi_setenv("bootlimit", "2");
		if ( set == 1 ) {
			uboot_abi_setenv("boot2_active", "0");
			uboot_abi_setenv("boot1_valid", "1");
		} else {
			uboot_abi_setenv("boot2_active", "1");
			uboot_abi_setenv("boot2_valid", "1");
		}
		uboot_abi_saveenv();

		HAL_NVIC_SystemReset();
	} else {
		console_printf("filed to open file %s (res %d)\n", path, res);
		console_puts("   - \n\r");
	}

	console_printf("\"%s\" command %s\n", path, pcCommandString);

	return pdFALSE;
}

int sanity_check(void)
{
	/*
	   Perform any sanity checks specific to your application.
	   The sanity check is performed on the first boot after upgrade.
	   The failed sanity check causes the system to restart
	   and the bootcount to be incremented.
	   If bootcount exceeds bootlimit then U-Boot rolls back to the previos boot set.
	 */
	return 0;
}

void reliable_init(void)
{
	char *uvar;
	uvar = uboot_abi_getenv("upgrade_available");
	if (uvar == NULL || strcmp(uvar, "1") == 0) {
		if (sanity_check() != 0) {
			HAL_NVIC_SystemReset();
		}
		uboot_abi_setenv("upgrade_available", "0");
		uboot_abi_setenv("bootcount", "0");
		uboot_abi_saveenv();
	}
}
