#include "stm32h7xx_hal.h"
#ifdef HAL_SPI_MODULE_ENABLED
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "console.h"
#include "spi_test.h"

#define CS_LOW HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, RESET);
#define CS_HIGH HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, SET);

static SPI_TypeDef * spi_buses[6] = {
		SPI1, SPI2, SPI3, SPI4, SPI5, SPI6
};

static void spi_test_cs_init(GPIO_TypeDef * port, uint32_t pin)
{
	GPIO_InitTypeDef gpio_init;

	gpio_init.Pin = pin;
	gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init.Pull = GPIO_PULLUP;
	gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(port, &gpio_init);
}

BaseType_t cmd_spi_flash_test(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char * args = pcCommandString + 15;
	SPI_HandleTypeDef hspi;
	uint32_t bus;
	uint8_t devid_cmd = 0x9F;
	uint8_t devid_res[20];

	sscanf(args, "%ld", &bus);
	if (bus == 0 || bus > 6) {
		sprintf(wBuf, "Illegal SPI bus number");
		return pdFALSE;
	}
	bzero(&hspi, sizeof(SPI_HandleTypeDef));

	hspi.Instance = spi_buses[bus-1];
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi.Init.NSS = SPI_NSS_HARD_OUTPUT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 7;
	hspi.Init.CRCLength = SPI_CRC_LENGTH_8BIT;
	hspi.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	hspi.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
	hspi.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
	hspi.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
	hspi.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
	hspi.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
	hspi.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
	hspi.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_ENABLE;
	hspi.Init.IOSwap = SPI_IO_SWAP_DISABLE;
	if (HAL_SPI_Init(&hspi) != HAL_OK) {
		sprintf(wBuf, "SPI Init failed");
		return pdFALSE;
	}
	spi_test_cs_init(GPIOG, GPIO_PIN_10);

	CS_LOW;
	if (HAL_SPI_Transmit(&hspi, &devid_cmd, sizeof(devid_cmd), 5000) != HAL_OK) {
		sprintf(wBuf, "SPI Transmit error");
		goto Exit;
	}

	if (HAL_SPI_Receive(&hspi, devid_res, sizeof(devid_res), 5000) != HAL_OK) {
		sprintf(wBuf, "SPI Receive error");
		goto Exit;
	}
	CS_HIGH;
	sprintf(wBuf, "Manufacturer ID:0x%02x Device ID:0x%02x%02x CFD Length:0x%02x",
			devid_res[0], devid_res[1], devid_res[2], devid_res[3]);
Exit:
	HAL_SPI_DeInit(&hspi);
	return pdFALSE;
}
#endif
