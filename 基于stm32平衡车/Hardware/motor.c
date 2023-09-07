#include "stm32f10x.h"                  // Device header
#include "motor.h"
#include "sys.h" 

void Motor_Init(void)//电机初始化函数
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15;//初始化pb12,pb13,pb14,pb15
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}


void Limit(int *motoA, int *motoB)//限制电机的最大转数
{
	if(*motoA > PWM_MAX)
	{
		*motoA = PWM_MAX;
	}
	
	if(*motoA < PWM_MIN)
	{
		*motoA = PWM_MIN;
	}
	
	if(*motoB > PWM_MAX)
	{
		*motoB = PWM_MAX;
	}
	
	if(*motoB < PWM_MIN)
	{
		*motoB = PWM_MIN;
	}
}


int GFP_abs(int p)//取函数的绝对值
{
	int q;
	q = p >0?p:(-p);
	return q;
}


void Load(int moto1,int moto2)//赋值函数，pid运算完成后的最终pwm值
{
	if(moto1 > 0)
	{
		Ain1 = 1;
		Ain2 = 0;//正转
	}
	else
	{
		Ain1 = 0;
		Ain2 = 1;//反转
	}
	
	TIM_SetCompare1(TIM1,GFP_abs(moto1));//研究pwm值
	
	if(moto2 > 0)
	{
		Bin1 = 1;
		Bin2 = 0;//正转
	}
	else
	{
		Bin1 = 0;
		Bin2 = 1;//反转
	}
	
	TIM_SetCompare4(TIM1,GFP_abs(moto2));//研究pwm值
	
}


char PWM_Zero=0,stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu-*Med_Jiaodu)>65)//如果小车过于偏斜，则停止电机
	{
		Load(PWM_Zero,PWM_Zero);
		stop=1;
	}
}

