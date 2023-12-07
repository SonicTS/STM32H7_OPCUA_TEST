#ifndef __I2C_DRVR_H
#define __I2C_DRVR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* Exported functions ------------------------------------------------------- */
I2C_HandleTypeDef * i2c_init(uint32_t bus, uint32_t freq);
int                 i2c_deinit(I2C_HandleTypeDef * hi2c);
int                 i2c_fastmode_enable(I2C_HandleTypeDef * hi2c);
int                 i2c_fastmode_disable(I2C_HandleTypeDef * hi2c);
uint32_t            i2c_read(I2C_HandleTypeDef *  hi2c,
                             uint16_t             i2c_addr,
                             uint16_t             mode,
                             uint16_t             data_addr,
                             uint8_t *            data,
                             uint16_t             size);
uint32_t            i2c_write(I2C_HandleTypeDef * hi2c,
                             uint16_t             i2c_addr,
                             uint16_t             mode,
                             uint16_t             data_addr,
                             uint8_t *            data,
                             uint16_t             size);
#ifdef __cplusplus
}
#endif

#endif /* __I2C_DRVR_H */
