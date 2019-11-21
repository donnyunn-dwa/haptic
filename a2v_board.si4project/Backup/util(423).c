#include "util.h"

#include "dw7914.h"

#define FLASH_TUNING_START_ADDR	((uint32_t)0x08008000)
#define FLASH_WAVEFORM_START_ADDR ((uint32_t)0x0800C000)

void util_flash_tuning_write(uint8_t *pData, uint16_t len)
{
	int i;

	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
							FLASH_FLAG_PGAERR|FLASH_FLAG_PGSERR);

	FLASH_Erase_Sector(FLASH_SECTOR_2, VOLTAGE_RANGE_2);

	for(i = 0; i < len; i++) {
		HAL_FLASH_Program(TYPEPROGRAM_BYTE, FLASH_TUNING_START_ADDR+i, pData[i]);
	}

	HAL_FLASH_Lock();
}

void util_flash_tuning_read(uint8_t *pData, uint16_t len)
{
	int i;

	for(i = 0; i < len; i++) {
		pData[i] = *(uint8_t*)(FLASH_TUNING_START_ADDR+i);
	}
}

bool util_flash_tuning_isEmpty(void)
{
	bool ret = true;
	uint32_t tmp;

	for(int i = 0; i < 16; i += 4) {
		tmp = *(uint32_t*)(FLASH_TUNING_START_ADDR + i);
		if(tmp != 0xFFFFFFFF) {
			ret = false;
			break;
		}
	}

	return ret;
}

void util_flash_waveform_write(uint8_t *pData, uint16_t addr, uint16_t len)
{
	int i;
	uint16_t startAddr = FLASH_WAVEFORM_START_ADDR + addr;

	for(i = 0; i < len; i++) {
		HAL_FLASH_Program(TYPEPROGRAM_BYTE, startAddr + i, pData[i]);
	}
}

void util_flash_waveform_read(uint8_t *pData, uint16_t addr, uint16_t len)
{
	int i;
	uint16_t startAddr = FLASH_WAVEFORM_START_ADDR + addr;

	for(i = 0; i < len; i++) {
		pData[i] = *(uint8_t*)(startAddr + i);
	}
}

bool util_flash_waveform_isEmpty(void)
{
	bool ret = true;
	uint32_t tmp;

	for(int i = 0; i < 16; i += 4) {
		tmp = *(uint32_t*)(FLASH_WAVEFORM_START_ADDR + i);
		if(tmp != 0xFFFFFFFF) {
			ret = false;
			break;
		}
	}

	return ret;
}

void util_waveform_to_flash(void)
{
	uint8_t* pData;
	uint16_t addr;
	uint16_t len = 0x40;

	HAL_FLASH_Unlock();

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR|
							FLASH_FLAG_PGAERR|FLASH_FLAG_PGSERR);

	FLASH_Erase_Sector(FLASH_SECTOR_3, VOLTAGE_RANGE_2);

	pData = (uint8_t*)malloc(len);

	for (addr = 0; addr < 0x4000; addr+=len) {
		dw7914_read_RAM(pData, addr, len);
		util_flash_waveform_write(pData, addr, len);
		
		HAL_GPIO_TogglePin(MCU_ACT_GPIO_Port, MCU_ACT_Pin);
	}
	
	free(pData);

	HAL_FLASH_Lock();
}

void util_flash_to_waveform(void)
{
	uint8_t* pData;
	uint16_t addr;
	uint16_t len = 0x40;

	pData = (uint8_t*)malloc(len);

	for (addr = 0; addr < 0x4000; addr+=len) {
		util_flash_waveform_read(pData, addr, len);
		dw7914_write_RAM(pData, addr, len);
	}
	
	free(pData);
}

