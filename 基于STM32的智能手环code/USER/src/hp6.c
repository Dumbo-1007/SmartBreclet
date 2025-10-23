#include "hp6.h"

uint8_t tx_buf[24] = {0};//�������ݰ�
uint8_t rx_buf[24] = {0};//�������ݰ�

/*crcУ���*/
const uint16_t crc16_tab[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,   
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,   
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,   
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,   
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,   
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,   
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,   
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,   
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,   
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,   
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,   
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,   
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,   
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,   
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,   
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,   
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,  
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,   
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,   
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,   
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,   
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,   
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,   
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,   
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,   
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,   
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,   
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,   
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,   
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,   
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,   
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};

//����Ѫѹ����
const uint8_t bm_open[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};


//�ر�Ѫѹ����
const uint8_t bm_close[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//��ȡѪѹ�������
const uint8_t bm_result[]=
{0xc8,0xd7,0xb6,0xa5,0x90,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//�������ʲ���
const uint8_t cmd_rate_open[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x01,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//�ر����ʲ���
const uint8_t cmd_rate_close[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//��ȡ���ʲ������
const uint8_t cmd_rate_result[]=
{0xc8,0xd7,0xb6,0xa5,0xD0,0x02,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

/*
***************************
�������ܣ�HP6��ʼ��
����������None
��������ֵ��None
����˵����PA2-SCL �������
		  PA1-SDA ��©���
		  PC13-EN �������
***************************
*/
void Hp6_Pin_Init(void)
{
	//��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//�������
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//��©���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//ʹ��λ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HP_SCL_H;
	HP_SDA_H;
	HP_ON;
}

/*
**********************************************************************
�������ܣ�iic��ʼ��������
�����βΣ�u8 delay
��������ֵ��None
��ע��ʱ���߸ߵ�ƽ�ڼ䣬�����߲����½���
**********************************************************************
*/
void HP_Send_Start(u8 delay)
{
	Delay_us(delay);
	HP_SCL_H;
	HP_SDA_H;
	Delay_us(delay);
	HP_SDA_L;
	Delay_us(delay);
	HP_SCL_L;
}

/*
**********************************************************************
�������ܣ�iicֹͣ��������
�����βΣ�u8 delay
��������ֵ��None
��ע��ʱ���߸ߵ�ƽ�ڼ䣬�����߲���������
**********************************************************************
*/
void HP_Send_Stop(u8 delay)
{
	HP_SDA_L;
	Delay_us(delay);
	HP_SCL_H;
	Delay_us(delay);
	HP_SDA_H;
	Delay_us(delay);
}

/*
**********************************************************************
�������ܣ���������Ӧ���ź�
�����βΣ�u8 ack,u8 delay
��������ֵ��None
**********************************************************************
*/
void HP_Mcu_Send_Ask(u8 ack,u8 delay)
{
	HP_SCL_L;
	if(ack)
		HP_SDA_H;
	else
		HP_SDA_L;
	Delay_us(delay);
	HP_SCL_H;
	Delay_us(delay);
	HP_SCL_L;
}

/*
**********************************************************************
�������ܣ���������Ӧ���ź�
�����βΣ�u8 delay
��������ֵ�����յ���Ӧ���ź�
**********************************************************************
*/
u8 HP_Mcu_Rec_Ask(u8 delay)
{
	u8 ack = 0;
	HP_SDA_H;//�Ͽ�io������������Ϣ
	HP_SCL_L;
	Delay_us(delay);
	HP_SCL_H;
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		ack = 1;
	Delay_us(delay);
	HP_SCL_L;
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
u8 HP_I2c_Send_Data(u8 data,u8 delay)
{
	u8 i,ack;
	for(i = 0; i < 8; i++)
	{
		HP_SCL_L;
		if(data & (0x80 >> i))
			HP_SDA_H;
		else
			HP_SDA_L;
		Delay_us(delay);
		HP_SCL_H;
		Delay_us(delay);
	}
	HP_SCL_L;
	ack = HP_Mcu_Rec_Ask(delay);
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
u8 HP_I2c_Rec_Data(u8 ack,u8 delay)
{
	u8 i;
	u8 buf = 0;
	HP_SDA_H;//�Ͽ�io������������Ϣ
	for(i = 0; i<8; i++)
	{
		HP_SCL_L;
		Delay_us(delay);
		HP_SCL_H;
		buf <<= 1;
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1))
		{
			buf |= 1;
		}
		Delay_us(delay);	
	}
	HP_SCL_H;
	HP_Mcu_Send_Ask(ack,delay);
	return buf;
}

//CRCУ��
uint16_t Crc16(uint8_t *data,uint16_t len)
{
	uint16_t crc16 = 0xFFFF;
	uint32_t uIndex ; 
	while (len--)
	{
		uIndex = (crc16&0xff) ^ ((*data) & 0xff) ; 
		data = data + 1;
		crc16 = ((crc16>>8) & 0xff) ^ crc16_tab[uIndex];
	}
	return crc16 ;
}

//HP6����
void HP_Send_Cmd_Config(u8 *cmd)
{
	u8 i;
	HP_Send_Start(5);
	HP_I2c_Send_Data(HP_WRITE_ADDRESS,10);
	for(i = 0; i < 24; i++)
	{
		HP_I2c_Send_Data(*(cmd+i),10);
	}
	HP_Send_Stop(5);
}

//HP6����
void HP_Rec_Cmd_Config(u8 *data)
{
	u8 i;
	HP_Send_Start(5);
	HP_I2c_Send_Data(HP_READ_ADDRESS,10);
	for(i = 0; i < 23; i++)
	{
		*(data+i) = HP_I2c_Rec_Data(0,10);
	}
	*(data+23) = HP_I2c_Rec_Data(1,10);
	HP_Send_Stop(5);
}

//HP6��������
u8 HP_Send_Cmd(u8 *tx_buf, u8*rx_buf)
{
	u16 crc;
	u16 check_sum = 0;
	crc = Crc16(&tx_buf[4],18);//����У��
	*(uint16_t*)(&tx_buf[22]) = crc;
	HP_Send_Cmd_Config(tx_buf);//��������
	Delay_ms(5);
	HP_Rec_Cmd_Config(rx_buf);
	crc = *(uint16_t*)(&rx_buf[22]); //У��ȷ�����������Ƿ�Ϊ��Ч����
	check_sum = Crc16(&rx_buf[4],18);
	if(crc != check_sum)
	{
		return 1;
	}
	return 0;
}

//����Ѫѹ����
u8 Bm_Open(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		rx_buf[i] = 0;
		tx_buf[i] = 0;
	}
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = bm_open[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
}

//�õ�Ѫѹ��������
u8 Get_Bm(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		rx_buf[i] = 0;
		tx_buf[i] = 0;
	}
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = bm_result[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
	
}

//�ر�Ѫѹ����
u8 Bm_Close(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = bm_close[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
}

//�������ʲ���
u8 Hr_Open(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		rx_buf[i] = 0;
		tx_buf[i] = 0;
	}
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = cmd_rate_open[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
}

//�õ����ʲ�������
u8 Get_Hr(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		rx_buf[i] = 0;
		tx_buf[i] = 0;
	}
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = cmd_rate_result[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
}

//�ر����ʲ���
u8 Hr_Close(void)
{
	u8 i;
	for(i = 0; i < 24; i++)
	{
		tx_buf[i] = cmd_rate_close[i];
	}
	return HP_Send_Cmd(tx_buf, rx_buf);
}


//�õ��������
void Get_Result(u8 *data)
{
	uint8_t i = 0;

	for(i=0; i < 24; i++)
	{
		data[i] = rx_buf[i];
	}
  

}
