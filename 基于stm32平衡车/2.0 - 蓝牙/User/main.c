#include "stm32f10x.h"                  // Device header


#include "sys.h" 
#include "DataScope_DP.h"

int MOTO1,MOTO2;//电机装载变量



int Encoder_Left,Encoder_Right;//编码器速度

int PWM_MAX = 7200;//pwm限幅
int PWM_MIN = -7200;

float Pitch,Roll,Yaw;//俯仰角，横滚角，偏航角
short gyrox,gyroy,gyroz;//陀螺仪，角速度
short aacx,aacy,aacz;//加速度

unsigned char i;          										 
unsigned char Send_Count; 	

int main(void)
{
	delay_init();
	
	NVIC_Config();
	
	uart1_init(128000);
	
	uart3_init(9600);
	USART3_Send_String("AT+NAME My Car\r\n");//改蓝牙的名字
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();
	
	MPU6050_EXTI_Init();
	
	Encoder_TIM2_Init();
	Encoder_TIM3_Init();
	
	Motor_Init();

	
	PWM_Init_TIM1(1-1,7200-1);

	//OLED_ShowNumber(1,1,MPU_DEVICE_ID_REG,2,12);
	
	while(1)
	{
		//MPU_Get_Gyroscope(&GX,&GY,&GZ);
		//MPU_Get_Accelerometer(&ax,&ay,&az);
		//mpu_dmp_get_data(&Pitch,&Roll,&Yaw);
		OLED_Float(0,10,Pitch,3);
		OLED_Float(2,10,Roll,3);
		OLED_Float(4,10,Encoder_Left,3);
		OLED_Float(6,10,Encoder_Right,3);
		DataScope_Get_Channel_Data(Pitch, 1 );//通道 1
		DataScope_Get_Channel_Data(Roll, 2 );//通道 2
		Send_Count=DataScope_Data_Generate(2);//发送 2 个通道
		for( i = 0 ; i < Send_Count; i++)
		{
			while((USART1->SR&0X40)==0);
			USART1->DR = DataScope_OutPut_Buffer[i];
		}
	}
}

