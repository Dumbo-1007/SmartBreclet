#ifndef IIC_H_
#define IIC_H_
#include "stm32f4xx.h"
#include "tim2.h"
#define SHT_SCL_H   GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define SHT_SCL_L   GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define SHT_SDA_H   GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define SHT_SDA_L   GPIO_ResetBits(GPIOB, GPIO_Pin_9)

void I2c_Init(u16 SCL, u16 SDA);
void Send_Start(u8 delay);
void Send_Stop(u8 delay);
u8 I2c_Send_Data(u8 data,u8 delay);
u8 I2c_Rec_Data(u8 ack,u8 delay);
#endif

