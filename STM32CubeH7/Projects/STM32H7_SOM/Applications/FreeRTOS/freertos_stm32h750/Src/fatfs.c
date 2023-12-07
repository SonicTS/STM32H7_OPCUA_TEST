/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "console.h"
#include "fatfs.h"

uint8_t retUSBH;    /* Return value for USBH */
char USBHPath[4];   /* USBH logical drive path */
FATFS USBHFatFS;    /* File system object for USBH logical drive */
FIL USBHFile;       /* File object for USBH */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */    

uint8_t MX_FATFS_Init(void)
{
  /*## FatFS: Link the USBH driver ###########################*/
  retUSBH = FATFS_LinkDriver(&USBH_Driver, USBHPath);
  if (retUSBH != FR_OK)
  {
    console_puts("ERROR : Cannot Link FatFs Driver! \n");
  }
  return retUSBH;

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  /* USER CODE END Init */
}

uint8_t MX_FATFS_DeInit(void)
{
  retUSBH = FATFS_UnLinkDriver(USBHPath);
  if (retUSBH != FR_OK)
  {
    console_puts("ERROR : Cannot UnLink FatFS Driver! \n");
  }
  return retUSBH;
}

/**
  * @brief Mount FatFs
  * @param  None
  * @retval None
  */
void MX_FATFS_Mount(void)
{
  if (f_mount(&USBHFatFS, "", 0) != FR_OK)
  {
    console_puts("ERROR : Cannot Mount FatFs! \n");
  }
}


/**
  * @brief  Unmount FatFs
  * @param  None
  * @retval None
  */
void MX_FATFS_Umount(void)
{
  if (f_mount(NULL, "", 0) != FR_OK)
  {
    console_puts("ERROR : Cannot Unmount FatFs! \n");
  }
}

/**
  * @brief  Gets Time from RTC 
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
