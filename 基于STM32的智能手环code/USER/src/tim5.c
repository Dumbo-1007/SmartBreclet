#include "tim5.h"
#include "adc.h"
#include "tim2.h"
#include "oled.h"
#include "includes.h"
void tim5_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ�ܶ�ʱ��5Ӳ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	//���ö�ʱ��5��Ƶֵ������ֵ��
   TIM_TimeBaseStructure.TIM_Period = 50*1000-1;     //����ֵ��0~9999��������ʱʱ��Ϊ50ms
   TIM_TimeBaseStructure.TIM_Prescaler = 100-1;   //Ĭ��TIM_Prescaler+1��8400Ԥ��Ƶֵ,��ʱ5��ʱ��Ƶ��Ϊ100MHz
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;            //��F407�ǲ�֧��ʱ�ӷ�Ƶ��
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���
   TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
	//���ö�ʱ��3�жϵĴ�����ʽ��ʱ�����
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	//���ö�ʱ��3���ж����ȼ�
   NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

	///ʹ�ܶ�ʱ��5����
	TIM_Cmd(TIM5, ENABLE);

}
	
extern int key_ok;
//��ʱ��5���жϷ�����
void TIM5_IRQHandler(void)
{
	//�жϱ�־λ��δ����ȷ�ϼ�ʱ����
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
	//��ձ�־λ
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}



////TIM5--100MHZ
//void TIM5_delayms(u16 nms)
//{
//	TIM_TimeBaseInitTypeDef  TIM5_TimeBaseInitStruct;
//	//1. ��TIM5ʱ��
//	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
//	TIM_SetCounter(TIM5,0);
//	
//	//2. TIM5��ʼ��
//	TIM5_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM5_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM5_TimeBaseInitStruct.TIM_Period = nms*1000-1;  
//	TIM5_TimeBaseInitStruct.TIM_Prescaler = 100-1;  //100 ��Ƶ
//	TIM5_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	
//	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseInitStruct);

//	//3. ʹ��TIM5
//	 TIM_Cmd(TIM5, ENABLE);

//	//4. �Ȱѱ�־λ����
//	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//	
//	//5. �ȴ�����ʱ�䵽
//	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==RESET)
//	{
//		//printf("6\n");
//	}

//	//6. �رն�ʱ��
//	 TIM_Cmd(TIM5, DISABLE);
//}


////TIM5--100MHZ
//void TIM5_delayus(u16 nus)
//{
//	TIM_TimeBaseInitTypeDef  TIM5_TimeBaseInitStruct;
//	//1. ��TIM5ʱ��
//	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
//	TIM_SetCounter(TIM5,0);
//	
//	//2. TIM5��ʼ��
//	TIM5_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM5_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM5_TimeBaseInitStruct.TIM_Period = nus*1-1;  
//	TIM5_TimeBaseInitStruct.TIM_Prescaler = 100-1;  //100 ��Ƶ
//	TIM5_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//	
//	TIM_TimeBaseInit(TIM5, &TIM5_TimeBaseInitStruct);

//	//3. ʹ��TIM5
//	 TIM_Cmd(TIM5, ENABLE);

//	//4. �Ȱѱ�־λ����
//	TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//	
//	//5. �ȴ�����ʱ�䵽
//	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==RESET)
//	{
//		//printf("6\n");
//	}

//	//6. �رն�ʱ��
//	 TIM_Cmd(TIM5, DISABLE);
//}


