#include "stm32h7xx_hal.h"
#ifdef HAL_UART_MODULE_ENABLED
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "dma_alloc.h"
#include "console.h"
#include "uart_drvr.h"

#define MAX_UART_PORTS	8
#define DMA_RX_BUF_SIZE	1024
#define DMA_TX_BUF_SIZE	1024
#define NUM_RX_BUFS		64
#define RX_BUF_MASK		(NUM_RX_BUFS -1)

typedef struct rx_dma_bd_s
{
	uint8_t *	dma_addr;
	uint16_t	count;
	uint16_t	left_to_copy;
	uint8_t		busy;
	uint8_t		timeout;
} rx_dma_bd_t;

typedef struct rx_dma_buf_s
{
	rx_dma_bd_t bds[NUM_RX_BUFS];
} rx_dma_buf_t;

typedef struct uart_drvr_queue_s
{
	uint8_t * addr;
	uint8_t * ubuf;
	uint16_t  size;

} uart_drvr_queue_t;

static uart_handler *    huarts[MAX_UART_PORTS] = {0};
static SemaphoreHandle_t hsuart_rx_int[MAX_UART_PORTS];
static SemaphoreHandle_t hsuart_tx_int[MAX_UART_PORTS];
static SemaphoreHandle_t hsuart_rx_op[MAX_UART_PORTS];
static SemaphoreHandle_t hsuart_tx_op[MAX_UART_PORTS];
static SemaphoreHandle_t hsuart_rx_dma[MAX_UART_PORTS];
static uint8_t *         uarts_dma_tx[MAX_UART_PORTS] = {0};
static rx_dma_buf_t      uarts_dma_rx[MAX_UART_PORTS];
static uint32_t          user_rx_dma_desc[MAX_UART_PORTS];
static uint32_t          drvr_rx_dma_desc[MAX_UART_PORTS];
static uint8_t           rx_dma_strt[MAX_UART_PORTS];

static int uart_drvr_cp_dma_data(uint32_t port, uint8_t * to_addr, uint16_t cnt)
{
	uint32_t indx;
	volatile rx_dma_bd_t * dma_bd;
	uint8_t * from_addr;
	uint16_t wrt_bytes = 0;
	uint16_t c;
	uint8_t cp_end = 0;

	do {
		indx = user_rx_dma_desc[port];
		dma_bd = &(uarts_dma_rx[port].bds[indx]);
		if (dma_bd->busy == 0) {
			int res;
			if (rx_dma_strt[port] != 2) {
				drvr_rx_dma_desc[port] = indx;
				res = HAL_UART_Receive_DMA(&huarts[port]->hal_uart_hndl, dma_bd->dma_addr, DMA_RX_BUF_SIZE);
				if (res != HAL_OK) return -res;
				rx_dma_strt[port] = 2;
			}
			xSemaphoreTake(hsuart_rx_int[port], portMAX_DELAY);
			vTaskDelay(5 / portTICK_PERIOD_MS);
		}
		c = (cnt < dma_bd->left_to_copy) ? cnt : dma_bd->left_to_copy;
		from_addr = dma_bd->dma_addr;
		memcpy(to_addr + wrt_bytes, from_addr + dma_bd->count, c);
		cnt -= c;
		wrt_bytes += c;
		taskENTER_CRITICAL();
		dma_bd->count += c;
		dma_bd->left_to_copy -= c;
		taskEXIT_CRITICAL();
		if (dma_bd->left_to_copy == 0) {
			cp_end = dma_bd->timeout;
			taskENTER_CRITICAL();
			dma_bd->count = 0;
			dma_bd->busy = 0;
			dma_bd->timeout = 0;
			taskEXIT_CRITICAL();
			if (cp_end == 0) {
				user_rx_dma_desc[port] = (indx + 1) & RX_BUF_MASK;
			}
		}
	} while ((cnt > 0) && (cp_end == 0));
	return wrt_bytes;
}

/*
 * Configure the DMA handler for transmit
 */
