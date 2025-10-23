#include "key.h"

//Key--PA0
void Key_init(void)
{
	GPIO_InitTypeDef gpio_initStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	gpio_initStruct.GPIO_Mode = GPIO_Mode_IN ;
	gpio_initStruct.GPIO_Pin = GPIO_Pin_0 ;
	gpio_initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	gpio_initStruct.GPIO_Speed = GPIO_Fast_Speed ;
	
	GPIO_Init(GPIOA,&gpio_initStruct);

}


u8 Key_Scan(void)
{
	static u8 key_flag = 0;
	if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1) && key_flag == 0)
	{
		Delay_ms(10);//ÑÓÊ±Ïû¶¶
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
		{
			key_flag = 1;
			return 1;
		}	
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		key_flag = 0;
	}
	return 0;
}
