#include "stm32h7xx_hal.h"
#ifdef HAL_UART_MODULE_ENABLED
#include "FreeRTOS.h"
#include "string.h"
#include "task.h"
#include "queue.h"
#include "console.h"
#include "uart_drvr.h"
#include "uart_test.h"

#define CHAR_EOF 0x04
#define BUF_SIZE (1024)
static uint8_t buf[BUF_SIZE];


BaseType_t cmd_uart_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char * args = pcCommandString + 9;
	UART_InitTypeDef port_init;
	uart_handler * h;
	uint32_t port;
	uint32_t baudrate;
	uint32_t cnt;
	uint16_t count;
	uint8_t end_of_test = 0;
	int res;
	int i;

	sscanf(args, "%ld %ld %ld", &port, &baudrate, &cnt);

	port_init.BaudRate        = baudrate;
	port_init.WordLength      = UART_WORDLENGTH_8B;
	port_init.StopBits        = UART_STOPBITS_1;
	port_init.Parity          = UART_PARITY_NONE;
	port_init.HwFlowCtl       = UART_HWCONTROL_NONE;
	port_init.Mode            = UART_MODE_TX_RX;
	port_init.OverSampling    = UART_OVERSAMPLING_16;
	port_init.ClockPrescaler  = UART_PRESCALER_DIV1;
	port_init.OneBitSampling  = UART_ONE_BIT_SAMPLE_DISABLE;

	h = uart_init(port, UART_DMA, UART_DMA, UART_FIFOMODE_ENABLE, UART_RXFIFO_THRESHOLD_1_8, UART_TXFIFO_THRESHOLD_1_8, &port_init);
	if (h == NULL) {
		sprintf(wBuf, "cmd_uart_test: uart_init failed.");
		return pdFALSE;
	}

	do
	{
		count = (cnt < BUF_SIZE) ? cnt : BUF_SIZE;
		res = uart_read(h, buf, count);
		if (res > 0) {
			for (i = 0; i < res; i++) {
				if (buf[i] == CHAR_EOF) {
					end_of_test = 1;
					break;
				}
			}
			if (end_of_test) res = i;
			uart_write(h, buf, res);
			cnt -= res;
		}
		else {
			sprintf(wBuf, "uart_read() error");
			goto Exit;
		}
	} while ((cnt > 0) && (end_of_test == 0));
	sprintf(wBuf, "uart test completed.");
Exit:
	uart_deinit(h);
	return pdFALSE;
}
#endif
