#ifndef SHT20_H_
#define SHT20_H_
#include "stm32f4xx.h"
#include "iic.h"
#include "tim2.h"

#define WRITE_ADDRESS 0X80
#define READ_ADDRESS  0X81
#define TEMPEATURE    0XF3
#define HUMIDITY  	  0XF5
void Sht20_Init(void);
float Sht20_Mesaure(u8 cmd);
#endif
