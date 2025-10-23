#include "show.h"

int key_ok = 0;
int key_enter = 0;  //确认按钮

//开机动画
void kaiji_gif(void)
{
	Show_Pic(0,0,(u8*)gImage_20);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_21);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_22);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_23);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_24);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_25);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_26);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_27);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_28);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_29);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_30);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_31);
	Delay_ms(100);
	Show_Pic(0,0,(u8*)gImage_32);
	Delay_ms(1000);
}

//显示当前时间
void show_tim(void)
{
	u8 buf[50] = {0};
	TIME_DATA	*p;
	u8 lock_flag = 0;
	//OLED_Clear(0);
	while(key_ok==0)
	{
		Show_Chinese(0, 20, 4,tim);
		p = RTC_getDateAndTime();
		sprintf((char*)buf,"20%02d-%02d-%02d-%d",p->year,p->month,p->day,p->week);
		Show_String(2, 0, buf);
		
		sprintf((char*)buf,"%02d:%02d:%02d",p->hour,p->minute,p->second);
		Show_String(4, 0, buf);
		if(key_enter==1)
		{
			TIM_Cmd(TIM3, DISABLE);  //失能TIM3
			TIM_Cmd(TIM5, DISABLE);	
			Show_Chinese(6,96,2,lock_sreen);
			lock_flag = 1;
		}
		else if(lock_flag == 1)
		{
			TIM_Cmd(TIM3, ENABLE);  //能TIM3
			TIM_Cmd(TIM5, ENABLE);	
			Show_Chinese(6,96,2,unlock);
			Delay_ms(1000);
			OLED_Clear(0);
			lock_flag = 0;
		}
	}
}
//测量温湿度
void show_TH(void)
{
	float Tem,Hum;
	u8 buf[50] = {0},clear_flag = 1;
	
	Show_Pic(0, 0, (u8*)RT_Picture);
	Show_Chinese(2, 49, 5,rt);
	Delay_ms(800);
	while(key_enter)
	{
		if(clear_flag==1)  //清屏一次
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		Show_Chinese(0, 20, 5,cur_TH);
		Tem = Sht20_Mesaure(TEMPEATURE);
		Hum = Sht20_Mesaure(HUMIDITY);
		sprintf((char*)buf,"Tem:%.2fC",Tem);
		Show_String(2, 0, buf);
		sprintf((char*)buf,"Hum:%.2f%%",Hum);
		Show_String(4, 0, buf);
		Delay_ms(500);
	}
}

//测量心率
u8 rate_flag = 0;
void show_rate(void)
{
	u8 data[24] = {0},buf[50] = {0},clear_flag = 1;
	Show_Pic(0, 0, (u8*)HR_Picture);
	Show_Chinese(2, 65, 4,Hr);
	Delay_ms(800);
	while(key_enter)
	{
		rate_flag = 1;
		//清屏两次
		if(clear_flag==1)
		{
			OLED_Clear(0);
			Hr_Open();     //开启心率测量
			Show_Pic(0, 0, (u8*)HR_Tip);
			Delay_ms(5000);
			clear_flag = 2;
		}
		if(clear_flag==2)
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		Get_Hr();  //得到测量数据
		Get_Result(data); //得到测量结果
		PWN_Led();
		sprintf((char*)buf,"heart rate:%d ",data[7]);
		Show_String(2, 0, buf);
		Delay_ms(1250);
	}
}
//测量血压
u8 Bm_flag = 0;
void show_press(void)
{
	u8 data[24] = {0},buf[50] = {0},clear_flag = 1;
	
	Show_Pic(0, 0, (u8*)BM_Picture);
	Show_Chinese(2, 65, 4,Bm);
	Delay_ms(800);
	while(key_enter)
	{
		Bm_flag = 1;
		//清两次屏幕
		if(clear_flag==1)
		{
			OLED_Clear(0);
			Bm_Open();  //开启血压测量
			Show_Pic(0, 0, (u8*)BM_Tip);
			Delay_ms(2560);
			clear_flag = 2;
		}
		if(clear_flag==2)
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		
		Get_Bm();  //得到测量数据
		Get_Result(data); //得到测量结果
		
		if(data[7]== 0)
		{
			//printf("正在测量中!\r\n");
			PWN_Led();  //PWM灯
			Show_String(2, 10, (u8*)"Measuring...");
		}
		else if(data[7] == 1)
		{
			sprintf((char*)buf,"Hight:%d       ",data[10]);
			Show_String(2, 0, buf);
			sprintf((char*)buf,"Low:%d ",data[11]);
			Show_String(4, 0, buf);
			Bm_Close();   //关闭测量
			TIM_SetCompare2(TIM3,0); // 关灯
		}
		else if(data[7]==2)
		{
			Show_String(2, 0, (u8*)"Measure error!");
			Delay_ms(1500);
			key_enter = 0;   // 退出测量
			Bm_Close();   //关闭测量
			TIM_SetCompare2(TIM3,0); // 关灯
		}
		Delay_ms(1280);
	}
}

