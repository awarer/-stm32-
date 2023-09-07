#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h" 

#define Ain1 PBout(13)
#define Ain2 PBout(12)

#define Bin1 PBout(14)
#define Bin2 PBout(15)

void Motor_Init(void);
void Limit(int *motoA, int *motoB);
int GFP_abs(int p);
void Load(int moto1,int moto2);
void Stop(float *Med_Jiaodu,float *Jiaodu);

#endif

