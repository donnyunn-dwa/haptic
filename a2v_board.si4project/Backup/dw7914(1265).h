/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DW7914_H
#define __DW7914_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* defines */

// i2c address
#define DW7914_DEV_ADDR (0x59<<1)
enum DW7914_REG_ADDR {
	DW7914_CHIP_ID = 0x00,
	DW7914_STATUS0,
	DW7914_INTZ_EN,
	DW7914_TRIG_CTRL,
	DW7914_PWM_FREQ,
	DW7914_BOOST_OUTPUT,
	DW7914_BOOST_OPTION,
	DW7914_BOOST_MODE,
	DW7914_VBAT,
	DW7914_VMH,
	DW7914_VD_CLAMP,
	DW7914_MODE,
	DW7914_PLAYBACK,
	DW7914_RTP_INPUT,
	DW7914_MEM_GAIN,
	DW7914_WAVE_SEQ0,
	DW7914_WAVE_SEQ1,
	DW7914_WAVE_SEQ2,
	DW7914_WAVE_SEQ3,
	DW7914_WAVE_SEQ4,
	DW7914_WAVE_SEQ5,
	DW7914_WAVE_SEQ6,
	DW7914_WAVE_SEQ7,
	DW7914_WAVE_SEQ_LOOP0,
	DW7914_WAVE_SEQ_LOOP1,
	DW7914_WAVE_SEQ_LOOP2,
	DW7914_WAVE_SEQ_LOOP3,
	DW7914_MAIN_SEQ_LOOP,
	DW7914_BRAKE0_M_WAVE,
	DW7914_BRAKE1_M_WAVE,
	DW7914_BRAKE_M_CTRL,
	DW7914_TRIG1_R_WAVE,
	DW7914_BRAKE0_R1_WAVE,
	DW7914_BRAKE1_R1_WAVE,
	DW7914_TRIG1_F_WAVE,
	DW7914_BRAKE0_F1_WAVE,
	DW7914_BRAKE1_F1_WAVE,
	DW7914_BRAKE_T1_CTRL,
	DW7914_TRIG2_R_WAVE,
	DW7914_BRAKE0_R2_WAVE,
	DW7914_BRAKE1_R2_WAVE,
	DW7914_TRIG2_F_WAVE,
	DW7914_BRAKE0_F2_WAVE,
	DW7914_BRAKE1_F2_WAVE,
	DW7914_BRAKE_T2_CTRL,
	DW7914_TRIG3_R_WAVE,
	DW7914_BRAKE0_R3_WAVE,
	DW7914_BRAKE1_R3_WAVE,
	DW7914_TRIG3_F_WAVE,
	DW7914_BRAKE0_F3_WAVE,
	DW7914_BRAKE1_F3_WAVE,
	DW7914_BRAKE_T3_CTRL,
	DW7914_TRACK_CTRL0,
	DW7914_TRACK_CTRL1,
	DW7914_TRACK_CTRL2,
	DW7914_TRACK0_WAVE,
	DW7914_TRACK1_WAVE,
	DW7914_BRAKE0_T_WAVE,
	DW7914_BRAKE1_T_WAVE,
	DW7914_BRAKE_AT_CTRL,
	DW7914_ZXD_CTRL0,
	DW7914_ZXD_CTRL1,
	DW7914_LRA_F0_CAL,
	DW7914_LRA_F0_INH,
	DW7914_LRA_F0_INL,
	DW7914_LRA_F0_OS,
	DW7914_LRA_F0_MH,
	DW7914_LRA_F0_ML,
	DW7914_STATUS1,
	DW7914_TRIG_DET_EN,
	DW7914_RAM_ADDRH,
	DW7914_RAM_ADDRL,
	DW7914_RAM_DATA,
	DW7914_FIFO_ADDRH,
	DW7914_FIFO_ADDRL,
	DW7914_FIFO_LEVELH,
	DW7914_FIFO_LEVELL,
	DW7914_FIFO_STATUSH,
	DW7914_FIFO_STATUSL,
	DW7914_RAM_CHKSUM3,
	DW7914_RAM_CHKSUM2,
	DW7914_RAM_CHKSUM1,
	DW7914_RAM_CHKSUM0,
	DW7914_SWRST = 0x5f,
	DW7914_REG_NUM = 85
};

