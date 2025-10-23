#include "iic.h"

/*
***************************
函数功能：IIC初始化
函数参数：u16 SCL, u16 SDA
函数返回值：None

***************************
*/
void I2c_Init(u16 SCL, u16 SDA)
{
	//开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//推挽输出
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = SCL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//开漏输出
	GPIO_InitStruct.GPIO_Pin = SDA;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	SHT_SCL_H;
	SHT_SDA_H;
}

/*
**********************************************************************
函数功能：iic起始条件函数
函数形参：u8 delay
函数返回值：None
备注：时钟线高电平期间，数据线产生下降沿
**********************************************************************
*/
void Send_Start(u8 delay)
{
	SHT_SCL_H;
	SHT_SDA_H;
	Delay_us(delay);
	SHT_SDA_L;
	Delay_us(delay);
	SHT_SCL_L;
}

/*
**********************************************************************
函数功能：iic停止条件函数
函数形参：u8 delay
函数返回值：None
备注：时钟线高电平期间，数据线产生上升沿
**********************************************************************
*/
void Send_Stop(u8 delay)
{
	SHT_SDA_L;
	SHT_SCL_H;
	Delay_us(delay);
	SHT_SDA_H;
	Delay_us(delay);
}

/*
**********************************************************************
函数功能：主机发送应答信号
函数形参：u8 ack,u8 delay
函数返回值：None
**********************************************************************
*/
void Mcu_Send_Ask(u8 ack,u8 delay)
{
	SHT_SCL_L;
	if(ack)
		SHT_SDA_H;
	else
		SHT_SDA_L;
	Delay_us(delay);
	SHT_SCL_H;
	Delay_us(delay);
	SHT_SCL_L;
}

/*
**********************************************************************
函数功能：主机接收应答信号
函数形参：u8 delay
函数返回值：接收到的应答信号
**********************************************************************
*/
u8 Mcu_Rec_Ask(u8 delay)
{
	u8 ack = 0;
	SHT_SDA_H;
	SHT_SCL_L;
	Delay_us(delay);
	SHT_SCL_H;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		ack = 1;
	Delay_us(delay);
	SHT_SCL_L;
	return ack;
}

/*
**********************************************************************
函数功能：主机发送一个字节数据，再接收一个应答信号函数
函数形参：待发送的数据,u8 delay
函数返回值：接收到的应答信号
备注：0：应答   1：非应答     8bit   先高后低
**********************************************************************
*/
u8 I2c_Send_Data(u8 data,u8 delay)
{
	u8 i,ack;
	for(i = 0; i < 8; i++)
	{
		SHT_SCL_L;
		if(data & (0x80 >> i))
			SHT_SDA_H;
		else
			SHT_SDA_L;
		Delay_us(delay);
		SHT_SCL_H;
		Delay_us(delay);
	}
	SHT_SCL_L;
	ack = Mcu_Rec_Ask(delay);
	return ack;
}

/*
**********************************************************************
函数功能：主机接收一个字节数据，再发送一个应答信号函数
函数形参：待发送的应答,u8 delay
函数返回值：接收到的数据
备注：0：应答   1：非应答     8bit   先高后低
**********************************************************************
*/
u8 I2c_Rec_Data(u8 ack,u8 delay)
{
	u8 i;
	u8 buf = 0;
	SHT_SDA_H;
	for(i = 0; i<8; i++)
	{
		SHT_SCL_L;
		Delay_us(delay);
		SHT_SCL_H;
		buf <<= 1;
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))
		{
			buf |= 1;
		}
		Delay_us(delay);	
	}
	SHT_SCL_H;
	Mcu_Send_Ask(ack,delay);
	return buf;
}

