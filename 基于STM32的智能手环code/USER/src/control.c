#include "control.h"

s8 cnt = 5;
struct SET_ALARM alarm;

float tem = 0;
float hum = 0;
u8 ptem[10];
u8 phum[10];
extern int key_enter;  //确认按钮
s8 alarm_cnt = 0;

void alarm_control(void)
{
	static u8 week_buf[3];
	static u8 hour_buf[3];
	static u8 min_buf[3];
	if(UP)
	{
		alarm_cnt--;
		if(alarm_cnt < 0)
			alarm_cnt = 2;
		Delay_ms(200);
	}
	else if(DOWN)
	{
		alarm_cnt++;
		if(alarm_cnt > 2)
			alarm_cnt = 0;
		Delay_ms(200);
	}
	switch(alarm_cnt)
	{
		case 0:
		{
			if(LEFT)
			{
				alarm.week -= 1; 
				if(alarm.week < 1)
				{
					alarm.week = 7;
				}
				sprintf((char*)week_buf,"%01d",alarm.week);
				Show_String(0, 41, (u8*)week_buf);
				Delay_ms(300);
			}
			else if(RIGHT)
			{
				alarm.week += 1; 
				if(alarm.week > 7)
				{
					alarm.week = 1;
				}
				sprintf((char*)week_buf,"%01d",alarm.week);
				Show_String(0, 41, (u8*)week_buf);
				Delay_ms(300);
			}
			break;
		}
		case 1:
		{
			if(LEFT)
			{
				alarm.hour -= 1; 
				if(alarm.hour < 0)
				{
					alarm.hour = 23;
				}
				sprintf((char*)hour_buf,"%02d",alarm.hour);
				Show_String(2, 41, (u8*)hour_buf);
				Delay_ms(300);
			}
			else if(RIGHT)
			{
				alarm.hour += 1; 
				if(alarm.hour > 23)
				{
					alarm.hour = 0;
				}
				sprintf((char*)hour_buf,"%02d",alarm.hour);
				Show_String(2, 41, (u8*)hour_buf);
				Delay_ms(300);
			}
			break;
		}
		case 2:
		{
			if(LEFT)
			{
				alarm.minute -= 1; 
				if(alarm.minute < 0)
				{
					alarm.minute = 59;
				}
				sprintf((char*)min_buf,"%02d",alarm.minute);
				Show_String(4, 57, (u8*)min_buf);
				Delay_ms(300);
			}
			else if(RIGHT)
			{
				alarm.minute += 1; 
				if(alarm.minute > 59)
				{
					alarm.minute = 0;
				}
				sprintf((char*)min_buf,"%02d",alarm.minute);
				Show_String(4, 57, (u8*)min_buf);
				Delay_ms(300);
			}
			break;
		}	
	}
	
}



