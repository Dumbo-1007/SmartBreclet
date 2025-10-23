#ifndef HP6_H_
#define HP6_H_
#include "stm32f4xx.h"
#include "tim2.h"

#define HP_SCL_H   GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define HP_SCL_L   GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define HP_SDA_H   GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define HP_SDA_L   GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define HP_OFF     GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define HP_ON      GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define HP_WRITE_ADDRESS  0XCC
#define HP_READ_ADDRESS   0XCD
void Hp6_Pin_Init(void);
u8 Bm_Open(void);
u8 Get_Bm(void);
u8 Bm_Close(void);
u8 Hr_Open(void);
u8 Get_Hr(void);
u8 Hr_Close(void);
void Get_Result(u8 *data);
#endif

