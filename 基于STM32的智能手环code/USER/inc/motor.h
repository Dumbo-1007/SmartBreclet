#ifndef MOTOR_H_
#define MOTOR_H_

#include "stm32f4xx.h"

#define MOTOR_ON   GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define MOTOR_OFF  GPIO_ResetBits(GPIOB, GPIO_Pin_10)
void Motor_Init(void);

#endif
