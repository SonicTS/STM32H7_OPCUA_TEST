/**
  ******************************************************************************
  * @file    FreeRTOS/freertos_stm32h750/Src/main.c
  * @author  MCD Application Team
  * @author  Emcraft Systems (www.emcraft.com)
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "main.h"
#include "ethernetif.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "FreeRTOS.h"
#include "FreeRTOS_CLI.h"
#include "task.h"
#include "dma_alloc.h"
#include "console.h"
#include "i2c_test.h"
#include "gpio_test.h"
#include "rtc_test.h"
#include "wdt_test.h"
#include "sdram_test.h"
#include "uart_test.h"
#include "gui.h"
#include "lcd_test.h"
#include "pwm_test.h"
#include "adc_test.h"
#include "spi_test.h"
#include "tim_test.h"
#include "ubifs_test.h"
#include "fatfs.h"
#include "usb_host.h"
#include "usb_test.h"
#include "uboot_env.h"
#include "sdram_info.h"
#include "reliable_upgrade.h"

#include "open62541.h"

extern void vCommandConsoleTask( void *pvParameters );

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define configTASK_STACK_CLI		(1024)
#define configTASK_STACK_GUI		(1024)
#define configTASK_PRIORITY_CLI		(tskIDLE_PRIORITY + 1)
// Example IP address: 192.168.1.100
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   127

// Example Netmask: 255.255.255.0
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

// Example Gateway address: 192.168.1.1
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static HeapRegion_t dma_alloc_region[] = {
		{(uint8_t *)DMA_REGION_START, DMA_REGION_SIZE},
		{ NULL, 0 }
};
struct netif gnetif;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void register_cmmands(void);
static void StartThread(void * argument);
static void Netif_Config(void);
//static void MPU_Config(void);
void ethernet_link_status_updated(struct netif *netif);
static void opcua_thread(void *arg);
static void manuallyDefineSonicAmp(UA_Server *server);
static UA_StatusCode sendCommandCallback(UA_Server *server,
        const UA_NodeId *sessionId, void *sessionHandle,
        const UA_NodeId *methodId, void *methodContext,
        const UA_NodeId *objectId, void *objectContext,
        size_t inputSize, const UA_Variant *input,
        size_t outputSize, UA_Variant *output);
static void beforeReadFrequency(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
        const UA_NodeId *nodeid, void *nodeContext,
        const UA_NumericRange *range, const UA_DataValue *data);

static const CLI_Command_Definition_t cmd_timer_cnt_command = {
		"timer_cnt",                                    /* The command string to type. */
		"timer_cnt",                                    /* Help String */
		cmd_timer_cnt,                                  /* The function to run. */
		0                                               /* Param1: 1 */
};
static const CLI_Command_Definition_t cmd_spi_flash_test_command = {
		"spi_flash_test",                               /* The command string to type. */
		"spi_flash_test <spi_bus>",                     /* Help String */
		cmd_spi_flash_test,                             /* The function to run. */
		1                                               /* Param1: 1 */
};
static const CLI_Command_Definition_t cmd_adc_se_command = {
		"adc_se",                                       /* The command string to type. */
		"adc_se <nadc> <chnl>",                         /* Help String */
		cmd_adc_se,                                     /* The function to run. */
		2                                               /* Param1: 2 */
};
static const CLI_Command_Definition_t cmd_pwm_start_command = {
		"pwm_start",                                    /* The command string to type. */
		"pwm_start <tim> <chnl> <period> <duty>",       /* Help String */
		cmd_pwm_start,                                  /* The function to run. */
		4                                               /* Param1: 4 */
};
static const CLI_Command_Definition_t cmd_pwm_stop_command = {
		"pwm_stop",                                    /* The command string to type. */
		"pwm_stop <tim> <chnl>",                       /* Help String */
		cmd_pwm_stop,                                  /* The function to run. */
		2                                              /* Param1: 2 */
};
static const CLI_Command_Definition_t cmd_sdram_test_command = {
		"sdram_test",                                   /* The command string to type. */
		"sdram_test <ptrn> <cnt>",                      /* Help String */
		cmd_sdram_test,                                 /* The function to run. */
		2                                               /* Param1: 2 */
};
static const CLI_Command_Definition_t cmd_uart_test_command = {
		"uart_test",                                    /* The command string to type. */
		"uart_test <port> <baudrate> <cnt>",            /* Help String */
		cmd_uart_test,                                  /* The function to run. */
		3                                               /* Param1: 3 */
};
static const CLI_Command_Definition_t cmd_wdt_stop_command = {
		"wdt_stop",                                     /* The command string to type. */
		"wdt_stop",                                     /* Help String */
		cmd_wdt_stop,                                   /* The function to run. */
		0                                               /* Param1: none */
};
static const CLI_Command_Definition_t cmd_rtc_get_date_command = {
		"rtc_get_date",                                 /* The command string to type. */
		"rtc_get_date",                                 /* Help String */
		cmd_rtc_get_date,                               /* The function to run. */
		0                                               /* Param1: none */
};
static const CLI_Command_Definition_t cmd_rtc_set_date_command = {
		"rtc_set_date",                                 /* The command string to type. */
		"rtc_set_date <date>",                          /* Help String */
		cmd_rtc_set_date,                               /* The function to run. */
		1                                               /* Param1: <date> */
};
static const CLI_Command_Definition_t cmd_i2c_scan_command = {
		"i2c_scan",                                     /* The command string to type. */
		"i2c_scan <bus>",                               /* Help String */
		cmd_i2c_scan,                                   /* The function to run. */
		1                                               /* Param1: <bus> */
};
static const CLI_Command_Definition_t cmd_i2c_read_command = {
		"i2c_read",                                     /* The command string to type. */
		"i2c_read <bus> <i2c_addr> <addr_data> <cnt>",  /* Help String */
		cmd_i2c_read,                                   /* The function to run. */
		4                                               /* Param1:  */
};
static const CLI_Command_Definition_t cmd_i2c_write_command = {
		"i2c_write",                                     /* The command string to type. */
		"i2c_write <bus> <i2c_addr> <addr_data> <data>", /* Help String */
		cmd_i2c_write,                                   /* The function to run. */
		4                                                /* Param1:  */
};
static const CLI_Command_Definition_t cmd_lcdtest_command  = {
		"lcdtest",                                 /* The command string to type. */
		"lcdtest",                                 /* Help String */
		cmd_lcdtest,                               /* The function to run. */
		0                                          /* Param1: varname=value */
};
static const CLI_Command_Definition_t cmd_ubipart_command  = {
		"ubipart",                                 /* The command string to type. */
		"ubipart <mtdpart>",                       /* Help String */
		cmd_ubipart_test,                          /* The function to run. */
		1                                          /* Param1: name of mtd partition */
};
static const CLI_Command_Definition_t cmd_ubifsmount_command  = {
		"ubifsmount",                              /* The command string to type. */
		"ubifsmount <vol>",                        /* Help String */
		cmd_ubifsmount_test,                      /* The function to run. */
		1                                          /* Param1: path within mounted ubifs parititon */
};
static const CLI_Command_Definition_t cmd_ubifsumount_command  = {
		"ubifsumount",                              /* The command string to type. */
		"ubifsumount",                              /* Help String */
		cmd_ubifsumount_test,                      /* The function to run. */
		0                                          /* Param1: */
};
static const CLI_Command_Definition_t cmd_ubifsls_command  = {
		"ubifsls",                                 /* The command string to type. */
		"ubifsls <path>",                          /* Help String */
		cmd_ubifsls_test,                          /* The function to run. */
		1                                          /* Param1: path within mounted ubifs parititon */
};
static const CLI_Command_Definition_t cmd_ubifsload_command  = {
		"ubifsload",                               /* The command string to type. */
		"ubifsload <path> <addr> <size>",          /* Help String */
		cmd_ubifsload_test,                        /* The function to run. */
		3                                          /* Param1: path to file */
};
static const CLI_Command_Definition_t cmd_ubifswrite_command  = {
		"ubifswrite",                               /* The command string to type. */
		"ubifswrite <path> <addr> <size>",          /* Help String */
		cmd_ubifswrite_test,                        /* The function to run. */
		3                                           /* Param1: path to file */
};
static const CLI_Command_Definition_t cmd_ubifsmkdir_command  = {
		"ubifsmkdir",                               /* The command string to type. */
		"ubifsmkdir <path>",                        /* Help String */
		cmd_ubifsmkdir_test,                        /* The function to run. */
		1                                           /* Param1: path to dir */
};
static const CLI_Command_Definition_t cmd_ubifsrmdir_command  = {
		"ubifsrmdir",                               /* The command string to type. */
		"ubifsrmdir <path>",                        /* Help String */
		cmd_ubifsrmdir_test,                        /* The function to run. */
		1                                           /* Param1: path to dir */
};
static const CLI_Command_Definition_t cmd_ubifsunlink_command  = {
		"ubifsunlink",                               /* The command string to type. */
		"ubifsunlink <path>",                        /* Help String */
		cmd_ubifsunlink_test,                        /* The function to run. */
		1                                           /* Param1: path to file */
};
static const CLI_Command_Definition_t cmd_ubifsstress_command  = {
		"ubifsstress",                               /* The command string to type. */
		"ubifsstress",                               /* Help String */
		cmd_ubifsstress_test,                        /* The function to run. */
		0                                            /* Param1: */
};
static const CLI_Command_Definition_t cmd_usbfatls_command  = {
		"usbfatls",                               /* The command string to type. */
		"usbfatls",                               /* Help String */
		cmd_usbfatls_test,                        /* The function to run. */
		0                                         /* Param1: */
};
static const CLI_Command_Definition_t cmd_uboot_getenv_command  = {
		"uboot_getenv",                           /* The command string to type. */
		"uboot_getenv <var>",                     /* Help String */
		cmd_uboot_getenv,                         /* The function to run. */
		1                                         /* Param1: variable name */
};
static const CLI_Command_Definition_t cmd_uboot_setenv_command  = {
		"uboot_setenv",                           /* The command string to type. */
		"uboot_setenv <var=val>",                 /* Help String */
		cmd_uboot_setenv,                         /* The function to run. */
		1                                         /* Param1: variable name */
};
static const CLI_Command_Definition_t cmd_uboot_saveenv_command  = {
		"uboot_saveenv",                          /* The command string to type. */
		"uboot_saveenv",                          /* Help String */
		cmd_uboot_saveenv,                        /* The function to run. */
		0                                         /* Param1: none */
};
static const CLI_Command_Definition_t cmd_sdram_info_command  = {
		"sdram_info",                          /* The command string to type. */
		"sdram_info",                          /* Help String */
		cmd_sdram_info,                        /* The function to run. */
		0                                         /* Param1: none */
};
static const CLI_Command_Definition_t cmd_reliable_upgrade_command = {
		"reliable_upgrade",                       /* The command string to type. */
		"reliable_upgrade <image>",               /* Help String */
		cmd_reliable_upgrade,                     /* The function to run. */
		1                                         /* Param1: image name */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32H7xx HAL library initialization:
       - TIM6 timer is configured by default as source of HAL time base, but user
         can eventually implement his proper time base source (another general purpose
         timer for application or other time source), keeping in mind that Time base
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
         handled in milliseconds basis.
         This application uses FreeRTOS, the RTOS initializes the systick to generate an interrupt each 1ms.
         The systick is then used for FreeRTOS time base.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
   */
  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /* Initialize DMA region */
  define_dma_regions(dma_alloc_region);

  /* Initialize console */
  console_init();

  xTaskCreate(gpio_test_led, "LED", configMINIMAL_STACK_SIZE,
		  NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(gpio_test_buttons, "BTNS", configMINIMAL_STACK_SIZE,
		  NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(wdt_task, "WDT", configMINIMAL_STACK_SIZE,
		  NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(tim_task, "Timer", configMINIMAL_STACK_SIZE,
		  NULL, tskIDLE_PRIORITY + 2, NULL);
  xTaskCreate(gui_ts_main, "GUI", configTASK_STACK_GUI,
		  NULL, tskIDLE_PRIORITY + 1, NULL);
  xTaskCreate(vCommandConsoleTask, "CLI", configTASK_STACK_CLI,
		  NULL, configTASK_PRIORITY_CLI, NULL);
  xTaskCreate(usb_task, "USB", configMINIMAL_STACK_SIZE,
		  NULL, tskIDLE_PRIORITY + 1, NULL);
  
  
  register_cmmands();
  console_printf("Start");
  reliable_init();
  console_printf("Start start thread");
  xTaskCreate(StartThread, "START", configMINIMAL_STACK_SIZE * 4, NULL, tskIDLE_PRIORITY + 1, NULL);
  vTaskStartScheduler();
  /* We should never get here as control is now taken by the scheduler */
  for (;;);
}

static void StartThread(void * argument)
{
  /* Create tcp_ip stack thread */
  console_printf("tcp init");
  tcpip_init(NULL, NULL);
  console_printf("Netif config");
  /* Initialize the LwIP stack */
  Netif_Config();
  console_printf("Start opcua thread");
  //sys_thread_new("opcua_thread", opcua_thread, NULL, 150000, 8);
  console_printf("Init_done\n");  

  for( ;; )
  {
    /* Delete the Init Thread */
    osThreadTerminate(NULL);
  }
}

static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;

#if LWIP_DHCP
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3);
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
#endif /* LWIP_DHCP */

  /* add the network interface */
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  /*  Registers the default network interface. */
  netif_set_default(&gnetif);


#if LWIP_NETIF_LINK_CALLBACK
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);

  xTaskCreate(ethernet_link_thread, 
              "EthLink", 
              configMINIMAL_STACK_SIZE * 2, 
              &gnetif, 
              tskIDLE_PRIORITY + 1, 
              NULL);
  console_printf("Ethernet_link thread started\n");
#endif
}
void ethernet_link_status_updated(struct netif *netif)
{
  if (netif_is_up(netif))
 {
  //empty do something
 }
}

