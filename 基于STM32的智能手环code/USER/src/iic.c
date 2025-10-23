#include "iic.h"

/*
***************************
�������ܣ�IIC��ʼ��
����������u16 SCL, u16 SDA
��������ֵ��None

***************************
*/
void I2c_Init(u16 SCL, u16 SDA)
{
	//��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//�������
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = SCL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//��©���
	GPIO_InitStruct.GPIO_Pin = SDA;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	SHT_SCL_H;
	SHT_SDA_H;
}

/*
**********************************************************************
�������ܣ�iic��ʼ��������
�����βΣ�u8 delay
��������ֵ��None
��ע��ʱ���߸ߵ�ƽ�ڼ䣬�����߲����½���
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
�������ܣ�iicֹͣ��������
�����βΣ�u8 delay
��������ֵ��None
��ע��ʱ���߸ߵ�ƽ�ڼ䣬�����߲���������
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
�������ܣ���������Ӧ���ź�
�����βΣ�u8 ack,u8 delay
��������ֵ��None
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
�������ܣ���������Ӧ���ź�
�����βΣ�u8 delay
��������ֵ�����յ���Ӧ���ź�
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
�������ܣ���������һ���ֽ����ݣ��ٽ���һ��Ӧ���źź���
�����βΣ������͵�����,u8 delay
��������ֵ�����յ���Ӧ���ź�
��ע��0��Ӧ��   1����Ӧ��     8bit   �ȸߺ��
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
�������ܣ���������һ���ֽ����ݣ��ٷ���һ��Ӧ���źź���
�����βΣ������͵�Ӧ��,u8 delay
��������ֵ�����յ�������
��ע��0��Ӧ��   1����Ӧ��     8bit   �ȸߺ��
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

