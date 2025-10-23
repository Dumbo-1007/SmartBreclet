#include "show.h"

int key_ok = 0;
int key_enter = 0;  //ȷ�ϰ�ť

//��������
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

//��ʾ��ǰʱ��
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
			TIM_Cmd(TIM3, DISABLE);  //ʧ��TIM3
			TIM_Cmd(TIM5, DISABLE);	
			Show_Chinese(6,96,2,lock_sreen);
			lock_flag = 1;
		}
		else if(lock_flag == 1)
		{
			TIM_Cmd(TIM3, ENABLE);  //��TIM3
			TIM_Cmd(TIM5, ENABLE);	
			Show_Chinese(6,96,2,unlock);
			Delay_ms(1000);
			OLED_Clear(0);
			lock_flag = 0;
		}
	}
}
//������ʪ��
void show_TH(void)
{
	float Tem,Hum;
	u8 buf[50] = {0},clear_flag = 1;
	
	Show_Pic(0, 0, (u8*)RT_Picture);
	Show_Chinese(2, 49, 5,rt);
	Delay_ms(800);
	while(key_enter)
	{
		if(clear_flag==1)  //����һ��
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

//��������
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
		//��������
		if(clear_flag==1)
		{
			OLED_Clear(0);
			Hr_Open();     //�������ʲ���
			Show_Pic(0, 0, (u8*)HR_Tip);
			Delay_ms(5000);
			clear_flag = 2;
		}
		if(clear_flag==2)
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		Get_Hr();  //�õ���������
		Get_Result(data); //�õ��������
		PWN_Led();
		sprintf((char*)buf,"heart rate:%d ",data[7]);
		Show_String(2, 0, buf);
		Delay_ms(1250);
	}
}
//����Ѫѹ
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
		//��������Ļ
		if(clear_flag==1)
		{
			OLED_Clear(0);
			Bm_Open();  //����Ѫѹ����
			Show_Pic(0, 0, (u8*)BM_Tip);
			Delay_ms(2560);
			clear_flag = 2;
		}
		if(clear_flag==2)
		{
			OLED_Clear(0);
			clear_flag = 0;
		}
		
		Get_Bm();  //�õ���������
		Get_Result(data); //�õ��������
		
		if(data[7]== 0)
		{
			//printf("���ڲ�����!\r\n");
			PWN_Led();  //PWM��
			Show_String(2, 10, (u8*)"Measuring...");
		}
		else if(data[7] == 1)
		{
			sprintf((char*)buf,"Hight:%d       ",data[10]);
			Show_String(2, 0, buf);
			sprintf((char*)buf,"Low:%d ",data[11]);
			Show_String(4, 0, buf);
			Bm_Close();   //�رղ���
			TIM_SetCompare2(TIM3,0); // �ص�
		}
		else if(data[7]==2)
		{
			Show_String(2, 0, (u8*)"Measure error!");
			Delay_ms(1500);
			key_enter = 0;   // �˳�����
			Bm_Close();   //�رղ���
			TIM_SetCompare2(TIM3,0); // �ص�
		}
		Delay_ms(1280);
	}
}

//��������
void show_step(void)
{
	u8 err,clear_flag = 1;
	u8 uiBuf[40];
	static sportsInfo_t userSportsInfo;
	static timStamp_t timStamp;
	static u8 tempSecond;						 //����������̬��
	///timStamp_t *rtcTime; 						 //��ȡ������ʱ����
	TIME_DATA *rtcTime; 							  //��ȡ������ʱ����
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
		Mcu_Read_Mpu6050_XYZ(&accValue);			  //�õ����ٶȴ���������
		rtcTime = RTC_getDateAndTime(); 					  //��ȡ��ǰRTC��ֵ
		if(tempSecond != timStamp.second)					 //�����
		{
			tempSecond = timStamp.second;
			timStamp.twentyMsCount = 0 ;//20ms������������
		}
		else												  //�벻���£�1�����50*20ms
		{
			timStamp.twentyMsCount ++;//20ms��������++
		}
		timStamp.hour	 = rtcTime->hour;
		timStamp.minute = rtcTime->minute;
		timStamp.second = rtcTime->second;
		//����������ת��Ϊ��gΪ��λ������

		accValue.accX = ((float)(int)accValue.accX/16384) *10;
		accValue.accY = ((float)(int)accValue.accY/16384) *10;
		accValue.accZ = ((float)(int)accValue.accZ/16384) *10; 
		userSportsInfo = *onSensorChanged(&accValue,&timStamp,WatchInfo_getUserInfo(&err)); //���ü����㷨
		sprintf((char*)uiBuf,"Step:%05d ",userSportsInfo.stepCount); // ��ʾ����
		Show_String(0,0,uiBuf); 
		sprintf((char*)uiBuf,"kal:%.1f KAL",userSportsInfo.calories); // ��ʾ��·��
		Show_String(2,2,uiBuf); 	
		sprintf((char*)uiBuf,"dis:%.1f M",userSportsInfo.distance); // ��ʾ���
		Show_String(4,0,uiBuf); 			

		Delay_ms(20);  //��50Hz��Ƶ��ȥ��ȡ������ٶȵ�XYZ����ٶ�ֵ
	}
}

//��������
u8 setclk_flag = 0;
void show_setclock(void)
{
	u8 clear_flag = 1;
	OLED_Clear(0);
	Show_Pic(0, 0, (u8*)Clock_Pic);  //����ͼƬ
	Show_Chinese(2, 65, 4,clock);    //���֣���������
	Delay_ms(800);
	while(key_enter)
	{
		setclk_flag = 1;
		if(clear_flag==1)  //�������һ����
		{
			OLED_Clear(0);
			Show_String(0, 0, (u8*)"week:0");
			Show_String(2,0,(u8*)"hour:0");
			Show_String(4,0,(u8*)"minute:0");
			clear_flag = 0;
		}
		alarm_control();  //��������		
	}
}

//��ʾ��ǰ����
void show_curclock(void)
{
	u8 buf[50];
	OLED_Clear(0);
	
	Show_Chinese(0, 20, 4,cur_clk);//���֣���ǰ����
	sprintf((char*)buf,"%d-%02d-%02d",alarm.week,alarm.hour,alarm.minute);
	Show_String(2, 20, buf);
	
	Delay_ms(1000);
}

//��β����
void end_work(void)
{ 
	//��������˳�ʱ����������
	if(setclk_flag==1&&key_enter==0) 
	{
		setclk_flag = 0;  //���־λ
		Rtc_Alarm(alarm.week,alarm.hour, alarm.minute,0);//��������
		OLED_Clear(0);
		Show_String(2,0,(u8*)"set success!");//��ʾ���óɹ�
		Delay_ms(1500);
		key_enter = 0;// ��ֹ�ֶ�����ν���
	}
	//��������˳�ʱ���رղ���
	if(rate_flag==1&&key_enter==0) 
	{
		rate_flag = 0;
		Hr_Close();   //�رղ���
		TIM_SetCompare2(TIM3,0); // �ص�
	}
	
	if(Bm_flag==1&&key_enter==0) 
	{
		Bm_flag = 0;
		Bm_Close();   //�رղ���
		TIM_SetCompare2(TIM3,0);
	}
	key_enter = 0;
	OLED_Clear(0); //����
}
