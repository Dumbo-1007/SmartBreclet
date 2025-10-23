#include "tim5.h"
#include "adc.h"
#include "tim2.h"
#include "oled.h"
#include "includes.h"
void tim5_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能定时器5硬件时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	//配置定时器5分频值、计数值等
   TIM_TimeBaseStructure.TIM_Period = 50*1000-1;     //计数值，0~9999，决定定时时间为50ms
   TIM_TimeBaseStructure.TIM_Prescaler = 100-1;   //默认TIM_Prescaler+1，8400预分频值,定时5的时钟频率为100MHz
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //在F407是不支持时钟分频的
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数
   TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	//配置定时器3中断的触发方式：时间更新
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	//配置定时器3的中断优先级
   NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

	///使能定时器5工作
	TIM_Cmd(TIM5, ENABLE);

}
	
extern int key_ok;
//定时器5的中断服务函数
void TIM5_IRQHandler(void)
{
	//判断标志位，未按下确认键时进入
	if(TIM_GetITStatus(TIM5,TIM_IT_Update)==SET && key_enter==0)
	{
		if(LEFT)
		{
			Delay_ms(10);
			if(LEFT)
			{
				while(ADC_value>5);
				key_ok--;
				if(key_ok < 0) key_ok = 5;
			}
		}
		else if(RIGHT)
		{
			Delay_ms(10);
			if(RIGHT)
			{
				while(ADC_value>5);
				key_ok++;
				if(key_ok > 5) 
				{
					key_ok = 0;
				}
			}
		}
		else if(UP)
		{
			Delay_ms(10);
			if(UP)
			while(ADC_value>5);
			key_ok = 0;
		}
		else if(DOWN)
		{
			Delay_ms(10);
			if(DOWN)
			while(ADC_value>5);
			key_ok =  6;
		}
	}
	//清空标志位
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}



////TIM5--100MHZ
//void TIM5_delayms(u16 nms)
//{
//	TIM_TimeBaseInitTypeDef  TIM5_TimeBaseInitStruct;
//	//1. 打开TIM5时钟
//	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
//	TIM_SetCounter(TIM5,0);
//	
//	//2. TIM5初始化
//	TIM5_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM5_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM5_TimeBaseInitStruct.TIM_Period = nms*1000-1;  
//	TIM5_TimeBaseInitStruct.TIM_Prescaler = 100-1;  //100 分频
//	TIM5_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	
//	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseInitStruct);

//	//3. 使能TIM5
//	 TIM_Cmd(TIM5, ENABLE);

//	//4. 先把标志位清零
//	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//	
//	//5. 等待计数时间到
//	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==RESET)
//	{
//		//printf("6\n");
//	}

//	//6. 关闭定时器
//	 TIM_Cmd(TIM5, DISABLE);
//}


////TIM5--100MHZ
//void TIM5_delayus(u16 nus)
//{
//	TIM_TimeBaseInitTypeDef  TIM5_TimeBaseInitStruct;
//	//1. 打开TIM5时钟
//	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
//	TIM_SetCounter(TIM5,0);
//	
//	//2. TIM5初始化
//	TIM5_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM5_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM5_TimeBaseInitStruct.TIM_Period = nus*1-1;  
//	TIM5_TimeBaseInitStruct.TIM_Prescaler = 100-1;  //100 分频
//	TIM5_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	
//	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseInitStruct);

//	//3. 使能TIM5
//	 TIM_Cmd(TIM5, ENABLE);

//	//4. 先把标志位清零
//	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//	
//	//5. 等待计数时间到
//	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==RESET)
//	{
//		//printf("6\n");
//	}

//	//6. 关闭定时器
//	 TIM_Cmd(TIM5, DISABLE);
//}


