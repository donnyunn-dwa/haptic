#include "dw7914.h"

#include "../Res/freqLow.h"
#include "../Res/freqMid.h"
#include "../Res/freqHigh.h"

#include <string.h>

DW7914_RegisterTypeDef hDW7914;

const uint8_t dw7914_defValue[DW7914_REG_NUM] = {
	DW7914_CHIP_ID_DEF_VALUE,
	DW7914_STATUS0_DEF_VALUE,
	DW7914_INTZ_EN_DEF_VALUE,
	DW7914_TRIG_CTRL_DEF_VALUE,
	DW7914_PWM_FREQ_DEF_VALUE,
	DW7914_BOOST_OUTPUT_DEF_VALUE,
	DW7914_BOOST_OPTION_DEF_VALUE,
	DW7914_BOOST_MODE_DEF_VALUE,
	DW7914_VBAT_DEF_VALUE,
	DW7914_VMH_DEF_VALUE, 0xFF,
	DW7914_VD_CLAMP_DEF_VALUE,
	DW7914_MODE_DEF_VALUE,
	DW7914_PLAYBACK_DEF_VALUE,
	DW7914_RTP_INPUT_DEF_VALUE,
	DW7914_MEM_GAIN_DEF_VALUE,
	DW7914_WAVE_SEQ0_DEF_VALUE,
	DW7914_WAVE_SEQ1_DEF_VALUE,
	DW7914_WAVE_SEQ2_DEF_VALUE,
	DW7914_WAVE_SEQ3_DEF_VALUE,
	DW7914_WAVE_SEQ4_DEF_VALUE,
	DW7914_WAVE_SEQ5_DEF_VALUE,
	DW7914_WAVE_SEQ6_DEF_VALUE,
	DW7914_WAVE_SEQ7_DEF_VALUE,
	DW7914_WAVE_SEQ_LOOP0_DEF_VALUE,
	DW7914_WAVE_SEQ_LOOP1_DEF_VALUE,
	DW7914_WAVE_SEQ_LOOP2_DEF_VALUE,
	DW7914_WAVE_SEQ_LOOP3_DEF_VALUE,
	DW7914_MAIN_SEQ_LOOP_DEF_VALUE,
	DW7914_BRAKE0_M_WAVE_DEF_VALUE,
	DW7914_BRAKE1_M_WAVE_DEF_VALUE,
	DW7914_BRAKE_M_CTRL_DEF_VALUE,
	DW7914_TRIG1_R_WAVE_DEF_VALUE,
	DW7914_BRAKE0_R1_WAVE_DEF_VALUE,
	DW7914_BRAKE1_R1_WAVE_DEF_VALUE,
	DW7914_TRIG1_F_WAVE_DEF_VALUE,
	DW7914_BRAKE0_F1_WAVE_DEF_VALUE,
	DW7914_BRAKE1_F1_WAVE_DEF_VALUE,
	DW7914_BRAKE_T1_CTRL_DEF_VALUE,
	DW7914_TRIG2_R_WAVE_DEF_VALUE,
	DW7914_BRAKE0_R2_WAVE_DEF_VALUE,
	DW7914_BRAKE1_R2_WAVE_DEF_VALUE,
	DW7914_TRIG2_F_WAVE_DEF_VALUE,
	DW7914_BRAKE0_F2_WAVE_DEF_VALUE,
	DW7914_BRAKE1_F2_WAVE_DEF_VALUE,
	DW7914_BRAKE_T2_CTRL_DEF_VALUE,
	DW7914_TRIG3_R_WAVE_DEF_VALUE,
	DW7914_BRAKE0_R3_WAVE_DEF_VALUE,
	DW7914_BRAKE1_R3_WAVE_DEF_VALUE,
	DW7914_TRIG3_F_WAVE_DEF_VALUE,
	DW7914_BRAKE0_F3_WAVE_DEF_VALUE,
	DW7914_BRAKE1_F3_WAVE_DEF_VALUE,
	DW7914_BRAKE_T3_CTRL_DEF_VALUE,
	DW7914_TRACK_CTRL0_DEF_VALUE,
	DW7914_TRACK_CTRL1_DEF_VALUE,
	DW7914_TRACK_CTRL2_DEF_VALUE,
	DW7914_TRACK0_WAVE_DEF_VALUE,
	DW7914_TRACK1_WAVE_DEF_VALUE,
	DW7914_BRAKE0_T_WAVE_DEF_VALUE,
	DW7914_BRAKE1_T_WAVE_DEF_VALUE,
	DW7914_BRAKE_AT_CTRL_DEF_VALUE,
	DW7914_ZXD_CTRL0_DEF_VALUE,
	DW7914_ZXD_CTRL1_DEF_VALUE,
	DW7914_LRA_F0_CAL_DEF_VALUE,
	DW7914_LRA_F0_INH_DEF_VALUE,
	DW7914_LRA_F0_INL_DEF_VALUE,
	DW7914_LRA_F0_OS_DEF_VALUE,
	DW7914_LRA_F0_MH_DEF_VALUE,
	DW7914_LRA_F0_ML_DEF_VALUE,
	DW7914_STATUS1_DEF_VALUE,
	DW7914_TRIG_DET_EN_DEF_VALUE,
	DW7914_RAM_ADDRH_DEF_VALUE,
	DW7914_RAM_ADDRL_DEF_VALUE,
	DW7914_RAM_DATA_DEF_VALUE,
	DW7914_FIFO_ADDRH_DEF_VALUE,
	DW7914_FIFO_ADDRL_DEF_VALUE,
	DW7914_FIFO_LEVELH_DEF_VALUE,
	DW7914_FIFO_LEVELL_DEF_VALUE,
	DW7914_FIFO_STATUSH_DEF_VALUE,
	DW7914_FIFO_STATUSL_DEF_VALUE,
	DW7914_RAM_CHKSUM3_DEF_VALUE,
	DW7914_RAM_CHKSUM2_DEF_VALUE,
	DW7914_RAM_CHKSUM1_DEF_VALUE,
	DW7914_RAM_CHKSUM0_DEF_VALUE,
	DW7914_SWRST_DEF_VALUE
};

