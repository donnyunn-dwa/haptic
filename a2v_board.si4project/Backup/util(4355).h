/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTIL_H
#define __UTIL_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "main.h"

void util_flash_write(uint8_t *pData, uint16_t len);
void util_flash_read(uint8_t *pData, uint16_t len);
bool util_flash_isEmpty(void);

#ifdef __cplusplus
	}
#endif
#endif /* __UTIL_H */