static void register_cmmands(void)
{
	FreeRTOS_CLIRegisterCommand(&cmd_i2c_scan_command);
	FreeRTOS_CLIRegisterCommand(&cmd_i2c_read_command);
	FreeRTOS_CLIRegisterCommand(&cmd_i2c_write_command);
	FreeRTOS_CLIRegisterCommand(&cmd_rtc_get_date_command);
	FreeRTOS_CLIRegisterCommand(&cmd_rtc_set_date_command);
	FreeRTOS_CLIRegisterCommand(&cmd_wdt_stop_command);
	FreeRTOS_CLIRegisterCommand(&cmd_sdram_test_command);
	FreeRTOS_CLIRegisterCommand(&cmd_uart_test_command);
	FreeRTOS_CLIRegisterCommand(&cmd_lcdtest_command);
	FreeRTOS_CLIRegisterCommand(&cmd_pwm_start_command);
	FreeRTOS_CLIRegisterCommand(&cmd_pwm_stop_command);
	FreeRTOS_CLIRegisterCommand(&cmd_adc_se_command);
	FreeRTOS_CLIRegisterCommand(&cmd_spi_flash_test_command);
	FreeRTOS_CLIRegisterCommand(&cmd_timer_cnt_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubipart_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsmount_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsumount_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsls_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsload_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifswrite_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsmkdir_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsrmdir_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsunlink_command);
	FreeRTOS_CLIRegisterCommand(&cmd_ubifsstress_command);
	FreeRTOS_CLIRegisterCommand(&cmd_usbfatls_command);
	FreeRTOS_CLIRegisterCommand(&cmd_uboot_getenv_command);
	FreeRTOS_CLIRegisterCommand(&cmd_uboot_setenv_command);
	FreeRTOS_CLIRegisterCommand(&cmd_uboot_saveenv_command);
	FreeRTOS_CLIRegisterCommand(&cmd_sdram_info_command);
	FreeRTOS_CLIRegisterCommand(&cmd_reliable_upgrade_command);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400000000 (CPU Clock)
  *            HCLK(Hz)                       = 200000000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_SPI1|RCC_PERIPHCLK_SDMMC
                              |RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_I2C4|RCC_PERIPHCLK_USB
                              |RCC_PERIPHCLK_QSPI|RCC_PERIPHCLK_FMC;
  PeriphClkInitStruct.PLL2.PLL2M = 2;
  PeriphClkInitStruct.PLL2.PLL2N = 12;
  PeriphClkInitStruct.PLL2.PLL2P = 1;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.PLL3.PLL3M = 5;
  PeriphClkInitStruct.PLL3.PLL3N = 48;
  PeriphClkInitStruct.PLL3.PLL3P = 2;
  PeriphClkInitStruct.PLL3.PLL3Q = 5;
  PeriphClkInitStruct.PLL3.PLL3R = 20;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_D1HCLK;
  PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
  PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL;
  PeriphClkInitStruct.Spi123ClockSelection = RCC_SPI123CLKSOURCE_PLL;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
  PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL3;
  PeriphClkInitStruct.I2c4ClockSelection = RCC_I2C4CLKSOURCE_D3PCLK1;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable USB Voltage detector 
  */
  HAL_PWREx_EnableUSBVoltageDetector();
}

