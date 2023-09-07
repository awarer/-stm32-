#include "stm32f10x.h"                  // Device header
#include "sys.h"
#include "usart3.h"


void uart3_init(u32 bound)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//��������3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//����ʱ��GPIOB
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//USART��ʼ��
	USART_InitStructure.USART_BaudRate = bound;//������9600��ͨ����ڲ������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ�����ݿ�����
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
}


unsigned char Fore,Back,Left,Right;
void USART3_IRQHandler(void) 
{
	int Bluetooth_data;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
	{
		Bluetooth_data=USART_ReceiveData(USART3);//������յ�����
		switch (Bluetooth_data)
		{
			case 0x00://ֹͣ Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
			case 0x01://ǰ�� A
				Fore=1,Back=0,Left=0,Right=0;
				break;
			case 0x02://��ǰ H
				Fore=1,Back=0,Left=1,Right=0;
				break;
			case 0x03://��ǰ B
				Fore=1,Back=0,Left=0,Right=1;
				break;
			case 0x04://��ת G
				Fore=0,Back=0,Left=1,Right=0;
				break;
			case 0x05://��ת C
				Fore=0,Back=0,Left=0,Right=1;
				break;
			case 0x06://���� E
				Fore=0,Back=1,Left=0,Right=0;
				break;
			case 0x07://���,������ F
				Fore=0,Back=1,Left=0,Right=1;
				break;
			case 0x08://�Һ������� D
				Fore=0,Back=1,Left=1,Right=0;
				break;
			default://ֹͣ Z
				Fore=0,Back=0,Left=0,Right=0;
				break;
		}
//		if(Bluetooth_data==90)
//		{
//			Fore=0,Back=0,Left=0,Right=0;//ɲ
//		}
//		else if(Bluetooth_data==65)
//		{
//			Fore=1,Back=0,Left=0,Right=0;//ǰ
//		}
//		else if(Bluetooth_data==69)
//		{
//			Fore=0,Back=1,Left=0,Right=0;//��
//		}
//		else if(Bluetooth_data==71)
//		{
//			Fore=0,Back=0,Left=1,Right=0;//��
//		}
//		else if(Bluetooth_data==0x07)
//		{
//			Fore=0,Back=0,Left=0,Right=1;//��
//		}
//		else 
//		{
//			Fore=0,Back=0,Left=0,Right=0;//ɲ
//		}
	}
}


//һ���ֽڵ�����
void USART3_Send_Data(char data)
{
	USART_SendData(USART3,data);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1)
	{
		
	}
}

//һ���ַ���
void USART3_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART3_Send_Data(*String++);
	}
}


