#ifndef __YUTAI_H
#define __YUTAI_H
#include "main.h"

/*
 * 舵机PWM信号要求周期为：20ms，频率为50Hz
 * 此二位自由度云台舵机为270°舵机
 * timer8 作为云台两个舵机的pwm输出定时器：
 * period为20000 - 1
 * prescaler为168 - 1，counter每隔1us累加一次
 */

#define MAX_Angle_Down          135
#define MID_Angle_Down          0
#define MIN_Angle_Down          -135
#define MAX_PWM_Down            2500               
#define MID_PWM_Down            1490        //->0°
#define MIN_PWM_Down            500

#define MAX_Angle_Up            90
#define MID_Angle_Up            0
#define MIN_Angle_Up            -90
#define MAX_PWM_Up              2195
#define MID_PWM_Up              1530        //->0°
#define MIN_PWM_Up              835

#define DuoJi_Down_TIM          htim8       
#define DuoJi_Down_Channel      TIM_CHANNEL_3
#define DuoJi_Up_TIM            htim8
#define DuoJi_Up_Channel        TIM_CHANNEL_4

#define Write_Up_PWM(x)         __HAL_TIM_SetCompare(&DuoJi_Up_TIM, DuoJi_Up_Channel, (uint16_t)x)
#define Write_Down_PWM(x)       __HAL_TIM_SetCompare(&DuoJi_Down_TIM, DuoJi_Down_Channel, (uint16_t)x)              

typedef enum{
    UP,
    DOWN,
}DuoJi;

extern float cur_angle_up, cur_angle_down;
extern float aim_up, aim_down;
extern float time_up, time_down;
extern float change_up, change_down;

/*********************************private*************************************/
float __DuoJi_AngleLimit(DuoJi direct, float angle);
float __DuoJi_GetPWM_Up(float angle);
float __DuoJi_GetPWM_Down(float angle);
/*****************************************************************************/


/**********************************public*************************************/
void DuoJi_Init(void);
void DuoJi_SetAngle(DuoJi direct, float angle);
void DuoJi_Control_T(DuoJi direct, float aim, uint16_t time_ms);
void DuoJi_Control(DuoJi direct, float aim, uint16_t time_ms);
/*****************************************************************************/


#endif
