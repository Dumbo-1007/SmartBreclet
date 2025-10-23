#include "rtc.h"

const char *pt = __TIME__;
const char *pd = __DATE__;
TIME_DATA tim_data;
u8 month[12][5] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
/***********************************************************
函数功能：判断闰年函数
函数形参：年
函数返回值：0平年  1闰年
************************************************************/
u8 Pd_Rn(u16 year)
{
	if( (year%4==0 && year%100!= 0) || (year%400 == 0) )
	{
		return 1;
	}
	return 0;
}

/***********************************************************
函数功能：1990年1月1日到今天的天数函数
函数形参：年 月 日
函数返回值：星期几
************************************************************/
u8 Statistics_Days(u16 year, u8 mon, u8 day)
{
	u32 buf = 0;//存储天数
	u16 i;//0-66535
	u8 week = 0;
	
	for(i = 1990; i < year; i++)
	{
		if( Pd_Rn(i) )
		{
			buf += 366;
		}else
		{
			buf += 365;
		}
	}
	
	switch(mon)//7
	{
		case 12: buf += 30; 
		case 11: buf += 31; 
		case 10: buf += 30; 
		case  9: buf += 31; 
		case  8: buf += 31; 
		case  7: buf += 30; 
		case  6: buf += 31; 
		case  5: buf += 30; 
		case  4: buf += 31; 
		case  3: buf += 28; buf += Pd_Rn(year); 
		case  2: buf += 31; 
		case  1: buf += 0; 
	}
	
	//统计从这个1日 到 今天的天数
	buf += day;//1990年1月1日  到  今天的天数
	
	switch(buf % 7)
	{
		case 1: week = 1; break;
		case 2: week = 2; break;
		case 3: week = 3; break;
		case 4: week = 4; break;
		case 5: week = 5; break;
		case 6: week = 6; break;
		case 0: week = 7; break;
	}
	
	return week;//将星期几返回出去了
}

//设置时间
ErrorStatus RTC_Set_Time(void)
{
	/*****************解析时间**********************/
	tim_data.hour = (pt[0]-'0')*10 + (pt[1]-'0');//、得到小时
	tim_data.minute = (pt[3]-'0')*10 + (pt[4]-'0');//得到分钟
	tim_data.second = (pt[6]-'0')*10 + (pt[7]-'0');//得到秒
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_TimeTypeInitStructure.RTC_Hours = tim_data.hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = tim_data.minute;
	RTC_TimeTypeInitStructure.RTC_Seconds = tim_data.second;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
}
	
//设置日期
ErrorStatus RTC_Setim_dataate()
{
	u8 str[4] = {0};
	u8 i;
	/*****************解析日期**********************/
	for(i = 0; i < 3; i++)
	{
		str[i] = pd[i];//May
	}
	str[i] = '\0';
	
	for(i = 0; i < 12; i++)
	{
		if(strcmp((char *)str, (char *)month[i]) == 0 )
		{
			i += 1;
			break;//找到月份了
		}
	}
	tim_data.month = i;//得到月
	if( pd[4] == ' ' )
	{
		tim_data.day = pd[5]-'0';//得到日
	}else
	{
		tim_data.day = (pd[4]-'0')*10 + (pd[5]-'0');//得到日
	}
	
	tim_data.year = (pd[9]-'0')*10 + (pd[10]-'0');//得到年
	
	/*****************解析星期**********************/
	tim_data.week = Statistics_Days(tim_data.year+2000, tim_data.month, tim_data.day);//得到星期几
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Year = tim_data.year;
	RTC_DateTypeInitStructure.RTC_Month = tim_data.month;
	RTC_DateTypeInitStructure.RTC_Date = tim_data.day;
	RTC_DateTypeInitStructure.RTC_WeekDay = tim_data.week;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

/************************
函数功能：RTC初始化
形参：无
返回值：成功返回0，失败返回1
说明：24小时制
************************/
u8 My_Rtc_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	RTC_WriteProtectionCmd(DISABLE); 
	u16 retry= 0; 
	//RCC_LSEConfig(RCC_LSE_ON);//LSE 开启 
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
	{
		retry++;
		Delay_ms(10);
		if(retry == 200)
		{	
			return 1;		//LSE 开启失败. 
		}
	}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 
	
	RTC_InitTypeDef RTC_InitStructure;
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
	RTC_InitStructure.RTC_SynchPrediv = 0xF9;//RTC同步分频系数(0~7FFF)
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//RTC设置为,24小时格式
	RTC_Init(&RTC_InitStructure);
	if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0xbbbb)
	{
		RTC_Set_Time();	//设置时间
		RTC_Setim_dataate();		//设置日期
		RTC_WriteBackupRegister(RTC_BKP_DR1, 0xbbbb); 
	}
	
	 
	return 0;
}

