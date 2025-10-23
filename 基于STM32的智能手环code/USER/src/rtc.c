#include "rtc.h"

const char *pt = __TIME__;
const char *pd = __DATE__;
TIME_DATA tim_data;
u8 month[12][5] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
/***********************************************************
�������ܣ��ж����꺯��
�����βΣ���
��������ֵ��0ƽ��  1����
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
�������ܣ�1990��1��1�յ��������������
�����βΣ��� �� ��
��������ֵ�����ڼ�
************************************************************/
u8 Statistics_Days(u16 year, u8 mon, u8 day)
{
	u32 buf = 0;//�洢����
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
	
	//ͳ�ƴ����1�� �� ���������
	buf += day;//1990��1��1��  ��  ���������
	
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
	
	return week;//�����ڼ����س�ȥ��
}

//����ʱ��
ErrorStatus RTC_Set_Time(void)
{
	/*****************����ʱ��**********************/
	tim_data.hour = (pt[0]-'0')*10 + (pt[1]-'0');//���õ�Сʱ
	tim_data.minute = (pt[3]-'0')*10 + (pt[4]-'0');//�õ�����
	tim_data.second = (pt[6]-'0')*10 + (pt[7]-'0');//�õ���
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_TimeTypeInitStructure.RTC_Hours = tim_data.hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = tim_data.minute;
	RTC_TimeTypeInitStructure.RTC_Seconds = tim_data.second;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
}
	
//��������
ErrorStatus RTC_Setim_dataate()
{
	u8 str[4] = {0};
	u8 i;
	/*****************��������**********************/
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
			break;//�ҵ��·���
		}
	}
	tim_data.month = i;//�õ���
	if( pd[4] == ' ' )
	{
		tim_data.day = pd[5]-'0';//�õ���
	}else
	{
		tim_data.day = (pd[4]-'0')*10 + (pd[5]-'0');//�õ���
	}
	
	tim_data.year = (pd[9]-'0')*10 + (pd[10]-'0');//�õ���
	
	/*****************��������**********************/
	tim_data.week = Statistics_Days(tim_data.year+2000, tim_data.month, tim_data.day);//�õ����ڼ�
	
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	RTC_DateTypeInitStructure.RTC_Year = tim_data.year;
	RTC_DateTypeInitStructure.RTC_Month = tim_data.month;
	RTC_DateTypeInitStructure.RTC_Date = tim_data.day;
	RTC_DateTypeInitStructure.RTC_WeekDay = tim_data.week;
	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
}

/************************
�������ܣ�RTC��ʼ��
�βΣ���
����ֵ���ɹ�����0��ʧ�ܷ���1
˵����24Сʱ��
************************/
u8 My_Rtc_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
	RTC_WriteProtectionCmd(DISABLE); 
	u16 retry= 0; 
	//RCC_LSEConfig(RCC_LSE_ON);//LSE ���� 
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
	{
		retry++;
		Delay_ms(10);
		if(retry == 200)
		{	
			return 1;		//LSE ����ʧ��. 
		}
	}

	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
	RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 
	
	RTC_InitTypeDef RTC_InitStructure;
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
	RTC_InitStructure.RTC_SynchPrediv = 0xF9;//RTCͬ����Ƶϵ��(0~7FFF)
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
	if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0xbbbb)
	{
		RTC_Set_Time();	//����ʱ��
		RTC_Setim_dataate();		//��������
		RTC_WriteBackupRegister(RTC_BKP_DR1, 0xbbbb); 
	}
	
	 
	return 0;
}

//RTC���ѹ��ܳ�ʼ��
void Rtc_Wakeup_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);//����
	
	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//����ʱ��ѡ��
	RTC_SetWakeUpCounter(0);//����WAKE UP�Զ���װ�ؼĴ���
	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
	EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ
	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
	//�����ж�
	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
	RTC_WakeUpCmd(ENABLE);//����WAKE UP ��ʱ����
}



//WAKE UP�жϺ���
void RTC_WKUP_IRQHandler(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	//printf("20%02d-%02d-%02d-%01d\r\n",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
	//printf("%02d:%02d:%02d\r\n",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);

}