static void uart_cfg_dma_tx(uart_handler * h) {
	__HAL_RCC_DMA1_CLK_ENABLE();
	switch (h->port) {
	case 0:
		h->hdma_tx.Instance = DMA1_Stream0;
		h->hdma_tx.Init.Request = DMA_REQUEST_USART1_TX;
		h->dma_tx_irqn = DMA1_Stream0_IRQn;
		break;
	case 1:
		h->hdma_tx.Instance = DMA1_Stream1;
		h->hdma_tx.Init.Request = DMA_REQUEST_USART2_TX;
		h->dma_tx_irqn = DMA1_Stream1_IRQn;
		break;
	case 2:
		h->hdma_tx.Instance = DMA1_Stream2;
		h->hdma_tx.Init.Request = DMA_REQUEST_USART3_TX;
		h->dma_tx_irqn = DMA1_Stream2_IRQn;
		break;
	case 3:
		h->hdma_tx.Instance = DMA1_Stream3;
		h->hdma_tx.Init.Request = DMA_REQUEST_UART4_TX;
		h->dma_tx_irqn = DMA1_Stream3_IRQn;
		break;
	case 4:
		h->hdma_tx.Instance = DMA1_Stream4;
		h->hdma_tx.Init.Request = DMA_REQUEST_UART5_TX;
		h->dma_tx_irqn = DMA1_Stream4_IRQn;
		break;
	case 5:
		h->hdma_tx.Instance = DMA1_Stream5;
		h->hdma_tx.Init.Request = DMA_REQUEST_USART6_TX;
		h->dma_tx_irqn = DMA1_Stream5_IRQn;
		break;
	case 6:
		h->hdma_tx.Instance = DMA1_Stream6;
		h->hdma_tx.Init.Request = DMA_REQUEST_UART7_TX;
		h->dma_tx_irqn = DMA1_Stream6_IRQn;
		break;
	case 7:
		h->hdma_tx.Instance = DMA1_Stream7;
		h->hdma_tx.Init.Request = DMA_REQUEST_UART8_TX;
		h->dma_tx_irqn = DMA1_Stream7_IRQn;
		break;
	}
	h->hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
	h->hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
	h->hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
	h->hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	h->hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	h->hdma_tx.Init.Mode                = DMA_NORMAL;
	h->hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
	h->hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	h->hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	h->hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
	h->hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
	HAL_DMA_Init(&h->hdma_tx);
	__HAL_LINKDMA(&h->hal_uart_hndl, hdmatx, h->hdma_tx);
}
/*
 * Configure the DMA handler for receive
 */
static void uart_cfg_dma_rx(uart_handler * h) {
	__HAL_RCC_DMA2_CLK_ENABLE();
	switch (h->port) {
	case 0:
		h->hdma_rx.Instance = DMA2_Stream0;
		h->hdma_rx.Init.Request = DMA_REQUEST_USART1_RX;
		h->dma_rx_irqn = DMA2_Stream0_IRQn;
		break;
	case 1:
		h->hdma_rx.Instance = DMA2_Stream1;
		h->hdma_rx.Init.Request = DMA_REQUEST_USART2_RX;
		h->dma_rx_irqn = DMA2_Stream1_IRQn;
		break;
	case 2:
		h->hdma_rx.Instance = DMA2_Stream2;
		h->hdma_rx.Init.Request = DMA_REQUEST_USART3_RX;
		h->dma_rx_irqn = DMA2_Stream2_IRQn;
		break;
	case 3:
		h->hdma_rx.Instance = DMA2_Stream3;
		h->hdma_rx.Init.Request = DMA_REQUEST_UART4_RX;
		h->dma_rx_irqn = DMA2_Stream3_IRQn;
		break;
	case 4:
		h->hdma_rx.Instance = DMA2_Stream4;
		h->hdma_rx.Init.Request = DMA_REQUEST_UART5_RX;
		h->dma_rx_irqn = DMA2_Stream4_IRQn;
		break;
	case 5:
		h->hdma_rx.Instance = DMA2_Stream5;
		h->hdma_rx.Init.Request = DMA_REQUEST_USART6_RX;
		h->dma_rx_irqn = DMA2_Stream5_IRQn;
		break;
	case 6:
		h->hdma_rx.Instance = DMA2_Stream6;
		h->hdma_rx.Init.Request = DMA_REQUEST_UART7_RX;
		h->dma_rx_irqn = DMA2_Stream6_IRQn;
		break;
	case 7:
		h->hdma_rx.Instance = DMA2_Stream7;
		h->hdma_rx.Init.Request = DMA_REQUEST_UART8_RX;
		h->dma_rx_irqn = DMA2_Stream7_IRQn;
		break;
	}
	h->hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
	h->hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
	h->hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
	h->hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	h->hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
	h->hdma_rx.Init.Mode                = DMA_NORMAL;
	h->hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
	h->hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
	h->hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
	h->hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
	h->hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
	HAL_DMA_Init(&h->hdma_rx);
	__HAL_LINKDMA(&h->hal_uart_hndl, hdmarx, h->hdma_rx);
}