//RTC唤醒功能初始化
void Rtc_Wakeup_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);//配置
	
	RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//唤醒时钟选择
	RTC_SetWakeUpCounter(0);//设置WAKE UP自动重装载寄存器
	RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
	EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
	//设置中断
	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
	RTC_WakeUpCmd(ENABLE);//开启WAKE UP 定时器　
}



//WAKE UP中断函数
void RTC_WKUP_IRQHandler(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志 
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	//printf("20%02d-%02d-%02d-%01d\r\n",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
	//printf("%02d:%02d:%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);

}

//RTC闹钟功能初始化，周几的闹钟
void Rtc_Alarm(u8 week,u8 hour, u8 minute, u8 second)
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);//配置外部中断线
	
	RTC_AlarmTypeDef   RTC_AlarmAStruct;
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); //先关闭闹钟A

	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_TimeTypeInitStructure.RTC_Hours = hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = minute;
	RTC_TimeTypeInitStructure.RTC_Seconds = second;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmAStruct.RTC_AlarmDateWeekDay = week;
	RTC_AlarmAStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//按星期闹钟
	RTC_AlarmAStruct.RTC_AlarmMask = RTC_AlarmMask_None;//不屏蔽 
	RTC_AlarmAStruct.RTC_AlarmTime = RTC_TimeTypeInitStructure; 
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmAStruct);
	
	//设置中断
	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_EnableIRQ(RTC_Alarm_IRQn);	
	//使能闹钟A的中断
	RTC_ITConfig(RTC_IT_ALRA,ENABLE); 
	//开启闹钟A
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); 

}

//闹钟A中断服务函数
void RTC_Alarm_IRQHandler()
{
	//判断中断是否发生
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET)
	{
		OLED_Clear(0);
		MOTOR_ON;
		clock_pic();  //闹钟动图，约3S
		MOTOR_OFF;
		RTC_ClearITPendingBit(RTC_IT_ALRA);//清中断标志位
		OLED_Clear(0);
	}
	EXTI_ClearITPendingBit(EXTI_Line17);
}	

TIME_DATA dateAndTime;

//获取当前时间
TIME_DATA *RTC_getDateAndTime(void)
{
	RTC_DateTypeDef RTC_Date;//定义结构体，用于保存获取的日期和时间
	RTC_TimeTypeDef RTC_Time;
	RTC_GetDate(RTC_Format_BIN,&RTC_Date);
	RTC_GetTime(RTC_Format_BIN,&RTC_Time); 
	
	dateAndTime.year = RTC_Date.RTC_Year;
	dateAndTime.month = RTC_Date.RTC_Month;
	dateAndTime.day = RTC_Date.RTC_Date;
	dateAndTime.week = RTC_Date.RTC_WeekDay;
	
	dateAndTime.hour = RTC_Time.RTC_Hours;
	dateAndTime.minute = RTC_Time.RTC_Minutes;
	dateAndTime.second = RTC_Time.RTC_Seconds;
	dateAndTime.ampm = RTC_Time.RTC_H12;
	
	return &dateAndTime;
}

void clock_pic(void)
{
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 0, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8, (u8*)Clock_Pic);
	Delay_ms(100);
	Show_Pic(0, 8*2, (u8*)Clock_Pic);
	Delay_ms(100);
}

//#include "rtc.h"

//const char *pt = __TIME__;
//const char *pd = __DATE__;
//TIME_DATA tim_data;
//u8 month[12][5] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
///***********************************************************
//函数功能：判断闰年函数
//函数形参：年
//函数返回值：0平年  1闰年
//************************************************************/
//u8 Pd_Rn(u16 year)
//{
//	if( (year%4==0 && year%100!= 0) || (year%400 == 0) )
//	{
//		return 1;
//	}
//	return 0;
//}

