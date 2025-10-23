#ifndef CONTROL_H_
#define CONTROL_H_
#include "includes.h"


#define LEFT  (ADC_value >= 4085)
#define RIGHT (ADC_value > 2035 && ADC_value < 2050) 
#define UP 	  (ADC_value > 1020 && ADC_value < 1030)
#define DOWN  (ADC_value > 1360 && ADC_value < 1370)

//typedef struct
//{
//	s8 hour;
//	s8 minute;
//	u8 week;
//}SET_ALARM;
struct SET_ALARM
{
	s8 hour;
	s8 minute;
	u8 week;
};

extern struct SET_ALARM alarm;

void Watch_Control(void);
void alarm_control(void);


#endif

