#include "uart_packet.h"

#include "util.h"
#include "dw7914.h"

CMD_LIST cmd[CMD_MAX] = {
	"SET",
	"GET",
	"DBG",
	"REC",
	"REG",
};

/**
 ** for using STM HAL Libraries
 **/
extern UART_HandleTypeDef huart2;

void uart_packet_Send(uint8_t *msg, int len)
{
	HAL_UART_Transmit_DMA(&huart2, msg, len);
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

void uart_packet_Respond(uint8_t loc, uint8_t data)
{
	static uint8_t res[6];

	res[0] = '#';
	res[1] = hex2asc((uint8_t)((loc >> 4)&0x0F));
	res[2] = hex2asc((uint8_t)(loc % 16));
	res[3] = hex2asc((uint8_t)((data >> 4)&0x0F));
	res[4] = hex2asc((uint8_t)(data % 16));
	res[5] = 0x00;
	
	uart_packet_Send(res, 6);
}

void uart_packet_debug(uint8_t *tuningA2V, uint8_t loc, uint8_t len)
{
	uint8_t* res;
	res = (uint8_t*)malloc(len+3);
	res[0] = '@';
	res[1] = loc;
	res[2] = len;
	for(int i = 0; i < len; i++) {
		res[i+3] = tuningA2V[i+loc];
	}
	uart_packet_Send(res, len+3);
	free(res);
}

void uart_packet_Exec(int cmd_num, uint8_t *tuningA2V, uint8_t loc, uint8_t val)
{
	switch(cmd_num)
	{
		case CMD_SET:
			tuningA2V[loc] = val;
			uart_packet_Respond(loc, tuningA2V[loc]);
			break;

		case CMD_GET:
			uart_packet_Respond(loc, tuningA2V[loc]);
			break;

		case CMD_DBG:
			uart_packet_debug(tuningA2V, loc, val);
			break;

		case CMD_REC:
			util_flash_write(tuningA2V + loc, val);
			break;

		case CMD_REG:
			dw7914_set_byte(loc, val);
			//uart_packet_Respond(loc, val);
			break;
			
		case CMD_MAX:
			uart_packet_Error();
			break;
	}
}

void uart_packet_Decode(char *packet, uint8_t *tuningA2V)
{
	int i, cnt = 0;
	uint8_t loc, val;
	int cmd_num = CMD_MAX;
	
	if(packet[cnt++] == '$') {
		for(i = 0; i < CMD_MAX; i++) {
			if(!strncmp(packet+cnt, cmd[i].cmd_name, CMD_LEN)) {
				cmd_num = i;
				break;
			}
		}		
		cnt += CMD_LEN;
		
		loc = asc2hex(packet[cnt++]) * 16;
		loc += asc2hex(packet[cnt++]);
		val = asc2hex(packet[cnt++]) * 16;
		val += asc2hex(packet[cnt++]);
		if(packet[cnt++] == '#') {
			uart_packet_Exec(cmd_num, tuningA2V, loc, val);
		}
		
	} else {
		uart_packet_Error();
		
	}

	return;
}

