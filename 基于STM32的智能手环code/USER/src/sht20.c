#include "sht20.h"

/*
***************************
函数功能：Sht20初始化
函数参数：None
函数返回值：None
函数说明：PB8-SCL 推挽输出
		  PB9-SDA 开漏输出
***************************
*/
void Sht20_Init(void)
{
	I2c_Init(GPIO_Pin_8,GPIO_Pin_9);
	Delay_ms(15);
	
	//软复位
	Send_Start(2);
	I2c_Send_Data(WRITE_ADDRESS,2);
	I2c_Send_Data(0xfe,2);
	Send_Stop(2);
}

//设置分辨率
void Set_Resolution(void)
{
	Send_Start(2);
	I2c_Send_Data(WRITE_ADDRESS,2);
	I2c_Send_Data(0xE6,2);//写用户寄存器
	I2c_Send_Data(0x03,2);//11位
	Send_Stop(2);
}

//测量函数
float Sht20_Mesaure(u8 cmd)
{
	u8 ack = 0;
	u8 LSB,MSB;
	u16 data;
	Set_Resolution();
	Send_Start(2);
	I2c_Send_Data(WRITE_ADDRESS,2);
	I2c_Send_Data(cmd,2);
	do
	{
		Send_Start(2);
		ack = I2c_Send_Data(READ_ADDRESS,2);
		Delay_ms(6);
	}while(ack);
	MSB = I2c_Rec_Data(0,2);
	LSB = I2c_Rec_Data(0,2);
	I2c_Rec_Data(1,2);
	Send_Stop(2);
	data = MSB << 8 | (LSB &0XFC);
	if(cmd ==  HUMIDITY)
	{
		return (((float)125 * data/65536 - 6));
	}
	else if(cmd ==TEMPEATURE)
	{
		return ((float)(175.72 * data/65536 - 46.85));
	}
	return 0;
}

