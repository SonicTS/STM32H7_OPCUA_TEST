#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "console.h"
#include "usb_test.h"
#include "usb_host.h"
#include "stdio.h"
#include "ff.h"
#include "ffconf.h"

void MX_USB_HOST_Process(void);
extern USBH_HandleTypeDef hUsbHostFS;

void usb_task(void * argument)
{
	MX_USB_HOST_Init();

	for (;;) {
		MX_USB_HOST_Process();
	}
}

/* Private typedef ---------------------------------------------------------- */
/* Private define ----------------------------------------------------------- */
/* Private macro ------------------------------------------------------------ */
/* Private variables -------------------------------------------------------- */
static int32_t recurseLevel = -1;
/* Private function prototypes ---------------------------------------------- */
/* Private functions -------------------------------------------------------- */

/**
  * @brief  Displays disk content.
  * @param  path: Pointer to root path
  * @param  recu_level: Disk content level
  * @retval Operation result
  */
FRESULT Explore_Disk(char *path, uint8_t recu_level)
{
	FRESULT res = FR_OK;
	FILINFO fno;
	DIR dir;
	char tmp[14];

	recurseLevel++;
	res = f_opendir(&dir, path);
	if (res == FR_OK)
	{
		while (USBH_MSC_IsReady(&hUSB_Host))
		{
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0)
			{
				break;
			}
			if (fno.fname[0] == '.')
			{
				continue;
			}

			strcpy(tmp, fno.fname);

			if (recu_level == 1)
			{
				console_puts("   |__");
			}
			else if (recu_level == 2)
			{
				console_puts("   |   |__");
			}
			if (fno.fattrib & AM_DIR)
			{
				strcat(tmp, "\n\r");
				console_puts((void *)tmp);
				Explore_Disk(fno.fname, 2);
			}
			else
			{
				strcat(tmp, "\n\r");
				console_puts((void *)tmp);
			}

			if ((fno.fattrib & AM_DIR) && (recu_level == 2))
			{
				Explore_Disk(fno.fname, 2);
			}
		}
		f_closedir(&dir);
	}

	return res;
}

BaseType_t cmd_usbfatls_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	Explore_Disk("0:/", 1);
	*wBuf = 0;
	return pdFALSE;
}
