#ifndef __UART_DRVR_H
#define __UART_DRVR_H

#ifdef __cplusplus
extern "C" {
#endif

#define UART_POLLING	0
#define UART_INTERRUPT	1
#define UART_DMA		2

typedef struct {
	UART_HandleTypeDef hal_uart_hndl;
	DMA_HandleTypeDef  hdma_rx;
	DMA_HandleTypeDef  hdma_tx;
	uint32_t           port;
	IRQn_Type          dma_rx_irqn;
	IRQn_Type          dma_tx_irqn;
	IRQn_Type          uart_irqn;
	uint32_t           rx_mode;
	uint32_t           tx_mode;
} uart_handler;

#include "stm32h7xx_hal.h"

uart_handler *    uart_init(uint32_t port, uint32_t rx_mode, uint32_t tx_mode, uint32_t fifo_mode, uint32_t rx_fifo_thresh, uint32_t tx_fifo_thresh, UART_InitTypeDef * port_init);
HAL_StatusTypeDef uart_deinit(uart_handler * huart);
int               uart_read(uart_handler * huart, uint8_t * pdata, uint16_t size);
int               uart_write(uart_handler * huart, uint8_t * pdata, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* __UART_DRVR_H */
