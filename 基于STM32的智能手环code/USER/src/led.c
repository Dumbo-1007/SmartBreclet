#include "led.h"

//LED--PA7
void LED_init(void)
{
	GPIO_InitTypeDef gpio_initStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	gpio_initStruct.GPIO_Mode = GPIO_Mode_OUT ;
	gpio_initStruct.GPIO_OType = GPIO_OType_PP ;
	gpio_initStruct.GPIO_Pin = GPIO_Pin_7 ;
	gpio_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	gpio_initStruct.GPIO_Speed = GPIO_Fast_Speed ;
	
	GPIO_Init(GPIOA,&gpio_initStruct);
	LED_OFF;
}