/**
 ** for using STM HAL Libraries
 **/
extern I2C_HandleTypeDef hi2c1;

inline void delay_ms(uint32_t ms)
{
	HAL_Delay(ms);
}

inline void enable_EN_pin(void)
{
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN1_Pin, GPIO_PIN_SET);
}

int i2c_write(uint16_t regAddr, uint8_t *pData, uint16_t len)
{
	int ret = 0;
	
	if (HAL_I2C_Mem_Write(&hi2c1, DW7914_DEV_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, pData, len, 100) != HAL_OK) {
		Error_Handler();
		ret = -1;
	}

	return ret;
}

int i2c_read(uint16_t regAddr, uint8_t *pData, uint16_t len)
{
	int ret = 0;
	
	if (HAL_I2C_Mem_Read(&hi2c1, DW7914_DEV_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, pData, len, 100) != HAL_OK) {
		Error_Handler();
		ret = -1;
	}

	return ret;
}

#ifdef FLASH_TUNING

#endif

/**
 ** driver functions
 **/ 
void dw7914_go(void)
{
	uint8_t tx[1];

	tx[0] = 0x01;
	//i2c_write(DW7914_PLAYBACK, tx, 1);
	i2c_write(0x09, tx, 1);
}

void dw7914_stop(void)
{
	uint8_t tx[1];

	tx[0] = 0x00;
	//i2c_write(DW7914_PLAYBACK, tx, 1);
	i2c_write(0x09, tx, 1);
}

void dw7914_swrst(void)
{
	uint8_t tx[1];

	tx[0] = 0x01;
	//i2c_write(DW7914_SWRST, tx, 1);
	i2c_write(0x2F, tx, 1);
}
/* porting dw7912 
void dw7914_clr_brake(DW7914_RegisterTypeDef *haptic)
{
	haptic->mode = (haptic->mode) & (~MODE_AUTO_BRAKE);
	i2c_write(DW7914_MODE, &(haptic->mode), 1);
}

void dw7914_set_brake(DW7914_RegisterTypeDef *haptic)
{
	haptic->mode = (haptic->mode) | MODE_AUTO_BRAKE;
	i2c_write(DW7914_MODE, &(haptic->mode), 1);
}
*/
void dw7914_set_vdClamp(DW7914_WAVEFORM_ID idWaveform, uint8_t vdClamp)
{
	uint16_t tmp16;
	uint8_t tx[3];

	tmp16 = idWaveform * 5;
	tx[0] = (uint8_t)(tmp16 >> 8);
	tx[1] = (uint8_t)(tmp16 & 0xff);
	tx[2] = vdClamp;
	//i2c_write(DW7914_RAM_ADDRH, tx, 3);
	i2c_write(0x1B, tx, 3);
}

