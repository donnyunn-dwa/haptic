/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_PACKET_H
#define __UART_PACKET_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "main.h"

#define UART_BUFF_SIZE 10

/* ${CMD}{INFO1}{INFO2}# */
#define CMD_LEN 4
#define INFO_LEN 2

typedef enum {
	CMD_CHK_CONN = 0,
	CMD_SET_TUNE,
	CMD_GET_TUNE,
	CMD_REQ_DBG,
	CMD_REC_TUNE,
	CMD_REG_WRT,
	CMD_REC_WAVE,
	CMD_MAX
} CMD_NUM;

typedef struct {
	char cmd_name[CMD_LEN];
} CMD_LIST;

void uart_packet_Decode(char *packet, uint8_t *tuningA2V);

#ifdef __cplusplus
	}
#endif
#endif /* __UART_PACKET_H */