//RTC���ӹ��ܳ�ʼ�����ܼ�������
void Rtc_Alarm(u8 week,u8 hour, u8 minute, u8 second)
{
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);//�����ⲿ�ж���
	
	RTC_AlarmTypeDef   RTC_AlarmAStruct;
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); //�ȹر�����A

	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_TimeTypeInitStructure.RTC_Hours = hour;
	RTC_TimeTypeInitStructure.RTC_Minutes = minute;
	RTC_TimeTypeInitStructure.RTC_Seconds = second;
	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
	
	RTC_AlarmAStruct.RTC_AlarmDateWeekDay = week;
	RTC_AlarmAStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//����������
	RTC_AlarmAStruct.RTC_AlarmMask = RTC_AlarmMask_None;//������ 
	RTC_AlarmAStruct.RTC_AlarmTime = RTC_TimeTypeInitStructure; 
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmAStruct);
	
	//�����ж�
	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,1,1));
	NVIC_EnableIRQ(RTC_Alarm_IRQn);	
	//ʹ������A���ж�
	RTC_ITConfig(RTC_IT_ALRA,ENABLE); 
	//��������A
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); 

}

//����A�жϷ�����
void RTC_Alarm_IRQHandler()
{
	//�ж��ж��Ƿ���
	if(RTC_GetITStatus(RTC_IT_ALRA)==SET)
	{
		OLED_Clear(0);
		MOTOR_ON;
		clock_pic();  //���Ӷ�ͼ��Լ3S
		MOTOR_OFF;
		RTC_ClearITPendingBit(RTC_IT_ALRA);//���жϱ�־λ
		OLED_Clear(0);
	}
	EXTI_ClearITPendingBit(EXTI_Line17);
}	

TIME_DATA dateAndTime;

//��ȡ��ǰʱ��
TIME_DATA *RTC_getDateAndTime(void)
{
	RTC_DateTypeDef RTC_Date;//����ṹ�壬���ڱ����ȡ�����ں�ʱ��
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
//�������ܣ��ж����꺯��
//�����βΣ���
//��������ֵ��0ƽ��  1����
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
//�������ܣ�1990��1��1�յ��������������
//�����βΣ��� �� ��
//��������ֵ�����ڼ�
//************************************************************/
//u8 Statistics_Days(u16 year, u8 mon, u8 day)
//{
//	u32 buf = 0;//�洢����
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
//	//ͳ�ƴ����1�� �� ���������
//	buf += day;//1990��1��1��  ��  ���������
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
//	return week;//�����ڼ����س�ȥ��
//}

//����ʱ��
//ErrorStatus RTC_Set_Time(void)
//{
//	/*****************����ʱ��**********************/
//	tim_data.hour = (pt[0]-'0')*10 + (pt[1]-'0');//���õ�Сʱ
//	tim_data.minute = (pt[3]-'0')*10 + (pt[4]-'0');//�õ�����
//	tim_data.second = (pt[6]-'0')*10 + (pt[7]-'0');//�õ���
//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	RTC_TimeTypeInitStructure.RTC_Hours = tim_data.hour;
//	RTC_TimeTypeInitStructure.RTC_Minutes = tim_data.minute;
//	RTC_TimeTypeInitStructure.RTC_Seconds = tim_data.second;
//	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
//	return RTC_SetTime(RTC_Format_BIN,&RTC_TimeTypeInitStructure);
//}
//	
//��������
//ErrorStatus RTC_Setim_dataate()
//{
//	u8 str[4] = {0};
//	u8 i;
//	/*****************��������**********************/
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
//			break;//�ҵ��·���
//		}
//	}
//	tim_data.month = i;//�õ���
//	if( pd[4] == ' ' )
//	{
//		tim_data.day = pd[5]-'0';//�õ���
//	}else
//	{
//		tim_data.day = (pd[4]-'0')*10 + (pd[5]-'0');//�õ���
//	}
//	
//	tim_data.year = (pd[9]-'0')*10 + (pd[10]-'0');//�õ���
//	
//	/*****************��������**********************/
//	tim_data.week = Statistics_Days(tim_data.year+2000, tim_data.month, tim_data.day);//�õ����ڼ�
//	
//	RTC_DateTypeDef RTC_DateTypeInitStructure;
//	RTC_DateTypeInitStructure.RTC_Year = tim_data.year;
//	RTC_DateTypeInitStructure.RTC_Month = tim_data.month;
//	RTC_DateTypeInitStructure.RTC_Date = tim_data.day;
//	RTC_DateTypeInitStructure.RTC_WeekDay = tim_data.week;
//	return RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
//}

///************************
//�������ܣ�RTC��ʼ��
//�βΣ���
//����ֵ���ɹ�����0��ʧ�ܷ���1
//˵����24Сʱ��
//************************/
//u8 My_Rtc_Init(void)
//{
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
//	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
//	RTC_WriteProtectionCmd(DISABLE); 
//	u16 retry= 0; 
//	//RCC_LSEConfig(RCC_LSE_ON);//LSE ���� 
//	RCC_LSICmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
//	{
//		retry++;
//		Delay_ms(10);
//		if(retry == 200)
//		{	
//			return 1;		//LSE ����ʧ��. 
//		}
//	}

//	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
//	RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ�� 
//	
//	RTC_InitTypeDef RTC_InitStructure;
//	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC�첽��Ƶϵ��(1~0X7F)
//	RTC_InitStructure.RTC_SynchPrediv = 0xF9;//RTCͬ����Ƶϵ��(0~7FFF)
//	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;//RTC����Ϊ,24Сʱ��ʽ
//	RTC_Init(&RTC_InitStructure);
//	if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0xbbbb)
//	{
//		RTC_Set_Time();	//����ʱ��
//		RTC_Setim_dataate();		//��������
//		RTC_WriteBackupRegister(RTC_BKP_DR1, 0xbbbb); 
//	}
//	
//	 
//	return 0;
//}

//RTC���ѹ��ܳ�ʼ��
//void Rtc_Wakeup_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line22;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure);//����
//	
//	RTC_WakeUpCmd(DISABLE);//�ر�WAKE UP
//	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//����ʱ��ѡ��
//	RTC_SetWakeUpCounter(0);//����WAKE UP�Զ���װ�ؼĴ���
//	RTC_ClearITPendingBit(RTC_IT_WUT); //���RTC WAKE UP�ı�־
//	EXTI_ClearITPendingBit(EXTI_Line22);//���LINE22�ϵ��жϱ�־λ
//	RTC_ITConfig(RTC_IT_WUT,ENABLE);//����WAKE UP ��ʱ���ж�
//	//�����ж�
//	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,1,1));
//	NVIC_EnableIRQ(RTC_WKUP_IRQn);
//	RTC_WakeUpCmd(ENABLE);//����WAKE UP ��ʱ����
//}



//WAKE UP�жϺ���
//void RTC_WKUP_IRQHandler(void)
//{
//	u8 data[256];
//	u8 tim[256];
//	RTC_TimeTypeDef RTC_TimeStruct;
//	RTC_DateTypeDef RTC_DateStruct;
//	RTC_ClearFlag(RTC_FLAG_WUTF);	//����жϱ�־
//	EXTI_ClearITPendingBit(EXTI_Line22);//����ж���22���жϱ�־ 
//	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
//	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
//	sprintf((char*)data,"20%02d-%02d-%02d-%01d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
//	sprintf((char*)tim,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
//	OLED_Clear(0);
//	Show_String(0, 0, (u8*)data);
//	Show_String(2, 0, (u8*)tim);
//}

//RTC���ӹ��ܳ�ʼ�����ܼ�������
//void Rtc_Alarm(u8 week,u8 hour, u8 minute, u8 second)
//{
//	
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStructure);//�����ⲿ�ж���
//	
//	RTC_AlarmTypeDef   RTC_AlarmAStruct;
//	RTC_AlarmCmd(RTC_Alarm_A,DISABLE); //�ȹر�����A