///***********************************************************
//函数功能：1990年1月1日到今天的天数函数
//函数形参：年 月 日
//函数返回值：星期几
//************************************************************/
//u8 Statistics_Days(u16 year, u8 mon, u8 day)
//{
//	u32 buf = 0;//存储天数
//	u16 i;//0-66535
//	u8 week = 0;
//	
//	for(i = 1990; i < year; i++)
//	{
//		if( Pd_Rn(i) )
//		{
//			buf += 366;
//		}else
//		{
//			buf += 365;
//		}
//	}
//	
//	switch(mon)//7
//	{
//		case 12: buf += 30; 
//		case 11: buf += 31; 
//		case 10: buf += 30; 
//		case  9: buf += 31; 
//		case  8: buf += 31; 
//		case  7: buf += 30; 
//		case  6: buf += 31; 
//		case  5: buf += 30; 
//		case  4: buf += 31; 
//		case  3: buf += 28; buf += Pd_Rn(year); 
//		case  2: buf += 31; 
//		case  1: buf += 0; 
//	}
//	
//	//统计从这个1日 到 今天的天数
//	buf += day;//1990年1月1日  到  今天的天数
//	
//	switch(buf % 7)
//	{
//		case 1: week = 1; break;
//		case 2: week = 2; break;
//		case 3: week = 3; break;
//		case 4: week = 4; break;
//		case 5: week = 5; break;
//		case 6: week = 6; break;
//		case 0: week = 7; break;
//	}
//	
//	return week;//将星期几返回出去了
//}

//设置时间
//ErrorStatus RTC_Set_Time(void)
//{
//	/*****************解析时间**********************/
//	tim_data.hour = (pt[0]-'0')*10 + (pt[1]-'0');//、得到小时
//	tim_data.minute = (pt[3]-'0')*10 + (pt[4]-'0');//得到分钟
//	tim_data.second = (pt[6]-'0')*10 + (pt[7]-'0');//得到秒
//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	RTC_TimeTypeInitStructure.RTC_Hours = tim_data.hour;
//	RTC_TimeTypeInitStructure.RTC_Minutes = tim_data.minute;
//	RTC_TimeTypeInitStructure.RTC_Seconds = tim_data.second;
//	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
//	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
//}
//	
//设置日期
//ErrorStatus RTC_Setim_dataate()
//{
//	u8 str[4] = {0};
//	u8 i;
//	/*****************解析日期**********************/
//	for(i = 0; i < 3; i++)
//	{
//		str[i] = pd[i];//May
//	}
//	str[i] = '\0';
//	
//	for(i = 0; i < 12; i++)
//	{
//		if(strcmp((char *)str, (char *)month[i]) == 0 )
//		{
//			i += 1;
//			break;//找到月份了
//		}
//	}
//	tim_data.month = i;//得到月
//	if( pd[4] == ' ' )
//	{
//		tim_data.day = pd[5]-'0';//得到日
//	}else
//	{
//		tim_data.day = (pd[4]-'0')*10 + (pd[5]-'0');//得到日
//	}
//	
//	tim_data.year = (pd[9]-'0')*10 + (pd[10]-'0');//得到年
//	
//	/*****************解析星期**********************/
//	tim_data.week = Statistics_Days(tim_data.year+2000, tim_data.month, tim_data.day);//得到星期几
//	
//	RTC_DateTypeDef RTC_DateTypeInitStructure;
//	RTC_DateTypeInitStructure.RTC_Year = tim_data.year;
//	RTC_DateTypeInitStructure.RTC_Month = tim_data.month;
//	RTC_DateTypeInitStructure.RTC_Date = tim_data.day;
//	RTC_DateTypeInitStructure.RTC_WeekDay = tim_data.week;
//	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
//}

///************************
//函数功能：RTC初始化
//形参：无
//返回值：成功返回0，失败返回1
//说明：24小时制
//************************/
//u8 My_Rtc_Init(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
//	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
//	RTC_WriteProtectionCmd(DISABLE); 
//	u16 retry= 0; 
//	//RCC_LSEConfig(RCC_LSE_ON);//LSE 开启 
//	RCC_LSICmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
//	{
//		retry++;
//		Delay_ms(10);
//		if(retry == 200)
//		{	
//			return 1;		//LSE 开启失败. 
//		}
//	}