//void Watch_Control(void)
//{
//	if(LEFT)  //4095
//	{
//		cnt--;
//		if(cnt < 0)
//			cnt = 4;
//		Menu_Left(cnt);
//		Delay_ms(300);
//	}else if(RIGHT)//2048
//	{
//		cnt++;
//		if(cnt > 4)
//			cnt = 0;
//		Menu_Right(cnt);
//		Delay_ms(300);
//	}else if(UP)
//	{
//		RTC_WakeUpCmd(ENABLE);
//		cnt = 5;
//		Delay_ms(300);
//	}
//	if(Scan_Key())
//	{
//		switch(cnt)
//		{
//			case 0:
//			{
//				OLED_Clear(0);
//				Show_String(0, 0, (u8*)"week:0");
//				Show_String(2,0,(u8*)"hour:0");
//				Show_String(4,0,(u8*)"minute:0");
//				while(1)
//				{
//					alarm_control();
//					if(Scan_Key())
//					{
//						Rtc_Alarm(alarm.week,alarm.hour, alarm.minute,0);	
//						OLED_Clear(0);
//						Show_String(2,0,(u8*)"set success!");
//						OLED_Clear(0);
//						Show_Pic(0, 0, (u8*)Clock_Pic);
//						Show_Chinese(2, 65, 4,clock);
//						break;
//					}					
//				}
//				break;
//			}
//			case 1:
//			{
//				OLED_Clear(0);
//				static sportsInfo_t userSportsInfo;
//				accValue_t accValue_temp;
//				static timStamp_t timStamp;
//				u8 err;
//				u8 uiBuf[40];
//				static u8 tempSecond;
//				TIME_DATA *rtcTime; 							  //获取年月日时分秒
//				while (1)
//				{		
//					Mcu_Read_Mpu6050_XYZ(&accValue_temp);			  //得到加速度传感器数据
//					rtcTime = RTC_getDateAndTime(); 					  //获取当前RTC的值
//					if(tempSecond != timStamp.second)					 //秒更新
//					{
//						tempSecond = timStamp.second;
//						timStamp.twentyMsCount = 0 ;//20ms计数变量清零
//					}
//					else												  //秒不更新，1秒等于50*20ms
//					{
//						timStamp.twentyMsCount ++;//20ms计数变量++
//					}
//					timStamp.hour	 = rtcTime->hour;
//					timStamp.minute = rtcTime->minute;
//					timStamp.second = rtcTime->second;
//					//将三轴数据转换为以g为单位的数据
//					accValue_temp.accX = ((float)(int)accValue_temp.accX/16384) *10;
//					accValue_temp.accY = ((float)(int)accValue_temp.accY/16384) *10;
//					accValue_temp.accZ = ((float)(int)accValue_temp.accZ/16384) *10; 
//					userSportsInfo = *onSensorChanged(&accValue_temp,&timStamp,WatchInfo_getUserInfo(&err)); //调用计数算法
//					sprintf((char*)uiBuf,"Step:%05d ",userSportsInfo.stepCount); // 显示步数
//					sprintf((char*)uiBuf,"Step:%05d ",userSportsInfo.stepCount); // 显示步数
//					Show_String(0, 0, uiBuf);
//					sprintf((char*)uiBuf,"kal:%.1f KAL",userSportsInfo.calories); // 显示卡路里
//					Show_String(2, 0, uiBuf);
//					sprintf((char*)uiBuf,"dis:%.1f M",userSportsInfo.distance); // 显示里程
//					Show_String(4, 0, uiBuf);
//					
//					Delay_ms(20);  //以50Hz的频率去读取三轴加速度的XYZ轴加速度值
//	
//					if(Scan_Key())
//					{
//						OLED_Clear(0);
//						Show_Pic(0, 0, (u8*)Step_Pic);
//						Show_Chinese(2, 65, 4,step);
//						break;
//					}
//				}				
//				break;
//			}
//			case 2:
//			{
//				OLED_Clear(0);
//				while(1)
//				{
//					hum = Sht20_Mesaure(HUMIDITY);
//					sprintf((char*)phum,"%s%.2f%s","HUM:",hum,"%");
//					Show_String(0, 0, (u8*)phum);
//					tem = Sht20_Mesaure(TEMPEATURE);
//					sprintf((char*)ptem,"%s%.2f%s","TEM:",tem,"C");
//					Show_String(2, 0, (u8*)ptem);
//					Delay_ms(1000);
//					if(Scan_Key())
//					{
//						OLED_Clear(0);
//						Show_Pic(0, 0, (u8*)RT_Picture);
//						Show_Chinese(2, 50, 5,rt);
//						break;
//					}
//				}
//				break;
//			}
//			case 3:
//			{
//				u8 bm_buf[128];
//				OLED_Clear(0);
//				Show_Pic(0, 0, (u8*)BM_Tip);
//				Bm_Open();
//				Delay_ms(3000);
//				OLED_Clear(0);
//				while(1)
//				{
//					 Get_Bm();
//					 Get_Result(bm_buf);
//					
//					 if(bm_buf[7] == 0)
//					 {
//						OLED_Clear(0);
//						PWN_Led();
//						Delay_us(500);
//						sprintf((char*)bm_buf,"%s","measuring...");
//						Show_String(2, 10, (u8*)bm_buf);
//						PWN_Led();
//					 }
//					 else if(bm_buf[7] == 1)
//					 {
//						OLED_Clear(0);
//						sprintf((char*)bm_buf,"%d",bm_buf[10]);
//						Show_String(2, 0, (u8*)"high:");
//						Show_String(2, 42, (u8*)bm_buf);
//						Show_String(4, 0, (u8*)"low:");
//						sprintf((char*)bm_buf,"%d",bm_buf[11]);
//						Show_String(4, 34, (u8*)bm_buf);
//						Bm_Close();
//						break;
//					 }
//					 else if(bm_buf[7] == 2)
//					 {
//						Show_String(2, 0, (u8*)"measure error!");
//						Bm_Close();
//						break;
//					 }
//					 Delay_ms(1280);
//					if(Scan_Key())
//					{
//						Bm_Close();
//						OLED_Clear(0);
//						Show_Pic(0, 0, (u8*)BM_Picture);
//						Show_Chinese(2, 65, 4,Bm);
//						break;
//					}	
//				}
//				break;
//			}
//			case 4:
//			{
//				u8 hr_buf[128];
//				OLED_Clear(0);
//				Show_Pic(0, 0, (u8*)HR_Tip);
//				Hr_Open();
//				Delay_ms(5000);
//				OLED_Clear(0);
//				while(1)
//				{
//					Get_Hr();
//					Get_Result(hr_buf);
//					OLED_Clear(0);
//					sprintf((char*)hr_buf,"%d",hr_buf[7]);
//					Show_String(2, 0, (u8*)"heart rate:");
//					Show_String(2, 90, (u8*)hr_buf);
//					PWN_Led();
//					if(hr_buf[6] == 0)
//					{
//						Show_String(2, 0, (u8*)"measure error!");
//						Hr_Close();
//						cnt = 5;
//						break;
//					 }
//					Delay_ms(1250);
//					 
//					if(Scan_Key())
//					{
//						Hr_Close();
//						OLED_Clear(0);
//						Show_Pic(0, 0, (u8*)HR_Picture);
//						Show_Chinese(2, 65, 4,Hr);
//						break;
//					}	
//				}
//				break;
//			}
//			default: break;
//		}
//	}
//}