void dw7914_set_wave_seq(DW7914_WAVEFORM_ID idWaveform)
{
	uint8_t tx[1];
	
	tx[0] = idWaveform;
	//i2c_write(DW7914_WAVE_SEQ0, tx, 1);
	i2c_write(0x0C, tx, 1);
}

void dw7914_seq_loop_setup(DW7914_RegisterTypeDef *haptic)
{
	//i2c_write(DW7914_WAVE_SEQ0, (haptic->wave_seq), 8);
	//i2c_write(DW7914_WAVE_SEQ_LOOP0, (haptic->wave_seq_loop), 4);	
	//i2c_write(DW7914_MAIN_SEQ_LOOP, &(haptic->main_seq_loop), 1);	
	i2c_write(0x0C, (haptic->wave_seq), 8);
	i2c_write(0x14, (haptic->wave_seq_loop), 4);	
	i2c_write(0x18, &(haptic->main_seq_loop), 1);	
}

void dw7914_write_header_to_memory(DW7914_MemoryHeader *header)
{
	uint16_t tmp16;
	uint8_t tx[5];

	tmp16 = ((header->waveNumber - 1) * 5) + 1;
	tx[0] = (uint16_t)(tmp16 >> 8);
	tx[1] = (uint8_t)(tmp16 & 0xff);
	//i2c_write(DW7914_RAM_ADDRH, tx, 2);
	i2c_write(0x1B, tx, 2);

	tx[0] = (uint8_t)(header->waveAddr >> 8);
	tx[1] = (uint8_t)(header->waveAddr & 0xff);
	tx[2] = (uint8_t)(header->waveSize>> 8);
	tx[3] = (uint8_t)(header->waveSize& 0xff);
	tx[4] = header->vdClamp;
	//i2c_write(DW7914_RAM_DATA, tx, 5);
	i2c_write(0x1D, tx, 5);
}

void dw7914_write_waveform_to_memory(DW7914_MemoryData *waveform)
{
	uint8_t tx[2];

	tx[0] = (uint8_t)(waveform->addr >> 8);
	tx[1] = (uint8_t)(waveform->addr & 0xff);
	//i2c_write(DW7914_RAM_ADDRH, tx, 2);
	i2c_write(0x1B, tx, 2);

	//i2c_write(DW7914_RAM_DATA, (uint8_t*)waveform->pData, waveform->size);
	i2c_write(0x1D, (uint8_t*)waveform->pData, waveform->size);
}

void dw7914_RAM_init(DW7914_RegisterTypeDef *haptic)
{
	DW7914_MemoryHeader header;
	DW7914_MemoryData waveform;
	uint16_t waveAddr = DW7914_WAVEFORM_START_ADDR;

	waveform.addr = waveAddr;
	waveform.pData = (uint8_t*)freqHigh + DW7914_HEADER_SIZE;
	waveform.size = freqHigh_length - DW7914_HEADER_SIZE;
	dw7914_write_waveform_to_memory(&waveform);
	header.waveNumber = WAVEFORM_ID_FREQHIGH;
	header.waveAddr = waveform.addr;
	header.waveSize = waveform.size;
	header.vdClamp = haptic->vd_clamp*2;
	dw7914_write_header_to_memory(&header);
	waveAddr += waveform.size;	

	waveform.addr = waveAddr;
	waveform.pData = (uint8_t*)freqMid + DW7914_HEADER_SIZE;
	waveform.size = freqMid_length - DW7914_HEADER_SIZE;
	dw7914_write_waveform_to_memory(&waveform);
	header.waveNumber = WAVEFORM_ID_FREQMID;
	header.waveAddr = waveform.addr;
	header.waveSize = waveform.size;
	header.vdClamp = haptic->vd_clamp;
	dw7914_write_header_to_memory(&header);
	waveAddr += waveform.size;	

	waveform.addr = waveAddr;
	waveform.pData = (uint8_t*)freqLow + DW7914_HEADER_SIZE;
	waveform.size = freqLow_length - DW7914_HEADER_SIZE;
	dw7914_write_waveform_to_memory(&waveform);
	header.waveNumber = WAVEFORM_ID_FREQLOW;
	header.waveAddr = waveform.addr;
	header.waveSize = waveform.size;
	header.vdClamp = haptic->vd_clamp*2;
	dw7914_write_header_to_memory(&header);
	waveAddr += waveform.size;	
}

