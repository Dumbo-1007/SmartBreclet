#include "adc.h"

u16 ADC_value;
/************************
函数功能：ADC初始化
形参：无
返回值：无
************************/
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	DMA_InitTypeDef DMA_InitStructure; 
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	//开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//模拟输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//DMA初始化（帮助你搬运数据）
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //使能ADC1时钟
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(ADC1->DR)); 
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&ADC_value; 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory  ; 
	DMA_InitStructure.DMA_BufferSize = 1; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*单次传输*/
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*单次传输*/

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	//ADC初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode =  ADC_DMAAccessMode_1; //DMA使能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=100/4=25Mhz,ADC时钟最好不要超过36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//单次模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位的分辨率
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//不扫描
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_480Cycles );//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度		480+12 
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);  
	ADC_SoftwareStartConv(ADC1) ;//软件触发规则组转换 

}

