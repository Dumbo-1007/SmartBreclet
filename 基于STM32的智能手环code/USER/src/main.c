#include "includes.h"

//ʱ����ʾ����������ʪ�ȣ����������ʲ���������Ѫѹ�����������Ʋ������� �������ӣ���
//key_okֵ��left������right�ӣ���up�飰

int main(void)
{

	//�ж����ȼ��������ã�ѡ���2��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//��ʼ��
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

	//����һϵ�еĳ�ʼ��
	kaiji_gif();   //��������
	OLED_Clear(0);
	while(1)
	{
	//��tim5���жϷ������иı䣬key_okֵ��left������right�ӣ���up�飰��down:key_ok = 6
		switch (key_ok) 
		{
			case 0: show_tim();break;     //��ʾ��ǰʱ��
			
			case 1: show_TH();break;      //��ʾ��ǰ��ʪ��
			
			case 2:	show_rate();break;	  //���ʲ���
			
			case 3:	show_press();break;   //Ѫѹ����
			
			case 4: show_step();break;     //��������
			
			case 5: show_setclock();break; //��������
			
			case 6: show_curclock();break; //��ʾ��ǰ����
		}
		end_work();   //��β����
	}
}