uart_handler * uart_init(uint32_t port, uint32_t rx_mode, uint32_t tx_mode, uint32_t fifo_mode, uint32_t rx_fifo_thresh, uint32_t tx_fifo_thresh, UART_InitTypeDef * port_init)
{
	uart_handler * h;

	if ((port < 1) || (port > MAX_UART_PORTS)) return 0;
	port = port - 1;
	if (huarts[port]) return huarts[port];

	if (rx_mode == UART_DMA) {
		if (uarts_dma_rx[port].bds[0].dma_addr == NULL) {
			uint8_t * addr;

			if ((addr = dma_alloc(DMA_RX_BUF_SIZE * NUM_RX_BUFS)) == NULL) {
				return 0;
			}
			for (int i = 0; i < NUM_RX_BUFS; i++) {
				uarts_dma_rx[port].bds[i].dma_addr = addr + i * DMA_RX_BUF_SIZE;
				uarts_dma_rx[port].bds[i].count = 0;
				uarts_dma_rx[port].bds[i].left_to_copy = 0;
				uarts_dma_rx[port].bds[i].busy = 0;
				uarts_dma_rx[port].bds[i].timeout = 0;
			}
		}
		user_rx_dma_desc[port] = 0;
		drvr_rx_dma_desc[port] = 0;
		rx_dma_strt[port] = 0;
	}
	if ((tx_mode == UART_DMA) && (uarts_dma_tx[port] == NULL)) {
		if ((uarts_dma_tx[port] = dma_alloc(DMA_TX_BUF_SIZE)) == NULL) {
			return 0;
		}
	}

	huarts[port] = pvPortMalloc(sizeof(uart_handler));
	if (huarts[port] == NULL) return 0;

	h = huarts[port];
	memcpy(&h->hal_uart_hndl.Init, port_init, sizeof(UART_InitTypeDef));
	switch (port) {
	case 0:
		h->hal_uart_hndl.Instance = USART1;
		h->uart_irqn = USART1_IRQn;
		break;
	case 1:
		h->hal_uart_hndl.Instance = USART2;
		h->uart_irqn = USART2_IRQn;
		break;
	case 2:
		h->hal_uart_hndl.Instance = USART3;
		h->uart_irqn = USART3_IRQn;
		break;
	case 3:
		h->hal_uart_hndl.Instance = UART4;
		h->uart_irqn = UART4_IRQn;
		break;
	case 4:
		h->hal_uart_hndl.Instance = UART5;
		h->uart_irqn = UART5_IRQn;
		break;
	case 5:
		h->hal_uart_hndl.Instance = USART6;
		h->uart_irqn = USART6_IRQn;
		break;
	case 6:
		h->hal_uart_hndl.Instance = UART7;
		h->uart_irqn = UART7_IRQn;
		break;
	case 7:
		h->hal_uart_hndl.Instance = UART8;
		h->uart_irqn = UART8_IRQn;
		break;
	}

	if (HAL_UART_Init(&h->hal_uart_hndl) != HAL_OK) {
		vPortFree(huarts[port]);
		huarts[port] = 0;
		return 0;
	}

	if (HAL_UARTEx_SetRxFifoThreshold(&h->hal_uart_hndl, rx_fifo_thresh) != HAL_OK) {
		vPortFree(huarts[port]);
		huarts[port] = 0;
		return 0;
	}

	if (HAL_UARTEx_SetTxFifoThreshold(&h->hal_uart_hndl, tx_fifo_thresh) != HAL_OK) {
		vPortFree(huarts[port]);
		huarts[port] = 0;
		return 0;
	}

	if (fifo_mode == UART_FIFOMODE_ENABLE) {
		if (HAL_UARTEx_EnableFifoMode(&h->hal_uart_hndl) != HAL_OK) {
			vPortFree(huarts[port]);
			huarts[port] = 0;
			return 0;
		}
	}

	h->port = port;
	h->rx_mode = rx_mode;
	if (rx_mode == UART_DMA) {
		uart_cfg_dma_rx(h);
	}
	h->tx_mode = tx_mode;
	if (tx_mode == UART_DMA) {
		uart_cfg_dma_tx(h);
	}

	hsuart_rx_int[port] = xSemaphoreCreateBinary();
	hsuart_tx_int[port] = xSemaphoreCreateBinary();
	hsuart_rx_op[port]  = xSemaphoreCreateMutex();
	hsuart_tx_op[port]  = xSemaphoreCreateMutex();
	hsuart_rx_dma[port] = xSemaphoreCreateBinary();

	if ((rx_mode != UART_POLLING) || (tx_mode != UART_POLLING)) {
		HAL_NVIC_SetPriority(h->uart_irqn, 0x0F, 0);
		HAL_NVIC_EnableIRQ(h->uart_irqn);
	}
	if (rx_mode == UART_DMA) {
		HAL_NVIC_SetPriority(h->dma_rx_irqn, 0x0F, 0);
	    WRITE_REG(h->hal_uart_hndl.Instance->RTOR, 0xFF);
	    SET_BIT(h->hal_uart_hndl.Instance->CR1, USART_CR1_RTOIE);
	    SET_BIT(h->hal_uart_hndl.Instance->CR2, UART_RECEIVER_TIMEOUT_ENABLE);

		HAL_NVIC_EnableIRQ(h->dma_rx_irqn);
	}
	if (tx_mode == UART_DMA) {
		HAL_NVIC_SetPriority(h->dma_tx_irqn, 0x0F, 0);
		HAL_NVIC_EnableIRQ(h->dma_tx_irqn);
	}
	return h;
}

