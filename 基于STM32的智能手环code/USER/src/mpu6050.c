#include "mpu6050.h"

/*
****************************************************************
�������ܣ����ֽ�д��ʼ������
�����βΣ�mpu6050�Ĵ�����ַ  д�뵽mpu6050�Ĵ���������
��������ֵ��0:д��ɹ�   ������д��ʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
u8 Mcu_WriteByte_Mpu6050(u8 r_addr, u8 r_dat)
{
	u8 ack = 0;
	//1.��ʼ����
	Send_Start(2);
	
	//2.������ַ+д
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 1;
	
	//3.�Ĵ�����ַ
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 2;
	
	//4.д���Ĵ������������
	ack = I2c_Send_Data( r_dat,2);
	if(ack)  
		return 3;
	
	//5.ֹͣ����
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
�������ܣ����ֽڶ���ʼ������
�����βΣ�mpu6050�Ĵ�����ַ  ��ȡ��mpu6050�Ĵ��������ݵĵ�ַ
��������ֵ��0:��ȡ�ɹ�   ��������ȡʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
u8 Mcu_ReadByte_Mpu6050(u8 r_addr, u8 *r_buf)
{
	u8 ack = 0;
	//1.��ʼ����
	Send_Start(2);
	//2.������ַ+д
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 4;
	//3.�Ĵ�����ַ
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 5;
	//4.��ʼ����
	Send_Start(2);
	//5.������ַ+��
	ack = I2c_Send_Data( MPU6050_ADDR << 1 | 1,2);
	if(ack)  
		return 6;
	//6.������ (����Ӧ��)
	*r_buf =  I2c_Rec_Data(1,2);
	//7.ֹͣ����
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
�������ܣ�����������
�����βΣ�mpu6050�Ĵ�����ַ  ��ȡ��mpu6050�Ĵ��������ݵĻ����ַ  ��ȡ�ֽ�
��������ֵ��0:��ȡ�ɹ�   ��������ȡʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
u8 Mcu_Continue_ReadByte_Mpu6050(u8 r_addr, u8 *r_buf, u8 num)
{
	u8 ack = 0;
	u8 i;
	//1.��ʼ����
	Send_Start(2);
	//2.������ַ+д
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 7;
	//3.�Ĵ�����ַ
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 8;
	//4.��ʼ����
	Send_Start(2);
	//5.������ַ+��
	ack = I2c_Send_Data( MPU6050_ADDR << 1 | 1,2);
	if(ack)  
		return 9;
	//6.������ (����Ӧ��)
	for(i = 0; i < (num-1); i++)
	{
		r_buf[i] =  I2c_Rec_Data(0,2);
	}
	r_buf[i] =  I2c_Rec_Data(1,2);
	//7.ֹͣ����
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
�������ܣ�MCU��д�Ĵ�������
�����βΣ��Ĵ�����ַ  �ڼ�λ��ʼ  ��λ����    д���ֵ
��������ֵ��0:��ȡ�ɹ�   ��������ȡʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
u8 Mcu_Contril_Mpu6050_Register(u8 r_addr, u8 s_bit, u8 bits, u8 dat)
{
	u8 temp = 0;
	u8 ack = 0;
	u8 buf = 0;
	
	ack = Mcu_ReadByte_Mpu6050(r_addr, &temp);//��ȡ����Ҫ�����ļĴ���������
	if(ack) 
		return 10;
	
	//ֻ�ı���Ҫ�ı��λ
	switch(bits)
	{
		case 1: buf = 0x01; break;
		case 2: buf = 0x03; break; 
		case 3: buf = 0x07; break;
		case 4: buf = 0x0f; break; 
		case 5: buf = 0x1f; break;
		case 6: buf = 0x3f; break; 
		case 7: buf = 0x7f; break;
		case 8: buf = 0xff; break; 
	}
	
	temp &= ~(buf << s_bit);//�������
	temp |= (dat << s_bit);//��ֵ����
	
	ack =  Mcu_WriteByte_Mpu6050(r_addr, temp);//�����úõ�����д�뵽�Ĵ�����
	if(ack) 
		return 11;
	
	return 0;
}

/*
****************************************************************
�������ܣ�MPU6050��ʼ������
�����βΣ�None
��������ֵ��0:��ʼ���ɹ�   ��������ʼ��ʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
u8 Mpu6050_Init(void)
{
	u8 ret = 0;
	u8 ack = 0;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 7, 1, 1);//��λMPU6050,��7λ
	if(ack) 
		return 12;
	
	while(1)//�ȴ���λ����
	{
		ack = Mcu_ReadByte_Mpu6050(0x6b, &ret);//�����ݶ�����
		if(ack) 
			return 13;
		
		if((ret & 0x80) == 0)//�жϵ�7λ�Ƿ�Ϊ1
		{
			break;//��λ
		}
	}
	
	Delay_ms(200);
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 6, 1, 0);//����ѭ��ģʽ
	if(ack) 
		return 14;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 0, 3, 1);//ѡ��X ������ʱ��
	if(ack) 
		return 15;
	
	ack = Mcu_Contril_Mpu6050_Register(0x1b, 3, 2, 3);//���� +-2000
	if(ack) 
		return 16;
	
	ack = Mcu_Contril_Mpu6050_Register(0x1c, 3, 2, 0);//���ٶ� +-2g
	if(ack) 
		return 17;
	
	ack = Mcu_Contril_Mpu6050_Register(0x23, 0, 8, 0);//��ֹfifo
	if(ack) 
		return 18;
	
	ack = Mcu_Contril_Mpu6050_Register(0x19, 0, 8, 19);//����Ƶ�ʷ�Ƶ��,20��Ƶ
	if(ack) 
		return 19;
	ack = Mcu_Contril_Mpu6050_Register(0x1a, 0, 3, 4);//����Ƶ��50hz
	if(ack) 
		return 20;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6c, 0, 6, 0);//���������ģʽ
	if(ack) 
		return 21;
	
	ack = Mcu_ReadByte_Mpu6050(0x75, &ret);//���ڱ�ʶ�豸�����
	if(ack) 
		return 22;
	if( ret != 0x68 )
	{
		return 23;
	}
	
	return 0;
}

/*
****************************************************************
�������ܣ���ȡMPU6050������ٶȺ���
�����βΣ������ȡ��������ٶȵĵ�ַ
��������ֵ��0:��ʼ���ɹ�   ��������ʼ��ʧ��
��ע��None
���ڣ�2020��7��3��
����Ա��Tom
�汾��v0.0
****************************************************************
*/
void Mcu_Read_Mpu6050_XYZ(accValue_t *p)
{
	u8 buf[6] = {0};
	
	Mcu_Continue_ReadByte_Mpu6050(0x3b, buf, 6);
	//buf[0]��   buf[1] X��
	//buf[2]��   buf[3] Y��
	//buf[4]��   buf[5] Z��

	p->accX = ((u16)buf[0]<<8)|buf[1]; 
	
	p->accY = ((u16)buf[2]<<8)|buf[3];

	p->accZ = ((u16)buf[4]<<8)|buf[5];

}
