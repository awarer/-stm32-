#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "usart3.h"


void uart3_init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//开启串口3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启时钟GPIOB
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//USART初始化
	USART_InitStructure.USART_BaudRate = bound;//波特率9600，通过入口参数获得
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口
}


unsigned char Fore,Back,Left,Right;
void USART3_IRQHandler(void) 
{
	int Bluetooth_data;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//接收中断标志位拉高
	{
		Bluetooth_data=USART_ReceiveData(USART3);//保存接收的数据
		switch (Bluetooth_data)
		{
			case 0x00://停止 Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
			case 0x01://前进 A
				Fore=1,Back=0,Left=0,Right=0;
				break;
			case 0x02://左前 H
				Fore=1,Back=0,Left=1,Right=0;
				break;
			case 0x03://右前 B
				Fore=1,Back=0,Left=0,Right=1;
				break;
			case 0x04://左转 G
				Fore=0,Back=0,Left=1,Right=0;
				break;
			case 0x05://右转 C
				Fore=0,Back=0,Left=0,Right=1;
				break;
			case 0x06://后退 E
				Fore=0,Back=1,Left=0,Right=0;
				break;
			case 0x07://左后,向右旋 F
				Fore=0,Back=1,Left=0,Right=1;
				break;
			case 0x08://右后，向左旋 D
				Fore=0,Back=1,Left=1,Right=0;
				break;
			default://停止 Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
		}
//		if(Bluetooth_data==90)
//		{
//			Fore=0,Back=0,Left=0,Right=0;//刹
//		}
//		else if(Bluetooth_data==65)
//		{
//			Fore=1,Back=0,Left=0,Right=0;//前
//		}
//		else if(Bluetooth_data==69)
//		{
//			Fore=0,Back=1,Left=0,Right=0;//后
//		}
//		else if(Bluetooth_data==71)
//		{
//			Fore=0,Back=0,Left=1,Right=0;//左
//		}
//		else if(Bluetooth_data==0x07)
//		{
//			Fore=0,Back=0,Left=0,Right=1;//右
//		}
//		else 
//		{
//			Fore=0,Back=0,Left=0,Right=0;//刹
//		}
	}
}


//一个字节的数据
void USART3_Send_Data(char data)
{
	USART_SendData(USART3,data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1)
	{
		
	}
}

//一个字符串
void USART3_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART3_Send_Data(*String++);
	}
}