/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  console_printf("Error_Handler!\n");
  /* USER CODE END Error_Handler_Debug */
}

static void opcua_thread(void *arg){
        //The default 64KB of memory for sending and receicing buffer caused problems to many users. With the code below, they are reduced to ~16KB
        UA_UInt32 sendBufferSize = 9000;       //64 KB was too much for my platform
        UA_UInt32 recvBufferSize = 9000;       //64 KB was too much for my platform
        UA_UInt16 portNumber = 4840;
        console_printf("No Server");
        UA_Server* mUaServer = UA_Server_new();
        UA_ServerConfig *uaServerConfig = UA_Server_getConfig(mUaServer);
        UA_String customHost = UA_STRING("192.168.1.200");
        UA_String_clear(&uaServerConfig->customHostname);
        UA_String_copy(&customHost, &uaServerConfig->customHostname);
        UA_ServerConfig_setMinimalCustomBuffer(uaServerConfig, portNumber, 0, sendBufferSize, recvBufferSize);
        //VERY IMPORTANT: Set the hostname with your IP before starting the server

        //UA_ServerConfig_setCustomHostname(uaServerConfig, UA_STRING("192.168.0.200"));

        //The rest is the same as the example
        // add a variable node to the adresspace

        /* allocations on the heap need to be freed */
		    manuallyDefineSonicAmp(mUaServer);
        UA_Boolean running = true;
        UA_StatusCode retval = UA_Server_run(mUaServer, &running);
        if(retval == UA_STATUSCODE_GOOD){
        	//sys_thread_new("UART", UARTthread, NULL, configMINIMAL_STACK_SIZE *2, osPriorityNormal);
        }
        UA_Server_delete(mUaServer);
}

