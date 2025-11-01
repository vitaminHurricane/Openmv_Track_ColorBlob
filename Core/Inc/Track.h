#ifndef __TRACK_H
#define __TRACK_H

#define MID_W       159     //设置宽的中点
#define MID_H       119     //设置高的中点

extern float target_x, target_y;

void Track_Update_Target(void);
void Track_Blob_Grad(uint16_t x, uint16_t y);
float Track_Blob_PID_X(void);
float Track_Blob_PID_Y(void);

#endif