HAL_StatusTypeDef uart_deinit(uart_handler * huart)
{
	HAL_StatusTypeDef res;
	uint32_t port;

	port = huart->port;
	switch (port) {
	case 0:
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
		break;
	case 1:
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();
		break;
	case 2:
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();
		break;
	case 3:
		__HAL_RCC_UART4_FORCE_RESET();
		__HAL_RCC_UART4_RELEASE_RESET();
		break;
	case 4:
		__HAL_RCC_UART5_FORCE_RESET();
		__HAL_RCC_UART5_RELEASE_RESET();
		break;
	case 5:
		__HAL_RCC_USART6_FORCE_RESET();
		__HAL_RCC_USART6_RELEASE_RESET();
		break;
	case 6:
		__HAL_RCC_UART7_FORCE_RESET();
		__HAL_RCC_UART7_RELEASE_RESET();
		break;
	case 7:
		__HAL_RCC_UART8_FORCE_RESET();
		__HAL_RCC_UART8_RELEASE_RESET();
		break;
	}
	res = HAL_UART_DeInit(&huart->hal_uart_hndl);
	if ((huart->rx_mode != UART_POLLING) || (huart->tx_mode != UART_POLLING)) {
		HAL_NVIC_DisableIRQ(huart->uart_irqn);
	}
	if (huart->rx_mode == UART_DMA) {
	    CLEAR_BIT(huart->hal_uart_hndl.Instance->CR1, USART_CR1_RTOIE);
	    CLEAR_BIT(huart->hal_uart_hndl.Instance->CR2, UART_RECEIVER_TIMEOUT_ENABLE);
		HAL_NVIC_DisableIRQ(huart->dma_rx_irqn);
		HAL_DMA_DeInit(&huart->hdma_rx);
	}
	if (huart->tx_mode == UART_DMA) {
		HAL_NVIC_DisableIRQ(huart->dma_tx_irqn);
		HAL_DMA_DeInit(&huart->hdma_tx);
	}
	vPortFree(huarts[port]);
	huarts[port] = 0;
	return res;
}