/* register setting masks */
enum DW7914_STATUS0_MASK {
	STATUS0_SCP						= (1<<0),
	STATUS0_OCP						= (1<<1),
	STATUS0_TSD						= (1<<2),
	STATUS0_UVLO					= (1<<3),
	STATUS0_PROCESS_DONE			= (1<<4),
	STATUS0_FIFO_EMPTY				= (1<<5),
	STATUS0_FIFO_FULL				= (1<<6),
	STATUS0_TRIG_IN					= (1<<7)
};

enum DW7914_INTZ_EN_MASK {
	INTZ_EN_SCP						= (1<<0),
	INTZ_EN_OCP						= (1<<1),
	INTZ_EN_TSD						= (1<<2),
	INTZ_EN_UVLO					= (1<<3),
	INTZ_EN_PROCESS_DONE			= (1<<4),
	INTZ_EN_FIFO_EMPTY				= (1<<5),
	INTZ_EN_FIFO_FULL				= (1<<6),
	INTZ_EN_TRIG_IN					= (1<<7)
};

enum DW7914_TRIG_CTRL_MASK {
	TRIG_CTRL_TRIG_PRIORITY			= (1<<0),
	
	TRIG_CTRL_TRIG_INTERVAL_2_5MS	= (0<<1),
	TRIG_CTRL_TRIG_INTERVAL_5MS		= (1<<1),
	TRIG_CTRL_TRIG_INTERVAL_10MS	= (2<<1),
	TRIG_CTRL_TRIG_INTERVAL_20MS	= (3<<1),
	TRIG_CTRL_TRIG_INTERVAL_40MS	= (4<<1),
	TRIG_CTRL_TRIG_INTERVAL_60MS	= (5<<1),
	TRIG_CTRL_TRIG_INTERVAL_80MS	= (6<<1),
	TRIG_CTRL_TRIG_INTERVAL_100MS	= (7<<1),

	TRIG_CTRL_TRIG1_LEVEL			= (1<<4),
	TRIG_CTRL_TRIG1_MODE			= (1<<5),
	TRIG_CTRL_TRIG2_MASTER			= (1<<6),
	TRIG_CTRL_SOUND_MODE			= (1<<7)
};

// 0x04
enum DW7914_PWM_FREQ_MASK {
	PWM_FREQ_PWM_FREQ_24KHZ			= (0<<0),
	PWM_FREQ_PWM_FREQ_48KHZ			= (1<<0),
	PWM_FREQ_PWM_FREQ_96KHZ			= (2<<0),
	PWM_FREQ_PWM_FREQ_12KHZ			= (3<<0),

	PWM_FREQ_VD_CLAMP_REG			= (1<<2),

	PWM_FREQ_VD_CLAMP_TIME_NOLIMIT	= (0<<5),
	PWM_FREQ_VD_CLAMP_TIME_20MS		= (1<<5),
	PWM_FREQ_VD_CLAMP_TIME_40MS		= (2<<5),
	PWM_FREQ_VD_CLAMP_TIME_80MS		= (3<<5)
};

// 0x07
enum DW7914_BOOST_MODE_MASK {
	BOOST_MODE_BST_EN				= (1<<0),
	BOOST_MODE_BST_BYPASS			= (1<<1),
	BOOST_MODE_BST_LUMP				= (1<<2),
	BOOST_MODE_BST_ADAPT			= (1<<3),

	BOOST_MODE_WAIT					= (0xF<<4)
};

