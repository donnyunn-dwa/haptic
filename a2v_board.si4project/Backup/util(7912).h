/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UTIL_H
#define __UTIL_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "main.h"

void util_flash_tuning_write(uint8_t *pData, uint16_t len);
void util_flash_tuning_read(uint8_t *pData, uint16_t len);
bool util_flash_tuning_isEmpty(void);

bool util_flash_waveform_isEmpty(void);

void util_waveform_to_flash(void);
void util_flash_to_waveform(void);


#ifdef __cplusplus
	}
#endif
#endif /* __UTIL_H */

