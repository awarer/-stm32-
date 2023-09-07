#include "stm32f10x.h"                  // Device header
#include "exti.h"
#include "sys.h" 

void MPU6050_EXTI_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//开启时钟
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;//MPU6050 INT引脚为pb5，进入第五条中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断触发
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);//选用PB5作为外部中断
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入,发生中断有下降沿动作，初始化要高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//初始化pb5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}