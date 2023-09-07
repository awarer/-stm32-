#ifndef __USART3_H__
#define __USART3_H__
#include "sys.h" 


void uart3_init(u32 bound);
void USART3_IRQHandler(void) ;
void USART3_Send_String(char *String);
void USART3_Send_Data(char data);

#endif

