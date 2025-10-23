#ifndef ADC_H_
#define ADC_H_
#include "stm32f4xx.h"
#include "stdio.h"

extern u16 ADC_value;
void Adc_Init(void);
#define LEFT  (ADC_value >= 4085)//3.3v
#define RIGHT (ADC_value > 2035 && ADC_value < 2050)//4096/2 
#define UP 	  (ADC_value > 1020 && ADC_value < 1030)//4096/4
#define DOWN  (ADC_value > 1360 && ADC_value < 1370)//4096/3

u16 adc_test(void);

#endif


