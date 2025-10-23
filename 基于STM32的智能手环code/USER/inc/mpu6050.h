#ifndef MPU_H_
#define MPU_H_
#include "stm32f4xx.h"
#include "iic.h"
#include "tim2.h"
#define MPU6050_ADDR  0x68

typedef struct 
{
	float accX;
	float accY;
	float accZ;
}accValue_t;


u8 Mpu6050_Init(void);
void Mcu_Read_Mpu6050_XYZ(accValue_t *p);

#endif




