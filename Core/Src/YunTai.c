#include "main.h"
#include "YunTai.h"
#include "tim.h"
#include "Delay.h"

float cur_angle_up = 0, cur_angle_down = 0;
float aim_up = 0, aim_down = 0;
float time_up = 0, time_down = 0;
float change_up = 0, change_down = 0;

float __DuoJi_AngleLimit(DuoJi direct, float angle)
{
    if (direct == UP) {
        if (angle <= MIN_Angle_Up) {
            angle = MIN_Angle_Up;
        } else if (angle >= MAX_Angle_Up) {
            angle = MAX_Angle_Up;
        }
    } else {
        if (angle <= MIN_Angle_Down) {
            angle = MIN_Angle_Down;
        } else if (angle >= MAX_Angle_Down) {
            angle = MAX_Angle_Down;
        }
    }
    return angle;
}

float __DuoJi_GetPWM_Up(float angle)
{
    angle = __DuoJi_AngleLimit(UP, angle);
    float pwm;
    pwm = (angle - MIN_Angle_Up) * ((MAX_PWM_Up - MIN_PWM_Up) / (float)(MAX_Angle_Up - MIN_Angle_Up)) + MIN_PWM_Up;
    return pwm;
}

float __DuoJi_GetPWM_Down(float angle)
{
    angle = __DuoJi_AngleLimit(DOWN, angle);
    float pwm;
    pwm = (angle - MIN_Angle_Down) * ((MAX_PWM_Down - MIN_PWM_Down) / (float)(MAX_Angle_Down - MIN_Angle_Down)) + MIN_PWM_Down;
    return pwm;
}

void DuoJi_SetAngle(DuoJi direct, float angle)
{
    if (direct == UP) {
        uint16_t pwm = __DuoJi_GetPWM_Up(angle);
        Write_Up_PWM(pwm);
        cur_angle_up = angle;
    } else {
        uint16_t pwm = __DuoJi_GetPWM_Down(angle);
        Write_Down_PWM(pwm);
        cur_angle_down = angle;
    }
}

void DuoJi_Init(void)
{
    DuoJi_SetAngle(DOWN, 0);
    DuoJi_SetAngle(UP, -30);
    Delay_ms(1000);
}

void DuoJi_Control_T(DuoJi direct, float aim, uint16_t time_ms)           //阻塞式控速，用于测试舵机
{
    if (direct == UP) {
        float distance = aim - cur_angle_up;
        float change = distance / (time_ms / 10);
        if (change > 3) {                                   //每十毫秒角度变化超过3°视为快速，直接到位
            DuoJi_SetAngle(UP, aim);
        } else {
            for (uint16_t cnt = 0; cnt < time_ms / 10; cnt++) {
                cur_angle_up += change;
                DuoJi_SetAngle(UP, cur_angle_up);
                Delay_ms(10);
            }
        }
    } else {
        float distance = aim - cur_angle_down;              //distance of angle
        float change = distance / (time_ms / 10);
        if (change > 3) {                                   //每十毫秒角度变化超过3°视为快速，直接到位
            DuoJi_SetAngle(DOWN, aim);
        } else {
            for (uint16_t cnt = 0; cnt < time_ms / 10; cnt++) {
                cur_angle_down += change;
                DuoJi_SetAngle(DOWN, cur_angle_down);
                Delay_ms(10);
            }
        }
    }
}

//专门设置一个定时器，在指定时间间隔之后微量改变舵机角度，达到控速目的
//这里是目标设置函数
void DuoJi_Control(DuoJi direct, float aim, uint16_t time_ms)
{
    if (direct == UP) {
        aim_up = aim;
        time_up = time_ms;
        change_up = (aim_up - cur_angle_up) / (time_up / 10);
    } else {
        aim_down = aim;
        time_down = time_ms;
        change_down = (aim_down - cur_angle_down) / (time_down / 10);
    }
}