// 0x0A
enum DW7914_VD_CLAMP_MASK {
	VD_CLAMP_0000 = 0, VD_CLAMP_0040, VD_CLAMP_0080, VD_CLAMP_0120, VD_CLAMP_0160, VD_CLAMP_0200, VD_CLAMP_0240, VD_CLAMP_0280, VD_CLAMP_0320, VD_CLAMP_0360,
	VD_CLAMP_0400, VD_CLAMP_0440, VD_CLAMP_0480, VD_CLAMP_0520, VD_CLAMP_0560, VD_CLAMP_0600, VD_CLAMP_0640, VD_CLAMP_0680, VD_CLAMP_0720, VD_CLAMP_0760,
	VD_CLAMP_0800, VD_CLAMP_0840, VD_CLAMP_0880, VD_CLAMP_0920, VD_CLAMP_0960, VD_CLAMP_1000, VD_CLAMP_1040, VD_CLAMP_1080,	VD_CLAMP_1120, VD_CLAMP_1160,
	VD_CLAMP_1200, VD_CLAMP_1240, VD_CLAMP_1280, VD_CLAMP_1320, VD_CLAMP_1360, VD_CLAMP_1400, VD_CLAMP_1440, VD_CLAMP_1480, VD_CLAMP_1520, VD_CLAMP_1560, 
	VD_CLAMP_1600, VD_CLAMP_1640, VD_CLAMP_1680, VD_CLAMP_1720, VD_CLAMP_1760, VD_CLAMP_1800, VD_CLAMP_1840, VD_CLAMP_1880, VD_CLAMP_1920, VD_CLAMP_1960,
	VD_CLAMP_2000, VD_CLAMP_2040, VD_CLAMP_2080, VD_CLAMP_2120, VD_CLAMP_2160, VD_CLAMP_2200, VD_CLAMP_2240, VD_CLAMP_2280, VD_CLAMP_2320, VD_CLAMP_2360, 
	VD_CLAMP_2400, VD_CLAMP_2440, VD_CLAMP_2480, VD_CLAMP_2520,	VD_CLAMP_2560, VD_CLAMP_2600, VD_CLAMP_2640, VD_CLAMP_2680, VD_CLAMP_2720, VD_CLAMP_2760,
	VD_CLAMP_2800, VD_CLAMP_2840, VD_CLAMP_2880, VD_CLAMP_2920, VD_CLAMP_2960, VD_CLAMP_3000, VD_CLAMP_3040, VD_CLAMP_3080, VD_CLAMP_3120, VD_CLAMP_3160, 
	VD_CLAMP_3200, VD_CLAMP_3240, VD_CLAMP_3280, VD_CLAMP_3320, VD_CLAMP_3360, VD_CLAMP_3400, VD_CLAMP_3440, VD_CLAMP_3480, VD_CLAMP_3520, VD_CLAMP_3560,
	VD_CLAMP_3600, VD_CLAMP_3640, VD_CLAMP_3680, VD_CLAMP_3720, VD_CLAMP_3760, VD_CLAMP_3800, VD_CLAMP_3840, VD_CLAMP_3880, VD_CLAMP_3920, VD_CLAMP_3960, 
	VD_CLAMP_4000, VD_CLAMP_4040, VD_CLAMP_4080, VD_CLAMP_4120, VD_CLAMP_4160, VD_CLAMP_4200, VD_CLAMP_4240, VD_CLAMP_4280, VD_CLAMP_4320, VD_CLAMP_4360, 
	VD_CLAMP_4400, VD_CLAMP_4440, VD_CLAMP_4480, VD_CLAMP_4520,	VD_CLAMP_4560, VD_CLAMP_4600, VD_CLAMP_4640, VD_CLAMP_4680, VD_CLAMP_4720, VD_CLAMP_4760,
	VD_CLAMP_4800, VD_CLAMP_4840, VD_CLAMP_4880, VD_CLAMP_4920, VD_CLAMP_4960, VD_CLAMP_5000, VD_CLAMP_5040, VD_CLAMP_5080, VD_CLAMP_5120, VD_CLAMP_5160, 
	VD_CLAMP_5200, VD_CLAMP_5240, VD_CLAMP_5280, VD_CLAMP_5320, VD_CLAMP_5360, VD_CLAMP_5400, VD_CLAMP_5440, VD_CLAMP_5480, VD_CLAMP_5520, VD_CLAMP_5560,
	VD_CLAMP_5600, VD_CLAMP_5640, VD_CLAMP_5680, VD_CLAMP_5720, VD_CLAMP_5760, VD_CLAMP_5800, VD_CLAMP_5840, VD_CLAMP_5880, VD_CLAMP_5920, VD_CLAMP_5960, 
	VD_CLAMP_6000, VD_CLAMP_6040, VD_CLAMP_6080, VD_CLAMP_6120, VD_CLAMP_6160, VD_CLAMP_6200, VD_CLAMP_6240, VD_CLAMP_6280, VD_CLAMP_6320, VD_CLAMP_6360, 
	VD_CLAMP_6400, VD_CLAMP_6440, VD_CLAMP_6480, VD_CLAMP_6520,	VD_CLAMP_6560, VD_CLAMP_6600, VD_CLAMP_6640, VD_CLAMP_6680, VD_CLAMP_6720, VD_CLAMP_6760,
	VD_CLAMP_6800, VD_CLAMP_6840, VD_CLAMP_6880, VD_CLAMP_6920, VD_CLAMP_6960, VD_CLAMP_7000, VD_CLAMP_7040, VD_CLAMP_7080, VD_CLAMP_7120, VD_CLAMP_7160, 
	VD_CLAMP_7200, VD_CLAMP_7240, VD_CLAMP_7280, VD_CLAMP_7320, VD_CLAMP_7360, VD_CLAMP_7400, VD_CLAMP_7440, VD_CLAMP_7480, VD_CLAMP_7520, VD_CLAMP_7560,
	VD_CLAMP_7600, VD_CLAMP_7640, VD_CLAMP_7680, VD_CLAMP_7720, VD_CLAMP_7760, VD_CLAMP_7800, VD_CLAMP_7840, VD_CLAMP_7880, VD_CLAMP_7920, VD_CLAMP_7960, 
	VD_CLAMP_8000, VD_CLAMP_8040, VD_CLAMP_8080, VD_CLAMP_8120, VD_CLAMP_8160, VD_CLAMP_8200, VD_CLAMP_8240, VD_CLAMP_8280, VD_CLAMP_8320, VD_CLAMP_8360, 
	VD_CLAMP_8400, VD_CLAMP_8440, VD_CLAMP_8480, VD_CLAMP_8520,	VD_CLAMP_8560, VD_CLAMP_8600, VD_CLAMP_8640, VD_CLAMP_8680, VD_CLAMP_8720, VD_CLAMP_8760,
	VD_CLAMP_8800, VD_CLAMP_8840, VD_CLAMP_8880, VD_CLAMP_8920, VD_CLAMP_8960, VD_CLAMP_9000, VD_CLAMP_9040, VD_CLAMP_9080, VD_CLAMP_9120, VD_CLAMP_9160, 
	VD_CLAMP_9200, VD_CLAMP_9240, VD_CLAMP_9280, VD_CLAMP_9320, VD_CLAMP_9360, VD_CLAMP_9400, VD_CLAMP_9440, VD_CLAMP_9480, VD_CLAMP_9520, VD_CLAMP_9560,
	VD_CLAMP_9600, VD_CLAMP_9640, VD_CLAMP_9680, VD_CLAMP_9720, VD_CLAMP_9760, VD_CLAMP_9800, VD_CLAMP_9840, VD_CLAMP_9880, VD_CLAMP_9920, VD_CLAMP_9960, 
	VD_CLAMP_10000, VD_CLAMP_10040, VD_CLAMP_10080, VD_CLAMP_10120, VD_CLAMP_10160, VD_CLAMP_10200
};

