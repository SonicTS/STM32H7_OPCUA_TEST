cmake_minimum_required(VERSION 3.10)

######## Project settings ########
PROJECT(freertos_stm32h750 C ASM)

set(TARGET freertos_stm32h750.elf)
######## Build and include settings ########
set(SOURCES
  ${SOURCES}
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c
Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c
Middlewares/Third_Party/FreeRTOS/Source/croutine.c
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
Middlewares/Third_Party/FreeRTOS/Source/list.c
Middlewares/Third_Party/FreeRTOS/Source/queue.c
Middlewares/Third_Party/FreeRTOS/Source/tasks.c
Middlewares/Third_Party/FreeRTOS/Source/timers.c
Middlewares/Third_Party/FatFs/src/diskio.c
Middlewares/Third_Party/FatFs/src/ff.c
Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma2d.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hcd.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_iwdg.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_ltdc.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c

Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth.c
#Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_eth_ex.c
Drivers/BSP/Components/LAN8720/PHY_LAN8720.c
Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c
Middlewares/Third_Party/LwIP/src/core/mem.c
Middlewares/Third_Party/LwIP/src/core/init.c
Middlewares/Third_Party/LwIP/src/core/tcp_out.c
Middlewares/Third_Party/LwIP/src/core/tcp_in.c
Middlewares/Third_Party/LwIP/src/core/tcp.c
Middlewares/Third_Party/LwIP/src/core/udp.c
Middlewares/Third_Party/LwIP/src/core/inet_chksum.c
Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c
Middlewares/Third_Party/LwIP/system/OS/sys_arch.c
Middlewares/Third_Party/LwIP/src/core/sys.c
Middlewares/Third_Party/LwIP/src/core/memp.c
Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c
Middlewares/Third_Party/LwIP/src/core/ip.c
Middlewares/Third_Party/LwIP/src/core/def.c
Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c
Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c
Middlewares/Third_Party/LwIP/src/netif/ethernet.c
Middlewares/Third_Party/LwIP/src/api/tcpip.c
Middlewares/Third_Party/LwIP/src/core/netif.c
Middlewares/Third_Party/LwIP/src/api/err.c
Middlewares/Third_Party/LwIP/src/core/timeouts.c
Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c
Middlewares/Third_Party/LwIP/src/core/stats.c
Middlewares/Third_Party/LwIP/src/core/pbuf.c
Middlewares/Third_Party/LwIP/src/core/dns.c
Middlewares/Third_Party/LwIP/src/api/netdb.c
Middlewares/Third_Party/LwIP/src/api/sockets.c
Middlewares/Third_Party/LwIP/src/api/netbuf.c
Middlewares/Third_Party/LwIP/src/api/api_lib.c
Middlewares/Third_Party/LwIP/src/api/api_msg.c
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_usb.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/system_stm32h7xx.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/FreeRTOS_CLI.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/adc_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/cmd_env.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/console.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/crc32.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/ctask.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/dma_alloc.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/env.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/fatfs.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/gpio_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/gui.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/i2c_drvr.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/i2c_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/lcd.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/pwm_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/rtc_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/sdram_info.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/sdram_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/spi_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/stm32h7xx_hal_msp.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/stm32h7xx_hal_timebase_tim.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/stm32h7xx_it.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/tim_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/ts_goodix_drvr.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/uart_drvr.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/uart_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/ubifs_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/uboot_abi.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/uboot_env.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/usb_host.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/usb_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/usbh_conf.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/usbh_diskio.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/usbh_platform.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/wdt_test.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/reliable_upgrade.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/SW4STM32/startup_stm32h743xx.s
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/ethernetif.c
#Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/open62541.c
Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Src/main.c
)

add_executable(${TARGET} "${SOURCES}")

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/SW4STM32/STM32H750-SOM/freertos_stm32h750_sdram.ld")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -T ${LINKER_SCRIPT}")

target_include_directories(${TARGET} PUBLIC
  Drivers/CMSIS/Device/ST/STM32H7xx/Include
  Drivers/STM32H7xx_HAL_Driver/Inc
  Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
  Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
  Middlewares/Third_Party/FreeRTOS/Source/include
  Middlewares/Third_Party/FatFs/src
  Middlewares/ST/STM32_USB_Host_Library/Core/Inc
  Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
  Utilities/Log
  Utilities/Fonts
  Utilities/CPU
  Drivers/CMSIS/Include
  Drivers/BSP/Components/LAN8720
  Middlewares/Third_Party/LwIP/src/include
  Middlewares/Third_Party/LwIP/system
  Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/Inc
)

target_compile_definitions(${TARGET} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32H750xx
    -DUSE_IOEXPANDER
)
target_link_libraries(${TARGET} m)

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${MY_CMAKE_OBJCOPY} -O binary ${TARGET} freertos_stm32h750.bin)

add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_SOURCE_DIR}/Projects/STM32H7_SOM/Applications/FreeRTOS/freertos_stm32h750/SW4STM32/STM32H750-SOM/freertos_stm32h750_mkimage.sh "freertos_stm32h750")
