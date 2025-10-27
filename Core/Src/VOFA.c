#include "main.h"
#include <stdio.h>
#include "VOFA.h"

int fputc(int ch, FILE *f)          //重定向串口打印
{
    HAL_UART_Transmit(&VOFA_UART, (uint8_t *)&ch, 1, 50);
    return ch;
}

