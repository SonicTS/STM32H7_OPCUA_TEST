#include "stm32h7xx_hal.h"
#ifdef HAL_I2C_MODULE_ENABLED
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "console.h"
#include "i2c_drvr.h"

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated from APB1 source clock = 50 MHz */
/* 0x40912732 takes in account the big rising and aims a clock of 100khz */
/* this value might be adapted when next Rev Birdie board is available */
/* #define I2C_TIMING          ((uint32_t)0x40912732) */

/* This example use TIMING to 0x00901954 to reach 400Khz speed */
/* (Rise time = 50ns, Fall time = 10ns) */
#define I2C_TIMING             ((uint32_t)0x00901954)

static I2C_HandleTypeDef *     hi2cx[4];
static SemaphoreHandle_t       hsi2cx[4];

static int i2c_get_bus_by_handle(I2C_HandleTypeDef * hi2c)
{
	int res = -1;
	int i;

	for (i = 0; i < 4; i++) {
		if (hi2cx[i] == hi2c) {
			res = i;
			break;
		}
	}
	return res;
}

I2C_HandleTypeDef * i2c_init(uint32_t bus, uint32_t freq)
{
	I2C_HandleTypeDef * hi2c;

	if (bus < 1 || bus > 4) return 0;

	if (hi2cx[bus-1]) {
		return hi2cx[bus-1];
	}
	if ((freq != 100) && (freq != 400)) return 0;

	hsi2cx[bus-1] = xSemaphoreCreateBinary();
	if (hsi2cx[bus-1] == NULL) return NULL;
	hi2c = pvPortMalloc(sizeof(I2C_HandleTypeDef));
	if (!hi2c) return NULL;
	bzero(hi2c, sizeof(I2C_HandleTypeDef));

	switch (bus)
	{
	case 1:
		hi2c->Instance = I2C1;
		break;
	case 2:
		hi2c->Instance = I2C2;
		break;
	case 3:
		hi2c->Instance = I2C3;
		break;
	case 4:
		hi2c->Instance = I2C4;
		break;
	}

	hi2c->Init.Timing          = I2C_TIMING;
	hi2c->Init.OwnAddress1     = 0x00;
	hi2c->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c->Init.OwnAddress2     = 0x00;
	hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(hi2c) != HAL_OK) {
		vPortFree(hi2c);
		hi2c = NULL;
	} else {
		if (freq == 400) i2c_fastmode_enable(hi2c);

		/* Enable and set I2C1 Interrupts to a lower priority */
		switch (bus)
		{
		case 1:
			HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
			HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
			break;
		case 2:
			HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
			HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
			break;
		case 3:
			HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
			HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
			break;
		case 4:
			HAL_NVIC_SetPriority(I2C4_EV_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C4_EV_IRQn);
			HAL_NVIC_SetPriority(I2C4_ER_IRQn, 0x0F, 0);
			HAL_NVIC_EnableIRQ(I2C4_ER_IRQn);
			break;
		}
	}

	hi2cx[bus-1] = hi2c;
	return hi2c;
}

int i2c_deinit(I2C_HandleTypeDef * hi2c)
{
	int res = -1;
	int bus = i2c_get_bus_by_handle(hi2c);

	if (bus >= 0) {
		res = 0;
		HAL_I2C_DeInit(hi2cx[bus]);
		vPortFree(hi2cx[bus]);
		hi2cx[bus] = 0;
		vSemaphoreDelete(hsi2cx[bus]);
	}
	return res;
}

uint32_t i2c_read(I2C_HandleTypeDef * hi2c, uint16_t i2c_addr, uint16_t mode,
		uint16_t data_addr, uint8_t * data, uint16_t size)
{
	uint16_t m;
	uint32_t bus;
	uint32_t res;

	if (mode == 0) {
		m = I2C_MEMADD_SIZE_8BIT;
	}
	else {
		m = I2C_MEMADD_SIZE_16BIT;
	}

	i2c_addr <<= 1;
	res = HAL_I2C_IsDeviceReady(hi2c, i2c_addr, 1, 1000);
	if (res == HAL_OK) {
		while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
		HAL_I2C_Mem_Read_IT(hi2c, i2c_addr, data_addr, m, data, size);
		bus = i2c_get_bus_by_handle(hi2c);
		xSemaphoreTake(hsi2cx[bus], portMAX_DELAY);
		res = HAL_I2C_GetError(hi2c);
	}
	else {
		res = HAL_I2C_ERROR_TIMEOUT;
	}
	return res;
}

