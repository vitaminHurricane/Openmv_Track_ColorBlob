#include "main.h"
#include "YunTai.h"
#include "Track.h"
#include "connect.h"
#include <stdio.h>

float P_x = 0, I_x = 0, D_x = 0;
float err_cur_x = 0, err_pre_x = 0, err_int_x = 0;
float target_x = MID_W, target_y = MID_H;

void Track_Blob_Grad(uint16_t x, uint16_t y)
{
    int8_t err_x = MID_W - x;       //x轴，水平方向上的偏差
    int8_t err_y = MID_H - y;       //y轴，竖直方向上的偏差
    printf("%d\r\n", err_x);
    if (err_x > 0) {                //err_x大于0时，舵机向右移动
        if (err_x < 5) {                
            DuoJi_SetSpeed(DOWN, 0);
        } else if (err_x >=5 && err_x < 20) {
            DuoJi_SetSpeed(DOWN, 3);
        } else if (err_x >= 20 && err_x < 40) {
            DuoJi_SetSpeed(DOWN, 12);
        } else if (err_x >= 40 && err_x < 60) {
            DuoJi_SetSpeed(DOWN, 19);
        } else if (err_x >= 60 && err_x < 80) {
            DuoJi_SetSpeed(DOWN, 30);
        } else {
            DuoJi_SetSpeed(DOWN, 50);
        }
    } else if (err_x < 0) {         //err_x小于0时，舵机向左移动
        if (err_x > -5) {
            DuoJi_SetSpeed(DOWN, 0);
        } else if (err_x <= -5 && err_x > -20) {
            DuoJi_SetSpeed(DOWN, -3);
        } else if (err_x <= -20 && err_x > -40) {
            DuoJi_SetSpeed(DOWN, -12);
        } else if (err_x <= -40 && err_x > -60) {
            DuoJi_SetSpeed(DOWN, -19);
        } else if (err_x <= -60 && err_x > -80) {
            DuoJi_SetSpeed(DOWN, -30);
        } else {
            DuoJi_SetSpeed(DOWN, -50);
        }
    }
}

void Track_Update_Target(void)
{
    target_x = result[0];
    target_y = result[1];
}

void Track_Blob_PID(uint16_t x, uint16_t y)
{
    err_pre_x = err_cur_x;
    err_cur_x = target_x - x;
    err_int_x += err_cur_x;
}