void dw7914_init(DW7914_RegisterTypeDef *haptic)
{
	uint8_t buf[1];
	
	memcpy(haptic, dw7914_defValue, DW7914_REG_NUM);

	enable_EN_pin();
	delay_ms(50);

	dw7914_swrst();
	delay_ms(10);

	// Initial Check
	//i2c_read(DW7914_CHIP_ID, buf, 1);
	i2c_read(0x00, buf, 1);
	//if(buf[0] != haptic->chip_id) {
	//	Error_Handler();
	//}

	//i2c_read(DW7914_STATUS0, buf, 1);
	i2c_read(0x01, buf, 1);

	// Initial Setup
	//haptic->mode = MODE_MEM_PLAYBACK;
	//i2c_write(DW7914_MODE, &(haptic->mode), 1);
	haptic->mode = 0x01;
	i2c_write(0x03, &(haptic->mode), 1);
	
	//haptic->pwm_freq = PWM_FREQ_PWM_FREQ_48KHZ;
	//i2c_write(DW7914_PWM_FREQ, &(haptic->pwm_freq), 1);
	haptic->pwm_freq = 0x01;
	i2c_write(0x04, &(haptic->pwm_freq), 1);
	
	//haptic->boost_output = 0x00;//0x6B;
	//i2c_write(DW7914_BOOST_OUTPUT, &(haptic->boost_output), 1);
	haptic->boost_output = 0x00;//0x6B;
	i2c_write(0x05, &(haptic->boost_output), 1);

	//haptic->boost_option = 0x30;//0x0C;
	//i2c_write(DW7914_BOOST_OPTION, &(haptic->boost_option), 1);
	
	//haptic->boost_mode = BOOST_MODE_BST_ADAPT;
	//i2c_write(DW7914_BOOST_MODE, &(haptic->boost_mode), 1);
	haptic->boost_mode = 0x01;
	i2c_write(0x06, &(haptic->boost_mode), 1);
	
	//haptic->vd_clamp = VD_CLAMP_1000;
	//i2c_write(DW7914_VD_CLAMP, &(haptic->vd_clamp), 1);
	haptic->vd_clamp = VD_CLAMP_0720;
	i2c_write(0x08, &(haptic->vd_clamp), 1);
	
/*brake
	haptic->brake_m_ctrl = 0x28;
	i2c_write(DW7914_BRAKE_M_CTRL, &(haptic->brake_m_ctrl), 1);

	haptic->brake0_m_wave = WAVEFORM_ID_SINE60_BRAKE;
	i2c_write(DW7914_BRAKE0_M_WAVE, &(haptic->brake0_m_wave), 1);

	haptic->brake1_m_wave = WAVEFORM_ID_SINE60_BRAKE;
	i2c_write(DW7914_BRAKE1_M_WAVE, &(haptic->brake1_m_wave), 1);	
break*/

	// write the waveforms to RAM
	dw7914_RAM_init(haptic);

	// setting sequence loop
	haptic->wave_seq[0] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_FREQMID);
	haptic->wave_seq[1] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[2] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[3] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[4] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[5] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[6] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	haptic->wave_seq[7] = (WAVE_SEQ_WAVEFORM_ID | WAVEFORM_ID_NONE);
	
	haptic->wave_seq_loop[0] =  0x3;			
	haptic->wave_seq_loop[0] |= 0x0<<4;			
	haptic->wave_seq_loop[1] =  0x0;			
	haptic->wave_seq_loop[1] |= 0x0<<4;			
	haptic->wave_seq_loop[2] =  0x0;			
	haptic->wave_seq_loop[2] |= 0x0<<4;		
	haptic->wave_seq_loop[3] =  0x0;	
	haptic->wave_seq_loop[3] |= 0x0<<4;			
	
	haptic->main_seq_loop = 0x0;
	dw7914_seq_loop_setup(haptic);

}