int uart_read(uart_handler * huart, uint8_t * pdata, uint16_t size)
{
	int res = 0;
	uint32_t port = huart->port;

	xSemaphoreTake(hsuart_rx_op[port], portMAX_DELAY);
	switch (huart->rx_mode) {
	case UART_POLLING:
		res = HAL_UART_Receive(&huart->hal_uart_hndl, pdata, size, 0xFFFF);
		if (res != HAL_OK) {
			res = -res;
		}
		else {
			res = size;
		}
		break;
	case UART_INTERRUPT:
		res = HAL_UART_Receive_IT(&huart->hal_uart_hndl, pdata, size);
		if (res != HAL_OK) {
			res = -res;
		}
		else {
			res = size;
			xSemaphoreTake(hsuart_rx_int[port], portMAX_DELAY);
		}
		break;
	case UART_DMA:
		res = uart_drvr_cp_dma_data(port, pdata, size);
		break;
	}
	xSemaphoreGive(hsuart_rx_op[port]);
	return res;
}

int uart_write(uart_handler * huart, uint8_t * pdata, uint16_t size)
{
	uint32_t port = huart->port;
	uint16_t wbytes;
	uint16_t wsize;
	int res = 0;

	xSemaphoreTake(hsuart_tx_op[port], portMAX_DELAY);
	switch (huart->tx_mode) {
	case UART_POLLING:
		res = HAL_UART_Transmit(&huart->hal_uart_hndl, pdata, size, 0xFFFF);
		if (res != HAL_OK) {
			res = -res;
		}
		else {
			res = size;
		}
		break;
	case UART_INTERRUPT:
		res = HAL_UART_Transmit_IT(&huart->hal_uart_hndl, pdata, size);
		if (res != HAL_OK) {
			res = -res;
		}
		else {
			res = size;
			xSemaphoreTake(hsuart_tx_int[huart->port], portMAX_DELAY);
		}
		break;
	case UART_DMA:
		wbytes = 0;
		do {
			if ((size - wbytes) > DMA_TX_BUF_SIZE) {
				wsize = DMA_TX_BUF_SIZE;
			}
			else {
				wsize = size - wbytes;
			}
			memcpy(uarts_dma_tx[port], pdata + wbytes, wsize);
			res = HAL_UART_Transmit_DMA(&huart->hal_uart_hndl, uarts_dma_tx[port], wsize);
			if (res != HAL_OK) {
				res = -res;
			}
			else {
				xSemaphoreTake(hsuart_tx_int[port], portMAX_DELAY);
			}
			wbytes += wsize;
		} while ((wbytes < size) && (res >= 0));
		if (res >= 0) res = size;
		break;
	}
	xSemaphoreGive(hsuart_tx_op[port]);
	return res;
}

