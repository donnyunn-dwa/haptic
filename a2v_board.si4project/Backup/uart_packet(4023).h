/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_PACKET_H
#define __UART_PACKET_H
#ifdef __cplusplus
	extern "C" {
#endif

#include "main.h"

#define UART_BUFF_SIZE 9

/* ${CMD}{REG}{VAL}# */
#define CMD_LEN 3
#define REG_LEN 2
#define VAL_LEN 2

typedef enum {
	CMD_SET = 0,
	CMD_GET,
	CMD_DBG,
	CMD_REC,
	CMD_REG,
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

