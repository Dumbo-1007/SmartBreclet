#include "oled.h"
#include "string.h"
/************************
函数功能：OLED屏管脚初始化
形参：无
返回值：无
说明：PB3是SCLK,PB5是SDIN
	  PB13-RES,PB7-CS,PA15-D/C
************************/
void OLED_Pin_Init(void)
{
	//GPIO初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init( GPIOA, &GPIO_InitStruct);
	GPIO_SetBits(GPIOB, GPIO_Pin_13 | GPIO_Pin_7);
}

/*
**********************************************************************
函数功能：spi数据交换的函数(主机发送一个字节数据给从机，并且读取从机发送过来的一个字节数据)
函数形参：主机要发送给从机的数据
函数返回值：主机读到从机数据
备注：数据位8bit  先高后低的顺序

**********************************************************************
*/
u8 Spi_Change_Data(u8 data)
{
	u8 i,buf;
	OLED_SCLK_H;
	for(i = 0; i < 8; i++)
	{
		OLED_SCLK_L;
		if(data & (0x80 >> i))
		{
			OLED_MOSI_H;
		}
		else
		{
			OLED_MOSI_L;
		}
		OLED_SCLK_H;
		if(1)
			buf = 1;
	}
	return buf;
}

//告诉OLED屏发送的是命令
void OLED_Send_Command(u8 cmd)
{
	OLED_CS_LOW;//片选
	OLED_COMMAND;
	Spi_Change_Data(cmd);
	OLED_CS_HIGH;
}

//告诉OLED屏发送的是数据
void OLED_Send_Data(u8 data)
{
	OLED_CS_LOW;//片选
	OLED_DATA;
	Spi_Change_Data(data);
	OLED_CS_HIGH;
}

//清屏
void OLED_Clear(u8 data)
{
	u8 i,j;
	for(i = 0; i < 8; i++)
	{
		OLED_Send_Command(0xB0+i);//从第一页开始
		OLED_Send_Command(0x10);//第一个点
		OLED_Send_Command(0x00);//第一列
		//自动填满一行
		for(j = 0; j < 132; j++)
		{
			OLED_Send_Data(data);
		}
		
	}
}

//OLED初始化配置
void OLED_Config(void)
{
	OLED_RES_HIGH;
	Delay_ms(100);
	OLED_RES_LOW;
	Delay_ms(100);
	OLED_RES_HIGH;
	Delay_ms(100); 
	
	OLED_Send_Command(0xAE);//--turn off oled panel
	OLED_Send_Command(0x02);//---SET low column address
	OLED_Send_Command(0x10);//---SET high column address
	OLED_Send_Command(0x40);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_Send_Command(0x81);//--SET contrast control register
	OLED_Send_Command(0xCF); // SET SEG Output Current Brightness
	OLED_Send_Command(0xA1);//--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_Send_Command(0xC8);//SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_Send_Command(0xA6);//--SET normal display
	OLED_Send_Command(0xA8);//--SET multiplex ratio(1 to 64)
	OLED_Send_Command(0x3f);//--1/64 duty
	OLED_Send_Command(0xD3);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_Send_Command(0x00);//-not offSET
	OLED_Send_Command(0xd5);//--SET display clock divide ratio/oscillator frequency
	OLED_Send_Command(0x80);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_Send_Command(0xD9);//--SET pre-charge period
	OLED_Send_Command(0xF1);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_Send_Command(0xDA);//--SET com pins hardware configuration
	OLED_Send_Command(0x12);
	OLED_Send_Command(0xDB);//--SET vcomh
	OLED_Send_Command(0x40);//SET VCOM Deselect Level
	OLED_Send_Command(0x20);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_Send_Command(0x02);//
	OLED_Send_Command(0x8D);//--SET Charge Pump enable/disable
	OLED_Send_Command(0x14);//--SET(0x10) disable
	OLED_Send_Command(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OLED_Send_Command(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OLED_Send_Command(0xAF);//--turn on oled panel
	
	OLED_Send_Command(0xAF); /*display ON*/ 
	OLED_Clear(0);
}

//OLED初始化
void OLED_Init(void)
{
	OLED_Pin_Init();
	OLED_Config();	
}

//设置显示位置
void Set_Pos(u8 page, u8 col)
{
	OLED_Send_Command(0xB0 | page);//从第一页开始
	OLED_Send_Command(0x10 | (col&0xf0)>>4);//列的高四位
	OLED_Send_Command(0x00 | (col&0xf));//列的低四位
}

//显示单个字符
void Show_Font(u8 page, u8 col, u8 ch)
{
	u8 i,j;
	u8 index;
	index = ch - ' ';
	for(i = 0; i < 2; i++)
	{
		Set_Pos(page+i,col+2);
		for(j = 0; j < 8; j++)
		{
			OLED_Send_Data(Aciss_8X16[index*16+8*i+j]);
		}
	}
}

//显示字符串
void Show_String(u8 page, u8 col, u8 *str)
{
	while(*str != 0)
	{
		Show_Font(page,col,*str);
		str++;
		col += 8;
		if(col > 122)
		{
			col = 0;
			page += 2;
		}
	}
}

//显示图片
u8 Show_Pic(u8 page, u8 col, u8 *pic)
{
	u8 i,j;
	u8 w = pic[2];
	u8 h = pic[4];
	if(w > 132 || (h/8 > 8))
	{
		Show_String(4, 64, (u8 *)"error");
		return 1;
	}
	pic += 6;
	for(i = 0; i < (h/8); i++)
	{
		Set_Pos(page+i,col+2);
		for(j = 0; j < w; j++)
		{
			OLED_Send_Data(*(pic++));
		}
	}
	return 0;
}

//显示文字
void Show_Chinese(u8 page, u8 col, u8 num,u8 *str)
{
	u8 i,j,k;
	for(k = 0; k < num; k++)
	{
		for(i = 0; i < 2; i++)
		{
			Set_Pos(page+i,col+2);
			for(j = 0; j < 16; j++)
			{
				OLED_Send_Data(str[i*16+j+k*32]);
			}
		}
		col += 16;		
		if(col > 114)
		{
			col = 0;
			page += 2;
		}		
	}
}

//滚动显示文字
void String_Roll(u8 page, u8 col, u8 num,u8 *str)
{
	char temp[256] = {0};
	u16 z = 32;
	u8 c;
	u8 len = strlen((const char*)str);
	for(c = 0; c < 32; c++)
	{
		temp[c] = str[c];
	}
	for(c = 0; c < 32; c++)
	{
		temp[32+c] = str[32+c];
	}
	

	while(z < num * 32)
	{
		str[z-32] = str[z];
		for(c = 0; c < 32; c++)
		{
			str[z-32+c] = str[z+c];
		}
		z += 32;
	}
	for(c = 0; c < 32; c++)
	{
		str[z-32+c] = temp[c];
	}
	for(c = 0; c < 32; c++)
	{
		str[z+c] = temp[32+c];
	}
	
	Show_Chinese(page, col, num,str);
}