static void uart_drvr_rx_callback(uart_handler * h)
{
	uint32_t port = h->port;
	uint32_t indx;
	rx_dma_bd_t * dma_bd;

	if (h->rx_mode == UART_DMA) {
		indx = drvr_rx_dma_desc[port];
		dma_bd = &(uarts_dma_rx[port].bds[indx]);
		if (!dma_bd->left_to_copy) {
			dma_bd->left_to_copy = DMA_RX_BUF_SIZE;
		}
		dma_bd->count = 0;
		dma_bd->busy = 1;
		if (rx_dma_strt[port] == 2) {
			drvr_rx_dma_desc[port] = (indx + 1) & RX_BUF_MASK;
			indx = drvr_rx_dma_desc[port];
			dma_bd = &(uarts_dma_rx[port].bds[indx]);
			HAL_UART_Receive_DMA(&h->hal_uart_hndl, dma_bd->dma_addr, DMA_RX_BUF_SIZE);
		}
	}
	xSemaphoreGiveFromISR(hsuart_rx_int[port], NULL);
}
static void HAL_UART_RxTimeoutCallback(uart_handler * h)
{
	UART_HandleTypeDef * huart = &h->hal_uart_hndl;
	uint16_t count = __HAL_DMA_GET_COUNTER(huart->hdmarx);
	uint32_t port = h->port;
	uint32_t indx;
	rx_dma_bd_t * dma_bd;

	indx = drvr_rx_dma_desc[port];
	dma_bd = &(uarts_dma_rx[port].bds[indx]);
	dma_bd->left_to_copy = DMA_RX_BUF_SIZE - count;
	dma_bd->timeout = 1;
	rx_dma_strt[port] = 1;
	huart->hdmarx->XferCpltCallback(huart->hdmarx);
	HAL_DMA_Abort(huart->hdmarx);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	switch ((uint32_t)(huart->Instance)) {
	case (uint32_t)USART1:
		uart_drvr_rx_callback(huarts[0]);
		break;
	case (uint32_t)USART2:
		uart_drvr_rx_callback(huarts[1]);
		break;
	case (uint32_t)USART3:
		uart_drvr_rx_callback(huarts[2]);
		break;
	case (uint32_t)UART4:
		uart_drvr_rx_callback(huarts[3]);
		break;
	case (uint32_t)UART5:
		uart_drvr_rx_callback(huarts[4]);
		break;
	case (uint32_t)USART6:
		uart_drvr_rx_callback(huarts[5]);
		break;
	case (uint32_t)UART7:
		uart_drvr_rx_callback(huarts[6]);
		break;
	case (uint32_t)UART8:
		uart_drvr_rx_callback(huarts[7]);
		break;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	switch ((uint32_t)(huart->Instance)) {
	case (uint32_t)USART1:
		xSemaphoreGiveFromISR(hsuart_tx_int[0], NULL);
		break;
	case (uint32_t)USART2:
		xSemaphoreGiveFromISR(hsuart_tx_int[1], NULL);
		break;
	case (uint32_t)USART3:
		xSemaphoreGiveFromISR(hsuart_tx_int[2], NULL);
		break;
	case (uint32_t)UART4:
		xSemaphoreGiveFromISR(hsuart_tx_int[3], NULL);
		break;
	case (uint32_t)UART5:
		xSemaphoreGiveFromISR(hsuart_tx_int[4], NULL);
		break;
	case (uint32_t)USART6:
		xSemaphoreGiveFromISR(hsuart_tx_int[5], NULL);
		break;
	case (uint32_t)UART7:
		xSemaphoreGiveFromISR(hsuart_tx_int[6], NULL);
		break;
	case (uint32_t)UART8:
		xSemaphoreGiveFromISR(hsuart_rx_int[7], NULL);
		break;
	}
}

void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[0]->hdma_tx);
}

void DMA1_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[1]->hdma_tx);
}

void DMA1_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[2]->hdma_tx);
}

void DMA1_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[3]->hdma_tx);
}

void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[4]->hdma_tx);
}

void DMA1_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[5]->hdma_tx);
}

void DMA1_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[6]->hdma_tx);
}

void DMA1_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[7]->hdma_tx);
}

void DMA2_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[0]->hdma_rx);
}

void DMA2_Stream1_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[1]->hdma_rx);
}

void DMA2_Stream2_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[2]->hdma_rx);
}

void DMA2_Stream3_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[3]->hdma_rx);
}

void DMA2_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[4]->hdma_rx);
}

void DMA2_Stream5_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[5]->hdma_rx);
}

void DMA2_Stream6_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[6]->hdma_rx);
}

void DMA2_Stream7_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&huarts[7]->hdma_rx);
}

void USART1_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[0]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[0]);
		return;
    }
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void USART2_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[1]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[1]);
		return;
    }
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void USART3_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[2]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[2]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void UART4_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[3]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[3]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void UART5_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[4]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[4]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void USART6_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[5]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[5]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void UART7_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[6]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[6]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

void UART8_IRQHandler(void)
{
	UART_HandleTypeDef * hal_uart_hndl = &huarts[7]->hal_uart_hndl;
	volatile uint32_t isrflags = READ_REG(hal_uart_hndl->Instance->ISR);
	volatile uint32_t cr1its   = READ_REG(hal_uart_hndl->Instance->CR1);

	if (((isrflags & USART_ISR_RTOF) != RESET) && ((cr1its & USART_CR1_RTOIE) != RESET)) {
		__HAL_UART_CLEAR_FLAG(hal_uart_hndl, USART_ICR_RTOCF);
		HAL_UART_RxTimeoutCallback(huarts[7]);
		return;
	}
	HAL_UART_IRQHandler(hal_uart_hndl);
}

#endif
