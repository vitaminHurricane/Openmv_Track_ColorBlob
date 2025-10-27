#ifndef __VOFA_H
#define __VOFA_H
#include "usart.h"

/*
 *  注意：串口默认波特率为115200 
 *  FireWater协议以\r\n为结尾，数据间以逗号（英文）分开
 */

#define VOFA_UART      huart1          //此处填写vofa使用的川口句柄

#endif
