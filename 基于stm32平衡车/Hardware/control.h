#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "sys.h" 

void EXTI9_5_IRQHandler(void);
int Vertical(float Med, float Angle, float Gyro_y); //直立环函数，期望角度，真实角度，y轴角速度
int Velocity(int Target,int encoder_left,int encoder_right); //速度环函数，期望速度，左电机速度，右电机速度
int Turn(int gyro_z, int RC);

#endif

