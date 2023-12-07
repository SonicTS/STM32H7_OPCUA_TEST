#include <stdio.h>
#include <string.h>
#include "projdefs.h"
#include "FreeRTOS.h"
#include "console.h"
#include "i2c_drvr.h"
#include "i2c_test.h"

static uint32_t byte_swap(uint32_t a, uint32_t c)
{
	switch (c) {
		case 1:
			a = a & 0xFF;
			break;
		case 2:
			a = ((a & 0x00FF) << 8) | ((a & 0xFF00) >> 8);
			break;
		case 3:
			a = ((a & 0x00FF) << 16) |
			((a & 0xFF00) << 0) |
			((a & 0x00FF0000) >>  16);
			break;
		case 4:
			a = ((a & 0xFF) << 24) |
			((a & 0xFF00) <<  8) |
			((a & 0xFF0000) >>  8) |
			((a & 0xFF000000) >> 24);
			break;
	}
	return a;
}

BaseType_t cmd_i2c_scan(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *        args = pcCommandString + 9;
	I2C_HandleTypeDef * hi2c;
	uint32_t            bus;
	uint32_t            i;
	uint32_t            c;
	char *              cmd_output = wBuf;

	sscanf(args, "%ld", &bus);
	if (bus < 1 || bus > 4) {
		sprintf(wBuf, "Illegal I2C bus specified");
		return pdFALSE;
	}
	if ((hi2c = i2c_init(bus, 100)) == NULL) {
		sprintf(wBuf, "i2c_init error");
		return pdFALSE;
	}

	c = sprintf(cmd_output, "\r\nI2C%ld: ", bus);
	cmd_output += c;
	console_puts("Scanning.");
	for (i = 1; i < 128; i++) {
		if (HAL_I2C_IsDeviceReady(hi2c, i<<1, 1, 1000) == HAL_OK) {
			c = sprintf(cmd_output, "0x%02lx ", i);
			cmd_output += c;
		}
		HAL_Delay(100);
		if ((i % 16) == 0) console_putc('.');
	}
	return pdFALSE;
}

BaseType_t cmd_i2c_read(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *        args = pcCommandString + 9;
	char *              parse_args;
	I2C_HandleTypeDef * hi2c;
	uint32_t            bus;
	uint32_t            i2c_addr;
	uint32_t            addr;
	uint32_t            count;
	uint32_t            msize;
	uint32_t            res;
	int                 i;
	int                 c;
	char *              cmd_output = wBuf;
	uint8_t *           pdata;

	c = sscanf(args, "%ld", &bus);
	if (bus < 1 || bus > 4) {
		sprintf(wBuf, "Illegal I2C bus specified.");
		return pdFALSE;
	}
	args += c+1;
	parse_args = strchr(args, ':');
	if (parse_args == NULL) {
		sscanf(args, "%lx %lx %lx", &i2c_addr, &addr, &count);
		msize = 1;
	}
	else {
		* parse_args = 0;
		c = sscanf(args, "%lx", &i2c_addr);
		sscanf(parse_args+1, "%ld %lx %lx", &msize, &addr, &count);
		if (msize > 1) {
			sprintf(wBuf, "Illegal I2C memory address size.");
			return pdFALSE;
		}
	}
#ifdef DEBUG
	sprintf(wBuf, "bus=%ld i2c_addr=%lx msize=%ld addr=%lx count=%lx\r\n",
			bus, i2c_addr, msize, addr, count);
	console_puts(wBuf);
#endif

	if (count > 128) {
		sprintf(wBuf, "Too big the <cnt> parameter.");
		return pdFALSE;
	}
	if ((pdata = pvPortMalloc(sizeof(uint8_t) * count)) == NULL) {
		sprintf(wBuf, "Couldn't allocate memory for data.");
		return pdFALSE;
	}

	if ((hi2c = i2c_init(bus, 100)) == NULL) {
		sprintf(wBuf, "i2c_init error.");
		return pdFALSE;
	}

	res = i2c_read(hi2c, i2c_addr, msize, addr, pdata, count);
	if (res == 0) {
		for (i = 0; i < count; i++) {
			if ((i & 0xf) == 0) {
				if (i > 0) {
					sprintf(cmd_output, "\r\n");
					cmd_output += 2;
				}
				c = sprintf(cmd_output, "%02lx: ", addr+i);
				cmd_output +=c;
			}
			c = sprintf(cmd_output, "%02x ", pdata[i]);
			cmd_output +=c;
		}
	}
	else {
		sprintf(wBuf, "i2c_read error 0x%lx", res);
	}
	vPortFree(pdata);
	return pdFALSE;
}

BaseType_t cmd_i2c_write(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	const char *        args = pcCommandString + 10;
	char *              parse_args;
	I2C_HandleTypeDef * hi2c;
	uint32_t            bus;
	uint32_t            i2c_addr;
	uint32_t            addr;
	uint32_t            count;
	uint32_t            msize;
	uint8_t             data[80];
	uint32_t            hexdata;
	uint32_t            res;
	int                 c;

	c = sscanf(args, "%ld", &bus);
	if (bus < 1 || bus > 4) {
		sprintf(wBuf, "Illegal I2C bus specified.");
		return pdFALSE;
	}
	args += c+1;
	parse_args = strchr(args, ':');
	if (parse_args == NULL) {
		sscanf(args, "%lx %lx %s", &i2c_addr, &addr, data);
		msize = 1;
	}
	else {
		* parse_args = 0;
		c = sscanf(args, "%lx", &i2c_addr);
		sscanf(parse_args+1, "%ld %lx %s", &msize, &addr, data);
		if (msize > 1) {
			sprintf(wBuf, "Illegal I2C memory address size.");
			return pdFALSE;
		}
	}

	if ((data[0] == '0') && (data[1] == 'x')) {
		count = strlen((const char *)(&data[2]));
		if (count > 8) {
			sprintf(wBuf, "Illegal data");
			return pdFALSE;
		}
		sscanf((char *)(&data[2]), "%lx", &hexdata);
		count = (count / 2) + (count % 2);
		* ((uint32_t *)data) = byte_swap(hexdata, count);
	}
	else {
		count = strlen((const char *)data);
	}
#ifdef DEBUG
	sprintf(wBuf, "bus=%ld i2c_addr=%lx msize=%ld addr=%lx data=%s count=%lx\r\n",
			bus, i2c_addr, msize, addr, data, count);
	console_puts(wBuf);
#endif

	if ((hi2c = i2c_init(bus, 100)) == NULL) {
		sprintf(wBuf, "i2c_init error.");
		return pdFALSE;
	}

	res = i2c_write(hi2c, i2c_addr, msize, addr, data, count);
	if (res == 0) {
		*wBuf = 0;
	}
	else {
		sprintf(wBuf, "I2c write error 0x%lx", res);
	}
	return pdFALSE;
}
