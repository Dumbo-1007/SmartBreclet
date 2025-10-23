#include "exti0.h"
#include "control.h"
static EXTI_InitTypeDef   EXTI_InitStructure;
static GPIO_InitTypeDef   GPIO_InitStructure;
static NVIC_InitTypeDef   NVIC_InitStructure;

void exti0_init(void)
{
	//ʹ�ܶ˿�A��Ӳ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 //ʹ��ϵͳ����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//����PA0������Ϊ����ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //��0������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //������Ӧ
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      //������������������������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //ʹ������
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //���GPIO_Pin_0��ʼ��
	
	//��PA0��EXTI0������һ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	//�ⲿ�ж�����
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;              //�ⲿ�ж�0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	 /* Enable and set EXTI Line0 Interrupt to the lowest priority */
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          //�ⲿ�ж�0�������ж�
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //��ռ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;      //��Ӧ���ȼ�
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //ʹ�ܸ�ͨ��
	 NVIC_Init(&NVIC_InitStructure);
		
}

//�ⲿ�ж�0���������ı�key_enterֵ
void EXTI0_IRQHandler(void)
{
	//�ж��Ƿ����ж�����
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		key_enter = !key_enter;  //һ��ʼΪ��
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}
