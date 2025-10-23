#include "includes.h"

//时间显示：０　　温湿度：１　　心率测量：２　血压测量：３　计步数：４ 设置闹钟：５
//key_ok值：left减１，right加１；up归０

int main(void)
{

	//中断优先级的组配置；选择第2组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化
	LED_init();
	Breath_Light_Init();
	Key_init();
	Motor_Init();
	exti0_init();
	tim5_init();
	OLED_Init();
	Adc_Init();
	Sht20_Init();
	My_Rtc_Init();
	Hp6_Pin_Init();
	Mpu6050_Init();   
	WatchInfo_init();
	Rtc_Wakeup_Init();

	//上面一系列的初始化
	kaiji_gif();   //开机动画
	OLED_Clear(0);
	while(1)
	{
	//在tim5的中断服务函数中改变，key_ok值：left减１，right加１；up归０，down:key_ok = 6
		switch (key_ok) 
		{
			case 0: show_tim();break;     //显示当前时间
			
			case 1: show_TH();break;      //显示当前温湿度
			
			case 2:	show_rate();break;	  //心率测量
			
			case 3:	show_press();break;   //血压测量
			
			case 4: show_step();break;     //步数计数
			
			case 5: show_setclock();break; //设置闹钟
			
			case 6: show_curclock();break; //显示当前闹钟
		}
		end_work();   //收尾工作
	}
}