//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
//	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 
//	
//	RTC_InitTypeDef RTC_InitStructure;
//	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
//	RTC_InitStructure.RTC_SynchPrediv = 0xF9;//RTC同步分频系数(0~7FFF)
//	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//RTC设置为,24小时格式
//	RTC_Init(&RTC_InitStructure);
//	if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0xbbbb)
//	{
//		RTC_Set_Time();	//设置时间
//		RTC_Setim_dataate();		//设置日期
//		RTC_WriteBackupRegister(RTC_BKP_DR1, 0xbbbb); 
//	}
//	
//	 
//	return 0;
//}

//RTC唤醒功能初始化
//void Rtc_Wakeup_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure);//配置
//	
//	RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
//	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//唤醒时钟选择
//	RTC_SetWakeUpCounter(0);//设置WAKE UP自动重装载寄存器
//	RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
//	EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
//	//设置中断
//	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,1,1));
//	NVIC_EnableIRQ(RTC_WKUP_IRQn);
//	RTC_WakeUpCmd(ENABLE);//开启WAKE UP 定时器　
//}



//WAKE UP中断函数
//void RTC_WKUP_IRQHandler(void)
//{
//	u8 data[256];
//	u8 tim[256];
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
//	RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
//	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志 
//	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
//	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
//	sprintf((char*)data,"20%02d-%02d-%02d-%01d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
//	sprintf((char*)tim,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
//	OLED_Clear(0);
//	Show_String(0, 0, (u8*)data);
//	Show_String(2, 0, (u8*)tim);
//}

//RTC闹钟功能初始化，周几的闹钟
//void Rtc_Alarm(u8 week,u8 hour, u8 minute, u8 second)
//{
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure);//配置外部中断线
//	
//	RTC_AlarmTypeDef   RTC_AlarmAStruct;
//	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); //先关闭闹钟A

//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	RTC_TimeTypeInitStructure.RTC_Hours = hour;
//	RTC_TimeTypeInitStructure.RTC_Minutes = minute;
//	RTC_TimeTypeInitStructure.RTC_Seconds = second;
//	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
//	
//	RTC_AlarmAStruct.RTC_AlarmDateWeekDay = week;
//	RTC_AlarmAStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//按星期闹钟
//	RTC_AlarmAStruct.RTC_AlarmMask = RTC_AlarmMask_None;//不屏蔽 
//	RTC_AlarmAStruct.RTC_AlarmTime = RTC_TimeTypeInitStructure; 
//	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmAStruct);
//	
//	//设置中断
//	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,1,1));
//	NVIC_EnableIRQ(RTC_Alarm_IRQn);	
//	//使能闹钟A的中断
//	RTC_ITConfig(RTC_IT_ALRA,ENABLE); 
//	//开启闹钟A
//	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); 

//}

//闹钟A中断服务函数
//void RTC_Alarm_IRQHandler()
//{
//	//判断中断是否发生
//	if(RTC_GetITStatus(RTC_IT_ALRA)==SET)
//	{
//		RTC_ClearITPendingBit(RTC_IT_ALRA);//清中断标志位
//		MOTOR_ON;
//		Delay_ms(2000);
//		MOTOR_OFF;
//		
//	}
//	EXTI_ClearITPendingBit(EXTI_Line17);
//}	

//TIME_DATA dateAndTime;

//获取当前时间
//TIME_DATA *RTC_getDateAndTime(void)
//{
//	RTC_DateTypeDef RTC_Date;//定义结构体，用于保存获取的日期和时间
//	RTC_TimeTypeDef RTC_Time;
//	RTC_GetDate(RTC_Format_BIN,&RTC_Date);
//	RTC_GetTime(RTC_Format_BIN,&RTC_Time); 
//	
//	dateAndTime.year = RTC_Date.RTC_Year;
//	dateAndTime.month = RTC_Date.RTC_Month;
//	dateAndTime.day = RTC_Date.RTC_Date;
//	dateAndTime.week = RTC_Date.RTC_WeekDay;
//	
//	dateAndTime.hour = RTC_Time.RTC_Hours;
//	dateAndTime.minute = RTC_Time.RTC_Minutes;
//	dateAndTime.second = RTC_Time.RTC_Seconds;
//	dateAndTime.ampm = RTC_Time.RTC_H12;
//	
//	return &dateAndTime;
//}

