#include "uart_packet.h"

#include "util.h"
#include "dw7914.h"

#define PREFIX (0x7914)
#define SUBFIX (0x00)

CMD_LIST cmd[CMD_MAX] = {
	"0000",
	"0001",
	"0002",
	"0003",
	"0004",
	"0005",
	"0006",
};

/**
 ** for using STM HAL Libraries
 **/
extern UART_HandleTypeDef huart2;

void uart_packet_Send(uint8_t *msg, int len)
{
	HAL_UART_Transmit(&huart2, msg, len, 100);
}

/**
 ** driver functions
 **/ 
uint8_t hex2asc(uint8_t hex)
{
	uint8_t asc = 0;

	if(hex < 10) {
		asc = hex + '0';
	} else if(hex < 16) {
		asc = hex + 'A' - 10;
	}
	
	return asc;
}

char asc2hex(char asc)
{
	char hex = 0xff;

	if((asc >= '0') && (asc <= '9')) {
		hex = asc - '0';
	} else if((asc >= 'A') && (asc <= 'F')) {
		hex = asc - 'A' + 0xa;
	} else if((asc >= 'a') && (asc <= 'f')) {
		hex = asc - 'a' + 0xa;
	}

	return hex;
}
 
void uart_packet_Error(void)
{
	uart_packet_Send("$ER", 4);
}

void uart_packet_Identify(uint16_t cmdCode)
{
	uint8_t res[13];
	int cnt = 0;

	*((uint16_t*)res) = PREFIX;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = cmdCode;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = 7;
	cnt += 2;
	res[cnt++] = 'D';
	res[cnt++] = 'W';
	res[cnt++] = '7';
	res[cnt++] = '9';
	res[cnt++] = '1';
	res[cnt++] = '4';
	res[cnt++] = SUBFIX;
	
	uart_packet_Send(res, cnt);
}

void uart_packet_Response(uint16_t cmdCode, uint8_t loc, uint8_t val)
{
	uint8_t res[9];
	int cnt = 0;

	*((uint16_t*)res) = PREFIX;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = cmdCode;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = 3;
	cnt += 2;
	res[cnt++] = loc;
	res[cnt++] = val;
	res[cnt++] = SUBFIX;
	
	uart_packet_Send(res, cnt);
}

void uart_packet_Debug(uint16_t cmdCode, uint8_t *tuningA2V, uint8_t loc, uint8_t len)
{
	uint8_t* res;
	int cnt = 0;

	res = (uint8_t*)malloc(7+len);
	
	*((uint16_t*)res) = PREFIX;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = cmdCode;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = len + 1;
	cnt += 2;
	for(int i = 0; i < len; i++) {
		res[cnt++] = tuningA2V[i+loc];
	}
	res[cnt++] = SUBFIX;
	
	uart_packet_Send(res, cnt);
	free(res);
}

void uart_packet_Record(uint16_t cmdCode, uint8_t *tuningA2V, uint8_t loc, uint8_t len)
{
	uint8_t* res;
	int cnt = 0;

	res = (uint8_t*)malloc(7+len);

	*((uint16_t*)res) = PREFIX;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = cmdCode;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = len + 1;
	cnt += 2;
	util_flash_tuning_read(res+cnt, len);
	cnt += len;
	res[cnt++] = SUBFIX;
	
	uart_packet_Send(res, cnt);
	free(res);
}

void uart_packet_Register(uint16_t cmdCode, uint8_t loc, uint8_t val)
{
	uint8_t res[9];
	int cnt = 0;

	*((uint16_t*)res) = PREFIX;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = cmdCode;
	cnt += 2;
	*((uint16_t*)(res+cnt)) = 3;
	cnt += 2;
	res[cnt++] = loc;
	res[cnt++] = val;
	res[cnt++] = SUBFIX;

	uart_packet_Send(res, cnt);
}

void uart_packet_Exec(int cmd_num, uint8_t *tuningA2V, uint8_t *info)
{
	switch(cmd_num)
	{
		case CMD_CHK_CONN:
			uart_packet_Identify(cmd_num);
			break;
		case CMD_SET_TUNE:
			tuningA2V[info[0]] = info[1];
			uart_packet_Response(cmd_num, info[0], tuningA2V[info[0]]);
			break;

		case CMD_GET_TUNE:
			uart_packet_Response(cmd_num, info[0], tuningA2V[info[0]]);
			break;

		case CMD_REQ_DBG:
			uart_packet_Debug(cmd_num, tuningA2V, info[0], info[1]);
			break;

		case CMD_REC_TUNE:
			util_flash_tuning_write(tuningA2V + info[0], info[1]);
			uart_packet_Record(cmd_num, tuningA2V, info[0], info[1]);
			break;

		case CMD_REG_WRT:
			I2C1_set_byte(info[0], info[1]);
			uart_packet_Register(cmd_num, info[0], info[1]);
			break;

		case CMD_REC_WAVE:
			util_waveform_to_flash();
			uart_packet_Response(cmd_num, info[0], info[1]);
			break;
			
		case CMD_MAX:
			uart_packet_Error();
			break;
	}
}

void uart_packet_Decode(char *packet, uint8_t *tuningA2V)
{
	int i, cnt = 0;
	uint8_t info[2];
	int cmd_num = CMD_MAX;
	
	if(packet[cnt++] == '$') {
		for(i = 0; i < CMD_MAX; i++) {
			if(!strncmp(packet+cnt, cmd[i].cmd_name, CMD_LEN)) {
				cmd_num = i;
				break;
			}
		}
		cnt += CMD_LEN;
		
		info[0] = asc2hex(packet[cnt++]) * 16;
		info[0] += asc2hex(packet[cnt++]);
		info[1] = asc2hex(packet[cnt++]) * 16;
		info[1] += asc2hex(packet[cnt++]);	
		if(packet[cnt++] == '#') {
			uart_packet_Exec(cmd_num, tuningA2V, info);
		}
		
	} else {
		uart_packet_Error();
		
	}

	return;
}

