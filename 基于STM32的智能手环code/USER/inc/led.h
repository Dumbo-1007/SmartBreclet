#ifndef	_LED_H
#define _LED_H

#include "stm32f4xx.h"
void LED_init(void);
#define LED_ON		GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LED_OFF		GPIO_SetBits(GPIOA,GPIO_Pin_7)	

#endif

