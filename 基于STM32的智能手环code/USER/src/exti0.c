#include "exti0.h"
#include "control.h"
static EXTI_InitTypeDef   EXTI_InitStructure;
static GPIO_InitTypeDef   GPIO_InitStructure;
static NVIC_InitTypeDef   NVIC_InitStructure;

void exti0_init(void)
{
	//使能端口A的硬件时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	 //使能系统配置时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//配置PA0的引脚为输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //第0根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;       //输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //高速响应
	//GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      //推挽输出，增加输出电流能力
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;        //使能下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);               //完成GPIO_Pin_0初始化
	
	//将PA0和EXTI0连接在一起
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	//外部中断配置
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;              //外部中断0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能
	EXTI_Init(&EXTI_InitStructure);
	
	 /* Enable and set EXTI Line0 Interrupt to the lowest priority */
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;          //外部中断0的请求中断
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; //抢占优先级
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;      //响应优先级
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能该通道
	 NVIC_Init(&NVIC_InitStructure);
		
}

//外部中断0服务函数，改变key_enter值
void EXTI0_IRQHandler(void)
{
	//判断是否有中断请求
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		key_enter = !key_enter;  //一开始为０
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	
}