// 0x0B
enum DW7914_MODE_MASK {
	MODE_RTP						= (0<<0),
	MODE_MEM_PLAYBACK				= (1<<0),
	MODE_ACTUATOR_DIAGNOSTICS		= (2<<0),

	MODE_AUTO_BRAKE					= (1<<2),
	MODE_AUTO_TRACK					= (1<<3),
	MODE_CHKSUM						= (1<<4),
	MODE_FIFO_FLUSH					= (1<<5)
};

// 0x0E
enum DW7914_MEM_GAIN_MASK {
	MEM_GAIN_100_PERCENT			= (0<<0),
	MEM_GAIN_87_5_PERCENT			= (1<<0),
	MEM_GAIN_75_PERCENT				= (2<<0),
	MEM_GAIN_62_5_PERCENT			= (3<<0),
	MEM_GAIN_50_PERCENT				= (4<<0),
	MEM_GAIN_37_5_PERCENT			= (5<<0),
	MEM_GAIN_25_PERCENT				= (6<<0),
	MEM_GAIN_12_5_PERCENT			= (7<<0),

	MEM_GAIN_MUTE_ON				= (1<<3)
};

// 0x0F~0x16
enum DW7914_WAVE_SEQ_MASK {
	WAVE_SEQ_WAVEFORM_ID			= (0<<7),
	WAVE_SEQ_DELAY					= (1<<7)
};

// 0x1E, 0x3B
enum DW7914_BRAKE_CTRL_MASK {
	BRAKE_CTRL_BRAKE0_CYCLE_1		= (0<<0),
	BRAKE_CTRL_BRAKE0_CYCLE_2		= (1<<0),
	BRAKE_CTRL_BRAKE0_CYCLE_3		= (2<<0),
	BRAKE_CTRL_BRAKE0_CYCLE_4		= (3<<0),

