#include "mpu6050.h"

/*
****************************************************************
函数功能：单字节写初始化函数
函数形参：mpu6050寄存器地址  写入到mpu6050寄存器的数据
函数返回值：0:写入成功   其它：写入失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
u8 Mcu_WriteByte_Mpu6050(u8 r_addr, u8 r_dat)
{
	u8 ack = 0;
	//1.起始条件
	Send_Start(2);
	
	//2.器件地址+写
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 1;
	
	//3.寄存器地址
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 2;
	
	//4.写到寄存器里面的数据
	ack = I2c_Send_Data( r_dat,2);
	if(ack)  
		return 3;
	
	//5.停止条件
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
函数功能：单字节读初始化函数
函数形参：mpu6050寄存器地址  读取到mpu6050寄存器的数据的地址
函数返回值：0:读取成功   其它：读取失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
u8 Mcu_ReadByte_Mpu6050(u8 r_addr, u8 *r_buf)
{
	u8 ack = 0;
	//1.起始条件
	Send_Start(2);
	//2.器件地址+写
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 4;
	//3.寄存器地址
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 5;
	//4.起始条件
	Send_Start(2);
	//5.器件地址+读
	ack = I2c_Send_Data( MPU6050_ADDR << 1 | 1,2);
	if(ack)  
		return 6;
	//6.读数据 (给非应答)
	*r_buf =  I2c_Rec_Data(1,2);
	//7.停止条件
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
函数功能：连续读函数
函数形参：mpu6050寄存器地址  读取到mpu6050寄存器的数据的缓冲地址  读取字节
函数返回值：0:读取成功   其它：读取失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
u8 Mcu_Continue_ReadByte_Mpu6050(u8 r_addr, u8 *r_buf, u8 num)
{
	u8 ack = 0;
	u8 i;
	//1.起始条件
	Send_Start(2);
	//2.器件地址+写
	ack = I2c_Send_Data( MPU6050_ADDR << 1,2);
	if(ack)  
		return 7;
	//3.寄存器地址
	ack = I2c_Send_Data(r_addr,2);
	if(ack)  
		return 8;
	//4.起始条件
	Send_Start(2);
	//5.器件地址+读
	ack = I2c_Send_Data( MPU6050_ADDR << 1 | 1,2);
	if(ack)  
		return 9;
	//6.读数据 (给非应答)
	for(i = 0; i < (num-1); i++)
	{
		r_buf[i] =  I2c_Rec_Data(0,2);
	}
	r_buf[i] =  I2c_Rec_Data(1,2);
	//7.停止条件
	Send_Stop(2);	
	return 0;
}

/*
****************************************************************
函数功能：MCU读写寄存器函数
函数形参：寄存器地址  第几位开始  几位数据    写入的值
函数返回值：0:读取成功   其它：读取失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
u8 Mcu_Contril_Mpu6050_Register(u8 r_addr, u8 s_bit, u8 bits, u8 dat)
{
	u8 temp = 0;
	u8 ack = 0;
	u8 buf = 0;
	
	ack = Mcu_ReadByte_Mpu6050(r_addr, &temp);//读取到将要操作的寄存器的数据
	if(ack) 
		return 10;
	
	//只改变想要改变的位
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
	
	temp &= ~(buf << s_bit);//清零操作
	temp |= (dat << s_bit);//赋值操作
	
	ack =  Mcu_WriteByte_Mpu6050(r_addr, temp);//把配置好的数据写入到寄存器中
	if(ack) 
		return 11;
	
	return 0;
}

/*
****************************************************************
函数功能：MPU6050初始化函数
函数形参：None
函数返回值：0:初始化成功   其它：初始化失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
u8 Mpu6050_Init(void)
{
	u8 ret = 0;
	u8 ack = 0;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 7, 1, 1);//复位MPU6050,第7位
	if(ack) 
		return 12;
	
	while(1)//等待复位结束
	{
		ack = Mcu_ReadByte_Mpu6050(0x6b, &ret);//将数据读出来
		if(ack) 
			return 13;
		
		if((ret & 0x80) == 0)//判断第7位是否为1
		{
			break;//复位
		}
	}
	
	Delay_ms(200);
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 6, 1, 0);//进入循环模式
	if(ack) 
		return 14;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6b, 0, 3, 1);//选择X 轴陀螺时钟
	if(ack) 
		return 15;
	
	ack = Mcu_Contril_Mpu6050_Register(0x1b, 3, 2, 3);//陀螺 +-2000
	if(ack) 
		return 16;
	
	ack = Mcu_Contril_Mpu6050_Register(0x1c, 3, 2, 0);//加速度 +-2g
	if(ack) 
		return 17;
	
	ack = Mcu_Contril_Mpu6050_Register(0x23, 0, 8, 0);//禁止fifo
	if(ack) 
		return 18;
	
	ack = Mcu_Contril_Mpu6050_Register(0x19, 0, 8, 19);//采样频率分频器,20分频
	if(ack) 
		return 19;
	ack = Mcu_Contril_Mpu6050_Register(0x1a, 0, 3, 4);//采样频率50hz
	if(ack) 
		return 20;
	
	ack = Mcu_Contril_Mpu6050_Register(0x6c, 0, 6, 0);//不进入待机模式
	if(ack) 
		return 21;
	
	ack = Mcu_ReadByte_Mpu6050(0x75, &ret);//用于标识设备的身份
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
函数功能：读取MPU6050三轴加速度函数
函数形参：保存读取到三轴加速度的地址
函数返回值：0:初始化成功   其它：初始化失败
备注：None
日期：2020年7月3日
程序员：Tom
版本：v0.0
****************************************************************
*/
void Mcu_Read_Mpu6050_XYZ(accValue_t *p)
{
	u8 buf[6] = {0};
	
	Mcu_Continue_ReadByte_Mpu6050(0x3b, buf, 6);
	//buf[0]高   buf[1] X低
	//buf[2]高   buf[3] Y低
	//buf[4]高   buf[5] Z低

	p->accX = ((u16)buf[0]<<8)|buf[1]; 
	
	p->accY = ((u16)buf[2]<<8)|buf[3];

	p->accZ = ((u16)buf[4]<<8)|buf[5];

}
