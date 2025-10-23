#ifndef __TIME5_H
#define __TIME5_H
#include "stm32f4xx.h"

void TIM5_delayms(u16 nms);
void TIM5_delayus(u16 nus);
void tim5_init(void);
#endif