//步数计数
void show_step(void)
{
	u8 err,clear_flag = 1;
	u8 uiBuf[40];
	static sportsInfo_t userSportsInfo;
	static timStamp_t timStamp;
	static u8 tempSecond;						 //保存秒钟暂态量
	///timStamp_t *rtcTime; 						 //获取年月日时分秒
	TIME_DATA *rtcTime; 							  //获取年月日时分秒
	accValue_t accValue;
	Show_Pic(0, 0, (u8*)Step_Pic);
	Show_Chinese(2, 65, 4,step);
	Delay_ms(800);
	while(key_enter)
	{
		if(clear_flag==1)
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		Mcu_Read_Mpu6050_XYZ(&accValue);			  //得到加速度传感器数据
		rtcTime = RTC_getDateAndTime(); 					  //获取当前RTC的值
		if(tempSecond != timStamp.second)					 //秒更新
		{
			tempSecond = timStamp.second;
			timStamp.twentyMsCount = 0 ;//20ms计数变量清零
		}
		else												  //秒不更新，1秒等于50*20ms
		{
			timStamp.twentyMsCount ++;//20ms计数变量++
		}
		timStamp.hour	 = rtcTime->hour;
		timStamp.minute = rtcTime->minute;
		timStamp.second = rtcTime->second;
		//将三轴数据转换为以g为单位的数据

		accValue.accX = ((float)(int)accValue.accX/16384) *10;
		accValue.accY = ((float)(int)accValue.accY/16384) *10;
		accValue.accZ = ((float)(int)accValue.accZ/16384) *10; 
		userSportsInfo = *onSensorChanged(&accValue,&timStamp,WatchInfo_getUserInfo(&err)); //调用计数算法
		sprintf((char*)uiBuf,"Step:%05d ",userSportsInfo.stepCount); // 显示步数
		Show_String(0,0,uiBuf); 
		sprintf((char*)uiBuf,"kal:%.1f KAL",userSportsInfo.calories); // 显示卡路里
		Show_String(2,2,uiBuf); 	
		sprintf((char*)uiBuf,"dis:%.1f M",userSportsInfo.distance); // 显示里程
		Show_String(4,0,uiBuf); 			

		Delay_ms(20);  //以50Hz的频率去读取三轴加速度的XYZ轴加速度值
	}
}

//设置闹钟
u8 setclk_flag = 0;
void show_setclock(void)
{
	u8 clear_flag = 1;
	OLED_Clear(0);
	Show_Pic(0, 0, (u8*)Clock_Pic);  //闹钟图片
	Show_Chinese(2, 65, 4,clock);    //文字：闹钟设置
	Delay_ms(800);
	while(key_enter)
	{
		setclk_flag = 1;
		if(clear_flag==1)  //进入后清一次屏
		{
			OLED_Clear(0);
			Show_String(0, 0, (u8*)"week:0");
			Show_String(2,0,(u8*)"hour:0");
			Show_String(4,0,(u8*)"minute:0");
			clear_flag = 0;
		}
		alarm_control();  //设置闹钟		
	}
}

//显示当前闹钟
void show_curclock(void)
{
	u8 buf[50];
	OLED_Clear(0);
	
	Show_Chinese(0, 20, 4,cur_clk);//文字：当前闹钟
	sprintf((char*)buf,"%d-%02d-%02d",alarm.week,alarm.hour,alarm.minute);
	Show_String(2, 20, buf);
	
	Delay_ms(1000);
}

//收尾工作
void end_work(void)
{ 
	//进入后且退出时，开启闹钟
	if(setclk_flag==1&&key_enter==0) 
	{
		setclk_flag = 0;  //清标志位
		Rtc_Alarm(alarm.week,alarm.hour, alarm.minute,0);//设置闹钟
		OLED_Clear(0);
		Show_String(2,0,(u8*)"set success!");//提示设置成功
		Delay_ms(1500);
		key_enter = 0;// 防止手抖，多次进入
	}
	//进入后且退出时，关闭测量
	if(rate_flag==1&&key_enter==0) 
	{
		rate_flag = 0;
		Hr_Close();   //关闭测量
		TIM_SetCompare2(TIM3,0); // 关灯
	}
	
	if(Bm_flag==1&&key_enter==0) 
	{
		Bm_flag = 0;
		Bm_Close();   //关闭测量
		TIM_SetCompare2(TIM3,0);
	}
	key_enter = 0;
	OLED_Clear(0); //清屏
}
