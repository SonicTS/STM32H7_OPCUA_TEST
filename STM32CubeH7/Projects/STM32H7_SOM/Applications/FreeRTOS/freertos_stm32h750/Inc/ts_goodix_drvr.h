#ifndef __TS_GOODIX_DRVR_H
#define __TS_GOODIX_DRVR_H

#include "stm32h7xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	I2C_HandleTypeDef * hi2c;
	uint16_t            devid;
	uint16_t            i2c_addr;
	uint32_t            pin_int;
	uint32_t            cfg_len;
	uint16_t            max_width;
	uint16_t            max_height;
} ts_goodix_handler;

ts_goodix_handler * ts_goodix_init(uint8_t bus, uint8_t i2c_addr, uint16_t max_width, uint16_t max_height,
		GPIO_TypeDef * port, uint32_t pin);
int ts_goodix_read_input(ts_goodix_handler * hts, uint16_t * x, uint16_t * y);
void ts_goodix_gpio_exti_callback(uint32_t gpio_pin);

#ifdef __cplusplus
}
#endif

#endif /* __TS_GOODIX_DRVR_H */