static void manuallyDefineSonicAmp(UA_Server *server){
	UA_NodeId SonicAmpId;
	UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
	oAttr.displayName = UA_LOCALIZEDTEXT("en-US", "SonicAmp");
	UA_Server_addObjectNode(server, UA_NODEID_NULL, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
								  UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
								  UA_QUALIFIEDNAME(1, "SonicAmp"),
								  UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE), oAttr, NULL, &SonicAmpId);
	UA_VariableAttributes mnAttr = UA_VariableAttributes_default;
	UA_String manufacturerName = UA_STRING("usePAT");
	UA_Variant_setScalar(&mnAttr.value, &manufacturerName, &UA_TYPES[UA_TYPES_STRING]);
	mnAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ManufacturerName");
	UA_Server_addVariableNode(server, UA_NODEID_NULL, SonicAmpId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT), UA_QUALIFIEDNAME(1, "ManufacturerName"), UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), mnAttr, NULL, NULL);
	UA_VariableAttributes modelAttr = UA_VariableAttributes_default;
	    UA_String modelName = UA_STRING("SonicAmp");
	    UA_Variant_setScalar(&modelAttr.value, &modelName, &UA_TYPES[UA_TYPES_STRING]);
	    modelAttr.displayName = UA_LOCALIZEDTEXT("en-US", "ModelName");
	    UA_Server_addVariableNode(server, UA_NODEID_NULL, SonicAmpId,
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
	                              UA_QUALIFIEDNAME(1, "ModelName"),
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), modelAttr, NULL, NULL);

	    UA_VariableAttributes statusAttr = UA_VariableAttributes_default;
	    UA_Boolean status = true;
	    UA_Variant_setScalar(&statusAttr.value, &status, &UA_TYPES[UA_TYPES_BOOLEAN]);
	    statusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Status");
	    UA_Server_addVariableNode(server, UA_NODEID_NULL, SonicAmpId,
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
	                              UA_QUALIFIEDNAME(1, "Status"),
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), statusAttr, NULL, NULL);

	    UA_VariableAttributes frqAttr = UA_VariableAttributes_default;
	    UA_NodeId frqNodeId = UA_NODEID_STRING(1, "Frequency");
	    frqAttr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
	    frqAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Frequency");
	    UA_Server_addVariableNode(server, frqNodeId, SonicAmpId,
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
	                              UA_QUALIFIEDNAME(1, "Frequency"),
	                              UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE), frqAttr, NULL, NULL);
	    UA_ValueCallback callback;
	    callback.onRead = beforeReadFrequency;
	    UA_Server_setVariableNode_valueCallback(server, frqNodeId, callback);

	    // add the method for sending string commands

	    UA_MethodAttributes cmdAttr = UA_MethodAttributes_default;
	    UA_NodeId cmdNodeId = UA_NODEID_STRING(1, "Send Command");
	    UA_Argument inputArgument;
	    UA_Argument_init(&inputArgument);
	    inputArgument.description = UA_LOCALIZEDTEXT("en-US", "command to be send vie UART");
	    inputArgument.name = UA_STRING("Command");
	    inputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	    inputArgument.valueRank = UA_VALUERANK_SCALAR;
	    UA_Argument outputArgument;
	    UA_Argument_init(&outputArgument);
	    outputArgument.description = UA_LOCALIZEDTEXT("en-US", "response from command");
	    outputArgument.name = UA_STRING("Response");
	    outputArgument.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	    outputArgument.valueRank = UA_VALUERANK_SCALAR;
		cmdAttr.description = UA_LOCALIZEDTEXT("en-US", "Send command");
	    cmdAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Send command");
	    cmdAttr.executable = true;
	    cmdAttr.userExecutable = true;
	    UA_Server_addMethodNode(server, cmdNodeId, SonicAmpId,
	    						UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
								UA_QUALIFIEDNAME(1, "Send command"),
								cmdAttr, &sendCommandCallback,
								1, &inputArgument, 1, &outputArgument, NULL, NULL);
}

