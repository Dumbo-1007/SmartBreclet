#include "usart.h"
#include "stdio.h"
void Usart1_Init(u32 baud)
{
	GPIO_InitTypeDef gpio_initStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	gpio_initStruct.GPIO_Mode = GPIO_Mode_AF ;
	gpio_initStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	gpio_initStruct.GPIO_Speed = GPIO_Fast_Speed ;
	GPIO_Init(GPIOA,&gpio_initStruct);
	
	GPIO_PinAFConfig(GPIOA, 9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, 10, GPIO_AF_USART1);


	USART_InitTypeDef  USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStruct.USART_BaudRate = baud;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStruct); 
	
	//设置中断
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,2,3));
	USART_ITConfig(USART1, USART_IT_RXNE | USART_IT_IDLE, ENABLE);  
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_Cmd(USART1, ENABLE); 

}

RECEIVE_BUF receive_buff = {0};
//接收中断服务函数
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		USART_ClearFlag(USART1, USART_IT_RXNE);//清除接收中断标志
		receive_buff.rec_buf[receive_buff.len++] = USART1->DR;	
	}
 	if(USART_GetITStatus(USART1,USART_IT_IDLE) == RESET)
	{
		//清标志
		if( USART1->DR )
		{
			;    //清空闲标志
		}
		receive_buff.rec_buf[receive_buff.len] = '\0';
		receive_buff.len = 0;
		receive_buff.rec_over = 1;
		printf("%s", receive_buff.rec_buf);
	}
}


#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}