//	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
//	RTC_TimeTypeInitStructure.RTC_Hours = hour;
//	RTC_TimeTypeInitStructure.RTC_Minutes = minute;
//	RTC_TimeTypeInitStructure.RTC_Seconds = second;
//	RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;
//	
//	RTC_AlarmAStruct.RTC_AlarmDateWeekDay = week;
//	RTC_AlarmAStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//����������
//	RTC_AlarmAStruct.RTC_AlarmMask = RTC_AlarmMask_None;//������ 
//	RTC_AlarmAStruct.RTC_AlarmTime = RTC_TimeTypeInitStructure; 
//	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmAStruct);
//	
//	//�����ж�
//	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,1,1));
//	NVIC_EnableIRQ(RTC_Alarm_IRQn);	
//	//ʹ������A���ж�
//	RTC_ITConfig(RTC_IT_ALRA,ENABLE); 
//	//��������A
//	RTC_AlarmCmd(RTC_Alarm_A,ENABLE); 

//}

//����A�жϷ�����
//void RTC_Alarm_IRQHandler()
//{
//	//�ж��ж��Ƿ���
//	if(RTC_GetITStatus(RTC_IT_ALRA)==SET)
//	{
//		RTC_ClearITPendingBit(RTC_IT_ALRA);//���жϱ�־λ
//		MOTOR_ON;
//		Delay_ms(2000);
//		MOTOR_OFF;
//		
//	}
//	EXTI_ClearITPendingBit(EXTI_Line17);
//}	

//TIME_DATA dateAndTime;

//��ȡ��ǰʱ��
//TIME_DATA *RTC_getDateAndTime(void)
//{
//	RTC_DateTypeDef RTC_Date;//����ṹ�壬���ڱ����ȡ�����ں�ʱ��
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