static void beforeReadFrequency(UA_Server *server, const UA_NodeId *sessionId, void *sessionContext,
        const UA_NodeId *nodeid, void *nodeContext,
        const UA_NumericRange *range, const UA_DataValue *data){
	//Implement call to UART Thread and think about the logic of the threads
	uint8_t *txData = (uint8_t *) "?freq\n";
  console_printf("?freq");
  HAL_StatusTypeDef status = HAL_OK;
	//HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, txData, sizeof(txData) - 1, HAL_MAX_DELAY);
	if(status != HAL_OK){
		return;
	}
	uint8_t ch;
	uint8_t response[50] = "102394";
	int index = 0;
  status = HAL_ERROR;
	do {

		//status = HAL_UART_Receive(&huart2, &ch, 1, 5000);
		if (status == HAL_OK && index < 49) { // Leave room for null terminator
			response[index++] = ch;
		}
	} while (ch != '\n' && index < 49 && status == HAL_OK);
	response[index] = '\0';
	UA_Variant value;
	UA_Int32 freq = (UA_Int32)atoi((char *)response);
	UA_Variant_setScalar(&value, &freq, &UA_TYPES[UA_TYPES_INT32]);
	UA_Server_writeValue(server, *nodeid, value);
}

static UA_StatusCode sendCommandCallback(UA_Server *server,
        const UA_NodeId *sessionId, void *sessionHandle,
        const UA_NodeId *methodId, void *methodContext,
        const UA_NodeId *objectId, void *objectContext,
        size_t inputSize, const UA_Variant *input,
        size_t outputSize, UA_Variant *output){
	uint8_t *txData = (uint8_t*)input->data;
	if(sizeof(txData) > 0){
    console_printf(txData);
		//HAL_UART_Transmit(&huart2, txData, sizeof(txData) - 1, HAL_MAX_DELAY);
	}
	uint8_t ch;
	uint8_t response[50] = "response";
	HAL_StatusTypeDef status  = HAL_ERROR;
	int index = 0;
	do {

	    //status = HAL_UART_Receive(&huart2, &ch, 1, 5000);
	    if (status == HAL_OK && index < 49) { // Leave room for null terminator
	        response[index++] = ch;
	    }
	} while (ch != '\n' && index < 49 && status == HAL_OK);
	response[index] = '\0';
	UA_Variant_setScalarCopy(output, &response, &UA_TYPES[UA_TYPES_STRING]);
	return UA_STATUSCODE_GOOD;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
