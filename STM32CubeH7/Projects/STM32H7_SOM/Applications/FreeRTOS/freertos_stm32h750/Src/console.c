#include <stdarg.h>
#include "stm32h7xx_hal.h"
#include "string.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "string.h"
#include "uart_drvr.h"
#include "console.h"

#define CONSOLE_BUF_SIZE 8
static uart_handler * hcnsl;
static uint8_t cbuf[CONSOLE_BUF_SIZE];

int console_init(void)
{
	UART_InitTypeDef port_init;
	int res = 0;

	port_init.BaudRate        = 115200;
	port_init.WordLength      = UART_WORDLENGTH_8B;
	port_init.StopBits        = UART_STOPBITS_1;
	port_init.Parity          = UART_PARITY_NONE;
	port_init.HwFlowCtl       = UART_HWCONTROL_NONE;
	port_init.Mode            = UART_MODE_TX_RX;
	port_init.OverSampling    = UART_OVERSAMPLING_16;
	port_init.ClockPrescaler  = UART_PRESCALER_DIV1;
	port_init.OneBitSampling  = UART_ONE_BIT_SAMPLE_DISABLE;

	hcnsl = uart_init(1, UART_INTERRUPT, UART_INTERRUPT, UART_FIFOMODE_DISABLE, UART_RXFIFO_THRESHOLD_1_8, UART_TXFIFO_THRESHOLD_1_8, &port_init);
	if (hcnsl == NULL) {
		res = 1;
	}
	return res;
}

void console_putc(char c)
{
	uint8_t chr = c;

	uart_write(hcnsl, &chr, 1);
}

char console_getc(void)
{
	uart_read(hcnsl, cbuf, 1);
	return cbuf[0];
}

void console_puts(char * str)
{
	uart_write(hcnsl, (uint8_t *)str, strlen(str));
}

void console_printf(const char *fmt, ...)
{
	char buf[128];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	console_puts(buf);
	console_putc('\r');
}
