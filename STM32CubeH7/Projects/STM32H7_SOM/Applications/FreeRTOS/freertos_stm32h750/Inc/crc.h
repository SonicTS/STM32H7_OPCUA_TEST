/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Prafulla Wadaskar <prafulla@marvell.com>
 */

#ifndef __CRC_H
#define __CRC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/* lib/crc32.c */
uint32_t crc32(uint32_t crc, char * p, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* __CRC_H */