uint32_t i2c_write(I2C_HandleTypeDef * hi2c, uint16_t i2c_addr, uint16_t mode,
		uint16_t data_addr, uint8_t * data, uint16_t size)
{
	uint16_t m;
	uint32_t bus;
	uint32_t res;

	if (mode == 0) {
		m = I2C_MEMADD_SIZE_8BIT;
	}
	else {
		m = I2C_MEMADD_SIZE_16BIT;
	}

	i2c_addr <<= 1;
	res = HAL_I2C_IsDeviceReady(hi2c, i2c_addr, 1, 1000);
	if (res == HAL_OK) {
		while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY);
		HAL_I2C_Mem_Write_IT(hi2c, i2c_addr, data_addr, m, data, size);
		bus = i2c_get_bus_by_handle(hi2c);
		xSemaphoreTake(hsi2cx[bus], portMAX_DELAY);
		res = HAL_I2C_GetError(hi2c);
	}
	else {
		res = HAL_I2C_ERROR_TIMEOUT ;
	}
	return res;
}

int i2c_fastmode_enable(I2C_HandleTypeDef * hi2c)
{
	int ret = 0;

	if ((i2c_get_bus_by_handle(hi2c)) != -1) {
		switch ((uint32_t)hi2c->Instance) {
		case (uint32_t)I2C1:
			HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
			break;
		case (uint32_t)I2C2:
			HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C2);
			break;
		case (uint32_t)I2C3:
			HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C3);
			break;
		case (uint32_t)I2C4:
			HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C4);
			break;
		}
	}
	else {
		ret = -1;
	}
	return ret;
}

int i2c_fastmode_disable(I2C_HandleTypeDef * hi2c)
{
	int ret = 0;

	if ((i2c_get_bus_by_handle(hi2c)) != -1) {
		switch ((uint32_t)hi2c->Instance) {
		case (uint32_t)I2C1:
			HAL_I2CEx_DisableFastModePlus(I2C_FASTMODEPLUS_I2C1);
			break;
		case (uint32_t)I2C2:
			HAL_I2CEx_DisableFastModePlus(I2C_FASTMODEPLUS_I2C2);
			break;
		case (uint32_t)I2C3:
			HAL_I2CEx_DisableFastModePlus(I2C_FASTMODEPLUS_I2C3);
			break;
		case (uint32_t)I2C4:
			HAL_I2CEx_DisableFastModePlus(I2C_FASTMODEPLUS_I2C4);
			break;
		}
	}
	else {
		ret = -1;
	}
	return ret;
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef * hi2c)
{
	uint32_t bus;

#ifdef DEBUG
	console_puts("HAL_I2C_MemRxCpltCallback\r\n");
#endif
	bus = i2c_get_bus_by_handle(hi2c);
	xSemaphoreGiveFromISR(hsi2cx[bus], NULL);
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef * hi2c)
{
	uint32_t bus;

#ifdef DEBUG
	console_puts("HAL_I2C_MemTxCpltCallback\r\n");
#endif
	bus = i2c_get_bus_by_handle(hi2c);
	xSemaphoreGiveFromISR(hsi2cx[bus], NULL);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	uint32_t bus;

#ifdef DEBUG
	console_puts("HAL_I2C_ErrorCallback\r\n");
#endif
	bus = i2c_get_bus_by_handle(hi2c);
	xSemaphoreGiveFromISR(hsi2cx[bus], NULL);
}

void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(hi2cx[0]);
}

void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(hi2cx[0]);
}

void I2C2_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(hi2cx[1]);
}

void I2C2_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(hi2cx[1]);
}

void I2C3_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(hi2cx[2]);
}

void I2C3_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(hi2cx[2]);
}

void I2C4_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(hi2cx[3]);
}

void I2C4_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(hi2cx[3]);
}
#endif
