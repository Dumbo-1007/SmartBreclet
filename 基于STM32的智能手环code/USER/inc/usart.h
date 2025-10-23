#ifndef USART_H
#define USART_H

#include "stm32f4xx.h"
typedef struct
{
	u8 rec_buf[512];
	u8 len;
	u8 rec_over;
}RECEIVE_BUF;
extern RECEIVE_BUF receive_buff;
void Usart1_Init(u32 baud);

#endif

