#ifndef __ENV_H
#define __ENV_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define CFG_ENV_SECTOR    3
#define CFG_ENV_BASE_ADDR (0x08060000)
#define CONFIG_ENV_SIZE   (128 * 1024)
#define ENV_HEADER_SIZE   (sizeof(uint32_t))
#define ENV_SIZE          (CONFIG_ENV_SIZE - ENV_HEADER_SIZE)

typedef struct environment_s {
	uint32_t	crc; /* CRC32 over data bytes */
#ifdef CONFIG_REDUNDAND_ENVIRONMENT
	char		flags; /* active/obsolete flags */
#endif
	char		data[ENV_SIZE]; /* Environment data      */
} env_t;

char * fw_getenv(char * name);
int    fw_setenv(char * name, char * value);

#ifdef __cplusplus
}
#endif

#endif /* __ENV_H */