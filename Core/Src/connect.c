#include "main.h"
#include "connect.h"
#include "usart.h"

uint8_t rx_buf[MAX] = {0};
uint8_t result[3] = {0};

void __Connect_Clear_RX(void)
{
    for (uint8_t i = 0; i < MAX; i++) {
        rx_buf[i] = '\0';
    }
}

void Connect_Openmv_Start(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&CONNECT_UART, rx_buf, MAX);
}

void Connect_Trans(void)
{
    result[0] = rx_buf[2] - '0' + (rx_buf[1] - '0') * 10 + (rx_buf[0] - '0') * 100;
    result[1] = rx_buf[5] - '0' + (rx_buf[4] - '0') * 10 + (rx_buf[3] - '0') * 100;
    result[2] = rx_buf[8] - '0' + (rx_buf[7] - '0') * 10 + (rx_buf[6] - '0') * 100;
}








