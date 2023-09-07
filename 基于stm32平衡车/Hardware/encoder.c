#include "stm32f10x.h"                  // Device header
#include "encoder.h"

void Encoder_TIM2_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//开启TIM2定时器
	
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0 |GPIO_Pin_1;//初始化pa0和pa1，为TIM2编码器的接口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//初始化定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ICStructInit(&TIM_ICInitStructure);//初始化输入捕获
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//输入比较滤波器，最大值
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置编码器模式，上升沿和下降沿都计数，增加采样率，不反向
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//清除TIM2的上升沿待处理标志位
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	
	TIM_SetCounter(TIM2,0);//清零定时器计数值
	
	TIM_Cmd(TIM2,ENABLE);//开启定时器
	
}


void Encoder_TIM3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启时钟
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//开启TIM3定时器

	
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6 |GPIO_Pin_7;//初始化pa6和pa7，为TIM3编码器的接口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);//初始化定时器
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;//不分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ICStructInit(&TIM_ICInitStructure);//初始化输入捕获
	TIM_ICInitStructure.TIM_ICFilter = 0xF;//输入比较滤波器，最大值
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//配置编码器模式，上升沿和下降沿都计数，增加采样率，不反向
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//清除TIM3的上升沿待处理标志位
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//配置溢出更新中断标志位
	
	TIM_SetCounter(TIM3,0);//清零定时器计数值
	
	TIM_Cmd(TIM3,ENABLE);//开启定时器
	
}


int Read_Speed(int TIMx)//速度采集函数
{
	int value_1;
	switch(TIMx)
	{
		case 2: value_1 = (short)TIM_GetCounter(TIM2);
				TIM_SetCounter(TIM2,0);//先把定时器2的值传给value_1，后清零
			break;
		case 3: value_1 = (short)TIM_GetCounter(TIM3);
				TIM_SetCounter(TIM3,0);//先把定时器3的值传给value_1，后清零
			break;
		default:value_1 = 0;
	}
	return value_1;
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != 0)//如果中断标志位不等于0，说明进入中断
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断标志位
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=0)//如果中断标志位不等于0，说明进入中断
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除中断标志位
	}
}