	BRAKE_CTRL_BRAKE1_CYCLE_1		= (0<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_2		= (1<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_3		= (2<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_4		= (3<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_5		= (4<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_6		= (5<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_7		= (6<<2),
	BRAKE_CTRL_BRAKE1_CYCLE_8		= (7<<2),

	BRAKE_CTRL_BRAKE1_VD_1_0		= (0<<5),
	BRAKE_CTRL_BRAKE1_VD_0_5		= (1<<5),
	BRAKE_CTRL_BRAKE1_VD_0_25		= (2<<5),
};


// 0x34
enum DW7914_TRACK_CTRL0_MASK {
	TRACK_CTRL0_CYCLE_1				= (0<<0),
	TRACK_CTRL0_CYCLE_2				= (1<<0),
	TRACK_CTRL0_CYCLE_3				= (2<<0),
	TRACK_CTRL0_CYCLE_4				= (3<<0),
	TRACK_CTRL0_CYCLE_5				= (4<<0),
	TRACK_CTRL0_CYCLE_6				= (5<<0),
	TRACK_CTRL0_CYCLE_7				= (6<<0),
	TRACK_CTRL0_CYCLE_8				= (7<<0),

	TRACK_CTRL0_LOOP_NONE			= (0<<3),
	TRACK_CTRL0_LOOP_1				= (1<<3),
	TRACK_CTRL0_LOOP_2				= (2<<3),
	TRACK_CTRL0_LOOP_3				= (3<<3),
	TRACK_CTRL0_LOOP_4				= (4<<3),
	TRACK_CTRL0_LOOP_5				= (5<<3),
	TRACK_CTRL0_LOOP_6				= (6<<3),
	TRACK_CTRL0_LOOP_7				= (7<<3),
	TRACK_CTRL0_LOOP_8				= (8<<3),
	TRACK_CTRL0_LOOP_9				= (9<<3),
	TRACK_CTRL0_LOOP_10				= (10<<3),
	TRACK_CTRL0_LOOP_11				= (11<<3),
	TRACK_CTRL0_LOOP_12				= (12<<3),
	TRACK_CTRL0_LOOP_13				= (13<<3),
	TRACK_CTRL0_LOOP_14				= (14<<3),
	TRACK_CTRL0_LOOP_INFINITE		= (15<<3),

	TRACK_CTRL0_PATTERN_EXTERNAL	= (0<<7),
	TRACK_CTRL0_PATTERN_INTERNAL	= (1<<7)
};

// 0x3C
enum DW7914_ZXD_CTRL0_MASK {
	ZXD_CTRL0_GND_TIME_25US			= (0<<0),
	ZXD_CTRL0_GND_TIME_50US			= (1<<0),
	ZXD_CTRL0_GND_TIME_75US			= (2<<0),
	ZXD_CTRL0_GND_TIME_100US		= (3<<0),
	ZXD_CTRL0_GND_TIME_125US		= (4<<0),
	ZXD_CTRL0_GND_TIME_150US		= (5<<0),
	ZXD_CTRL0_GND_TIME_175US		= (6<<0),
	ZXD_CTRL0_GND_TIME_200US		= (7<<0),
	ZXD_CTRL0_GND_TIME_225US		= (8<<0),
	ZXD_CTRL0_GND_TIME_250US		= (9<<0),
	ZXD_CTRL0_GND_TIME_275US		= (10<<0),
	ZXD_CTRL0_GND_TIME_300US		= (11<<0),
	ZXD_CTRL0_GND_TIME_325US		= (12<<0),
	ZXD_CTRL0_GND_TIME_350US		= (13<<0),
	ZXD_CTRL0_GND_TIME_375US		= (14<<0),
	ZXD_CTRL0_GND_TIME_400US		= (15<<0),
	
	ZXD_CTRL0_NULL_TIME_25US		= (0<<4),
	ZXD_CTRL0_NULL_TIME_50US		= (1<<4),
	ZXD_CTRL0_NULL_TIME_75US		= (2<<4),
	ZXD_CTRL0_NULL_TIME_100US		= (3<<4),
	ZXD_CTRL0_NULL_TIME_125US		= (4<<4),
	ZXD_CTRL0_NULL_TIME_150US		= (5<<4),
	ZXD_CTRL0_NULL_TIME_175US		= (6<<4),
	ZXD_CTRL0_NULL_TIME_200US		= (7<<4),
	ZXD_CTRL0_NULL_TIME_225US		= (8<<4),
	ZXD_CTRL0_NULL_TIME_250US		= (9<<4),
	ZXD_CTRL0_NULL_TIME_275US		= (10<<4),
	ZXD_CTRL0_NULL_TIME_300US		= (11<<4),
	ZXD_CTRL0_NULL_TIME_325US		= (12<<4),
	ZXD_CTRL0_NULL_TIME_350US		= (13<<4),
	ZXD_CTRL0_NULL_TIME_375US		= (14<<4),
	ZXD_CTRL0_NULL_TIME_400US		= (15<<4),
};

// 0x3D
enum DW7914_ZXD_CTRL1_MASK {
	ZXD_CTRL1_TIME_100US = 0, ZXD_CTRL1_TIME_150US, ZXD_CTRL1_TIME_200US, ZXD_CTRL1_TIME_250US,
	ZXD_CTRL1_TIME_300US, ZXD_CTRL1_TIME_350US, ZXD_CTRL1_TIME_400US, ZXD_CTRL1_TIME_450US,
	ZXD_CTRL1_TIME_500US, ZXD_CTRL1_TIME_550US, ZXD_CTRL1_TIME_600US, ZXD_CTRL1_TIME_650US,
	ZXD_CTRL1_TIME_700US, ZXD_CTRL1_TIME_750US, ZXD_CTRL1_TIME_800US, ZXD_CTRL1_TIME_850US,
	ZXD_CTRL1_TIME_900US, ZXD_CTRL1_TIME_950US, ZXD_CTRL1_TIME_1000US, ZXD_CTRL1_TIME_1050US,
	ZXD_CTRL1_TIME_1100US, ZXD_CTRL1_TIME_1150US, ZXD_CTRL1_TIME_1200US, ZXD_CTRL1_TIME_1250US,
	ZXD_CTRL1_TIME_1300US,
	
	ZXD_CTRL1_BEMF_GAIN_1X			= (0<<5),
	ZXD_CTRL1_BEMF_GAIN_2X			= (1<<5),
	ZXD_CTRL1_BEMF_GAIN_5X			= (2<<5),
	ZXD_CTRL1_BEMF_GAIN_10X			= (3<<5),
	ZXD_CTRL1_BEMF_GAIN_20X			= (4<<5),
	ZXD_CTRL1_BEMF_GAIN_40X			= (5<<5),
	ZXD_CTRL1_BEMF_GAIN_60X			= (6<<5),
	ZXD_CTRL1_BEMF_GAIN_80X			= (7<<5)
};

/* default values */
enum DW7914_DEF_VALUE {
	DW7914_CHIP_ID_DEF_VALUE 		= 0x40,
	DW7914_STATUS0_DEF_VALUE 		= 0x00,
	DW7914_INTZ_EN_DEF_VALUE 		= (INTZ_EN_UVLO | INTZ_EN_TSD | INTZ_EN_OCP | INTZ_EN_SCP),
	DW7914_TRIG_CTRL_DEF_VALUE 		= (TRIG_CTRL_TRIG_INTERVAL_10MS),
	DW7914_PWM_FREQ_DEF_VALUE 		= (PWM_FREQ_PWM_FREQ_48KHZ),
	DW7914_BOOST_OUTPUT_DEF_VALUE 	= 0x00,
	DW7914_BOOST_OPTION_DEF_VALUE 	= 0x00,
	DW7914_BOOST_MODE_DEF_VALUE 	= 0x00,
	DW7914_VBAT_DEF_VALUE 			= 0x00,
	DW7914_VMH_DEF_VALUE 			= 0xFF,
	DW7914_VD_CLAMP_DEF_VALUE 		= 0x00,
	DW7914_MODE_DEF_VALUE 			= 0x00,
	DW7914_PLAYBACK_DEF_VALUE 		= 0x00,
	DW7914_RTP_INPUT_DEF_VALUE 		= 0x00,
	DW7914_MEM_GAIN_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ0_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ1_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ2_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ3_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ4_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ5_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ6_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ7_DEF_VALUE 		= 0x00,
	DW7914_WAVE_SEQ_LOOP0_DEF_VALUE = 0x00,
	DW7914_WAVE_SEQ_LOOP1_DEF_VALUE = 0x00,
	DW7914_WAVE_SEQ_LOOP2_DEF_VALUE = 0x00,
	DW7914_WAVE_SEQ_LOOP3_DEF_VALUE = 0x00,
	DW7914_MAIN_SEQ_LOOP_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_M_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE1_M_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE_M_CTRL_DEF_VALUE 	= BRAKE_CTRL_BRAKE1_CYCLE_3,
	DW7914_TRIG1_R_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_R1_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_R1_WAVE_DEF_VALUE = 0x00,
	DW7914_TRIG1_F_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_F1_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_F1_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE_T1_CTRL_DEF_VALUE 	= BRAKE_CTRL_BRAKE1_CYCLE_3,
	DW7914_TRIG2_R_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_R2_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_R2_WAVE_DEF_VALUE = 0x00,
	DW7914_TRIG2_F_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_F2_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_F2_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE_T2_CTRL_DEF_VALUE 	= BRAKE_CTRL_BRAKE1_CYCLE_3,
	DW7914_TRIG3_R_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_R3_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_R3_WAVE_DEF_VALUE = 0x00,
	DW7914_TRIG3_F_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_F3_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE1_F3_WAVE_DEF_VALUE = 0x00,
	DW7914_BRAKE_T3_CTRL_DEF_VALUE 	= BRAKE_CTRL_BRAKE1_CYCLE_3,
	DW7914_TRACK_CTRL0_DEF_VALUE 	= (TRACK_CTRL0_PATTERN_INTERNAL),
	DW7914_TRACK_CTRL1_DEF_VALUE 	= 0x00,
	DW7914_TRACK_CTRL2_DEF_VALUE 	= 0x00,
	DW7914_TRACK0_WAVE_DEF_VALUE 	= 0x00,
	DW7914_TRACK1_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE0_T_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE1_T_WAVE_DEF_VALUE 	= 0x00,
	DW7914_BRAKE_AT_CTRL_DEF_VALUE 	= BRAKE_CTRL_BRAKE1_CYCLE_3,
	DW7914_ZXD_CTRL0_DEF_VALUE 		= (ZXD_CTRL0_NULL_TIME_100US | ZXD_CTRL0_GND_TIME_100US),
	DW7914_ZXD_CTRL1_DEF_VALUE 		= (ZXD_CTRL1_BEMF_GAIN_10X | ZXD_CTRL1_TIME_400US),
	DW7914_LRA_F0_CAL_DEF_VALUE 	= 0x02,
	DW7914_LRA_F0_INH_DEF_VALUE 	= 0x00,
	DW7914_LRA_F0_INL_DEF_VALUE 	= 0x00,
	DW7914_LRA_F0_OS_DEF_VALUE 		= 0x00,
	DW7914_LRA_F0_MH_DEF_VALUE 		= 0x00,
	DW7914_LRA_F0_ML_DEF_VALUE 		= 0x00,
	DW7914_STATUS1_DEF_VALUE 		= 0x00,
	DW7914_TRIG_DET_EN_DEF_VALUE 	= 0x00,
	DW7914_RAM_ADDRH_DEF_VALUE 		= 0x00,
	DW7914_RAM_ADDRL_DEF_VALUE 		= 0x00,
	DW7914_RAM_DATA_DEF_VALUE 		= 0x00,
	DW7914_FIFO_ADDRH_DEF_VALUE 	= 0x38,
	DW7914_FIFO_ADDRL_DEF_VALUE 	= 0x00,
	DW7914_FIFO_LEVELH_DEF_VALUE 	= 0x02,
	DW7914_FIFO_LEVELL_DEF_VALUE 	= 0x00,
	DW7914_FIFO_STATUSH_DEF_VALUE 	= 0x00,
	DW7914_FIFO_STATUSL_DEF_VALUE 	= 0x00,
	DW7914_RAM_CHKSUM3_DEF_VALUE 	= 0x00,
	DW7914_RAM_CHKSUM2_DEF_VALUE 	= 0x00,
	DW7914_RAM_CHKSUM1_DEF_VALUE 	= 0x00,
	DW7914_RAM_CHKSUM0_DEF_VALUE 	= 0x00,
	DW7914_SWRST_DEF_VALUE 			= 0x00
};

/* prototype variables */
typedef struct {
	uint8_t chip_id;
	uint8_t status0;
	uint8_t intz_en;
	uint8_t trig_ctrl;
	uint8_t pwm_freq;
	uint8_t boost_output;
	uint8_t boost_option;
	uint8_t boost_mode;
	uint8_t vbat;
	uint8_t vmh;
	uint8_t vd_clamp;
	uint8_t mode;
	uint8_t playback;
	uint8_t rtp_input;
	uint8_t mem_gain;	
	uint8_t wave_seq[8];
	uint8_t wave_seq_loop[4];
	uint8_t main_seq_loop;
	uint8_t brake0_m_wave;
	uint8_t brake1_m_wave;
	uint8_t brake_m_ctrl;
	uint8_t trig1_r_wave;
	uint8_t brake0_r1_wave;
	uint8_t brake1_r1_wave;
	uint8_t trig1_f_wave;
	uint8_t brake0_f1_wave;
	uint8_t brake1_f1_wave;
	uint8_t brake_t1_ctrl;
	uint8_t trig2_r_wave;
	uint8_t brake0_r2_wave;
	uint8_t brake1_r2_wave;
	uint8_t tirg2_f_wave;
	uint8_t brake0_f2_wave;
	uint8_t brake1_f2_wave;
	uint8_t brake_t2_ctrl;
	uint8_t trig3_r_wave;
	uint8_t brake9_r3_wave;
	uint8_t brake1_r3_wave;
	uint8_t trig3_f_wave;
	uint8_t brake0_f3_wave;
	uint8_t brake1_f3_wave;
	uint8_t brake_t3_ctrl;
	uint8_t track_ctrl0;
	uint8_t track_ctrl1;
	uint8_t track_ctrl2;
	uint8_t track0_wave;
	uint8_t track1_wave;
	uint8_t brake0_t_wave;
	uint8_t brake1_t_wave;
	uint8_t brake_at_ctrl;
	uint8_t zxd_ctrl0;
	uint8_t zxd_ctrl1;
	uint8_t lra_f0_cal;
	uint8_t lra_f0_inh;
	uint8_t lra_f0_inl;
	uint8_t lra_f0_os;
	uint8_t lra_f0_mh;
	uint8_t lra_f0_ml;
	uint8_t status1;
	uint8_t trig_det_en;
	uint8_t ram_addrh;
	uint8_t ram_addrl;
	uint8_t ram_data;
	uint8_t fifo_addrh;
	uint8_t fifo_addrl;
	uint8_t fifo_levelh;
	uint8_t fifo_levell;
	uint8_t fifo_statush;
	uint8_t fifo_statusl;
	uint8_t ram_chksum3;
	uint8_t ram_chksum2;
	uint8_t ram_chksum1;
	uint8_t ram_chksum0;
	uint8_t swrst;
} DW7914_RegisterTypeDef;

// ID List for Memory Playback
typedef enum {
	WAVEFORM_ID_NONE = 0,
	WAVEFORM_ID_FREQHIGH,
	WAVEFORM_ID_FREQMID,
	WAVEFORM_ID_FREQLOW,
	WAVEFORM_ID_MAX
} DW7914_WAVEFORM_ID;

typedef struct {
	DW7914_WAVEFORM_ID waveNumber;	// waveform number (-> header's address)
	uint16_t waveAddr;	// waveform start address
	uint16_t waveSize;	// waveform data size
	uint8_t vdClamp;
} DW7914_MemoryHeader;
#define DW7914_HEADER_SIZE 7

typedef struct {
	uint16_t addr;
	uint8_t *pData;
	uint16_t size;
} DW7914_MemoryData;
#define DW7914_WAVEFORM_START_ADDR (0x027C)

/* prototype functions */
void dw7914_set_byte(uint8_t addr, uint8_t data);
uint8_t dw7914_get_byte(uint8_t addr);
void dw7914_read_RAM(uint8_t* pData, uint16_t addr, uint16_t len);
void dw7914_write_RAM(uint8_t* pData, uint16_t addr, uint16_t len);

void dw7914_go(void);
void dw7914_stop(void);
void dw7914_set_vdClamp(DW7914_WAVEFORM_ID idWaveform, uint8_t vdClamp);
void dw7914_set_wave_seq(DW7914_WAVEFORM_ID idWaveform);
void dw7914_init(DW7914_RegisterTypeDef *haptic);

#ifdef __cplusplus
}
#endif
#endif /* __DW7914_H */

