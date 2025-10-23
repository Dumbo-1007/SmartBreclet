#ifndef RTC_H_
#define RTC_H_
#include "stm32f4xx.h"
#include "tim2.h"
#include "stdio.h"
#include "string.h"
#include "motor.h"
#include "oled.h"
typedef struct
{
	u8 hour;
	u8 minute;
	u8 second;
	u8 year;
	u8 month;
	u8 day;
	u8 week;
	u8 ampm;
}TIME_DATA;

typedef	struct
{
	u8 twentyMsCount;
	u8 hour;
	u8 minute;
	u8 second;	
}timStamp_t;

u8 My_Rtc_Init(void);
void Rtc_Wakeup_Init(void);
void Rtc_Alarm(u8 week,u8 hour, u8 minute, u8 second);
TIME_DATA *RTC_getDateAndTime(void);
timStamp_t *RTC_getDateAndTime2(void);

void clock_pic(void);
#endif

