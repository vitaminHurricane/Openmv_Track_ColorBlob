#ifndef __CONNECT_H
#define __CONNECT_H
#include "usart.h"

/**
 * 选择huart2作为与openmv通信的串口
 * TX->PD5
 * RX->PD6
 * 波特率->115200
 */

#define CONNECT_UART        huart2
#define MAX                 9

extern uint8_t rx_buf[];
extern uint8_t result[];

void Connect_Openmv_Start(void);
void Connect_Trans(void);

#endif
