#include "tim2.h"

void Delay_ms(u16 ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd  (RCC_APB1Periph_TIM2, ENABLE); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = ms*1000 - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 100 -1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM2,ENABLE);
	TIM_ClearFlag (TIM2, TIM_FLAG_Update);
	while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);
	TIM_Cmd(TIM2,DISABLE);

}

void Delay_us(u16 us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd  (RCC_APB1Periph_TIM2, ENABLE); 
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = us - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 100 -1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);

	TIM_Cmd(TIM2,ENABLE);
	TIM_ClearFlag (TIM2, TIM_FLAG_Update);
	while(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == RESET);
	TIM_Cmd(TIM2,DISABLE);

}

