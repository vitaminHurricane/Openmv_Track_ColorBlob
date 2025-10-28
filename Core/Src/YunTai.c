#include "main.h"
#include "YunTai.h"
#include "tim.h"
#include "Delay.h"

float cur_angle_up = 0, cur_angle_down = 0;
float aim_up = 0, aim_down = 0;
float time_up = 0, time_down = 0;
float change_up = 0, change_down = 0;

float __DuoJi_AngleLimit(DuoJi direct, float angle)         //舵机角度限制
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

float __DuoJi_GetPWM_Up(float angle)            //单独设置上舵机的角度
{
    angle = __DuoJi_AngleLimit(UP, angle);
    float pwm;    //计算角度对应的pwm占空比
    pwm = (angle - MIN_Angle_Up) * ((MAX_PWM_Up - MIN_PWM_Up) / (float)(MAX_Angle_Up - MIN_Angle_Up)) + MIN_PWM_Up;
    return pwm;
}

float __DuoJi_GetPWM_Down(float angle)          //单独设置下舵机的角度
{
    angle = __DuoJi_AngleLimit(DOWN, angle);
    float pwm;    //计算角度对应的pwm占空比
    pwm = (angle - MIN_Angle_Down) * ((MAX_PWM_Down - MIN_PWM_Down) / (float)(MAX_Angle_Down - MIN_Angle_Down)) + MIN_PWM_Down;
    return pwm;
}

void DuoJi_SetAngle(DuoJi direct, float angle)      //设置云台舵机角度
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

void DuoJi_Init(void)           //云台初始化
{
    DuoJi_SetAngle(DOWN, 0);
    DuoJi_SetAngle(UP, -25);
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
    if (time_ms < 10) {
        time_ms = 10;
    }
    if (direct == UP) {
        aim_up = aim;
        time_up = time_ms;
        change_up = (aim_up - cur_angle_up) / (time_up / 10);       //计算每次累加的角度
    } else {
        aim_down = aim;
        time_down = time_ms;
        change_down = (aim_down - cur_angle_down) / (time_down / 10);       //计算每次累加的角度
    }
}

void __DuoJi_SetSpeed_Up(int8_t speed)
{
    if (speed > 100) {
        speed = 100;
    } else if (speed < -100) {
        speed = -100;
    }
    if (cur_angle_up < MAX_Angle_Up && cur_angle_up > MIN_Angle_Up) {
        change_up = 0.05 * speed;
    } else {
        change_up = 0;
    }
}

void __DuoJi_SetSpeed_Down(int8_t speed)
{
    if (speed > 100) {
        speed = 100;
    } else if (speed < -100) {
        speed = -100;
    } 
    if (cur_angle_down < MAX_Angle_Down && cur_angle_down > MIN_Angle_Down) {
        change_down = 0.05 * speed;
    } else {
        change_down = 0;
    }
}

//舵机速度控制，设置一个速度，使舵机保持该速度一直运行
void DuoJi_SetSpeed(DuoJi direct, int8_t speed)
{
    if (direct == UP) {
        __DuoJi_SetSpeed_Up(speed);
    } else {
        __DuoJi_SetSpeed_Down(speed);
    }
}


