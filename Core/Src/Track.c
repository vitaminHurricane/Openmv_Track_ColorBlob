#include "main.h"
#include "YunTai.h"
#include "Track.h"
#include "connect.h"
#include <stdio.h>

float P_x = 0.124, I_x = 0.04, D_x = 0.15;
float P_y = 0.1, I_y = 0.03, D_y = 0.13;
float err_cur_x = 0, err_pre_x = 0, err_int_x = 0;
float err_cur_y = 0, err_pre_y = 0, err_int_y = 0;
float target_x = MID_W, target_y = MID_H;

void Track_Blob_Grad(uint16_t x, uint16_t y)
{
    int8_t err_x = MID_W - x;       //x轴，水平方向上的偏差
    int8_t err_y = MID_H - y;       //y轴，竖直方向上的偏差
    printf("%d\r\n", err_x);
    if (err_x > 0) {                //err_x大于0时，舵机向左移动
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
    } else if (err_x < 0) {         //err_x小于0时，舵机向右移动
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

float Track_Blob_PID_X(void)
{
    err_pre_x = err_cur_x;                      //当err_cur_x大于0时，说明目标在右边，setangle为负时，舵机右转
    err_cur_x = target_x - MID_W;               //当err_cur_x小于0时，说明目标在左边，setangle为正时，舵机左转
    
    if (err_cur_x >= -2 && err_cur_x <= 2) {
        //err_cur_x = 0;
    } else {
        err_int_x += err_cur_x;         
    }

    float result = -P_x * err_cur_x -I_x * err_int_x -D_x * (err_cur_x - err_pre_x);

    return result;
}

//当上舵机设置角度为负数时，向上抬头；为正数时向下低头
float Track_Blob_PID_Y(void)
{
    err_pre_y = err_cur_y;
    err_cur_y = target_y - MID_H;

    if (err_cur_y >= -2 && err_cur_y <= 2) {

    } else {
        err_int_y += err_cur_y;
    }

    float result = P_y * err_cur_y + I_y * err_int_y + D_y * (err_cur_y - err_pre_y);

    return result;
}

