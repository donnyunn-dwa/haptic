#include "util.h"

#define FLASH_USER_START_ADDR	((uint32_t)0x08060000)

void util_flash_write(uint8_t *pData, uint16_t len)
{
	int i;

	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
							FLASH_FLAG_PGAERR|FLASH_FLAG_PGSERR);

	FLASH_Erase_Sector(FLASH_SECTOR_7, VOLTAGE_RANGE_2);

	for(i = 0; i < len; i++) {
		HAL_FLASH_Program(TYPEPROGRAM_BYTE, FLASH_USER_START_ADDR+i, pData[i]);
	}

	HAL_FLASH_Lock();
}

void util_flash_read(uint8_t *pData, uint16_t len)
{
	int i;

	for(i = 0; i < len; i++) {
		pData[i] = *(uint8_t*)(FLASH_USER_START_ADDR+i);
	}
}

bool util_flash_isEmpty(void)
{
	bool ret = true;
	uint32_t tmp;

	for(int i = 0; i < 4; i += 4) {
		tmp = *(uint32_t*)(FLASH_USER_START_ADDR + i);
		if(tmp != 0xFFFFFFFF) {
			ret = false;
			break;
		}
	}

	return ret;
}

