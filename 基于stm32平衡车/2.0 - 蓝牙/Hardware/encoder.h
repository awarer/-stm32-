#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "sys.h" 
void Encoder_TIM2_Init(void);

void Encoder_TIM3_Init(void);

int Read_Speed(int TIMx);

void TIM2_IRQHandler(void);

void TIM3_IRQHandler(void);

#endif

