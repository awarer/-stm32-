#include "stm32f10x.h"                  // Device header

#include "pwm.h"
#include "sys.h" 

void PWM_Init_TIM1(uint16_t Psc, uint16_t Per)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	TIM_OCInitTypeDef TIM_OCInitStrycture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);//开启TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启AFIO时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;//初始化pa8和pa11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//初始化定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = Per;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = Psc;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStrycture.TIM_OCMode = TIM_OCMode_PWM1;//初始化输出比较
	TIM_OCInitStrycture.TIM_OCPolarity = TIM_OCPolarity_High;//上升沿
	TIM_OCInitStrycture.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStrycture.TIM_Pulse = 0;//CCR
	TIM_OC1Init(TIM1,&TIM_OCInitStrycture);
	TIM_OC4Init(TIM1,&TIM_OCInitStrycture);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//高级定时器专属的MOE主输出使能
	
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); //OC1预装载寄存器使能
	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable); //OC4预装载寄存器使能
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);//TIM1在ARR上预装载寄存器使能
	
	
	TIM_Cmd(TIM1,ENABLE);//开定时器1。	
	
}