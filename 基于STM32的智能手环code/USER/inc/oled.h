#ifndef OLED_H_
#define OLED_H_

#include "stm32f4xx.h"
#include "tim2.h"
#include "asi.h"
#include "bmp.h"

#define OLED_RES_LOW 	GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define OLED_RES_HIGH 	GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define OLED_CS_LOW 	GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define OLED_CS_HIGH 	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define OLED_COMMAND 	GPIO_ResetBits(GPIOA, GPIO_Pin_15)
#define OLED_DATA 		GPIO_SetBits(GPIOA, GPIO_Pin_15)
#define OLED_SCLK_H		GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define OLED_SCLK_L		GPIO_ResetBits(GPIOB, GPIO_Pin_3)
#define OLED_MOSI_H		GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define OLED_MOSI_L		GPIO_ResetBits(GPIOB, GPIO_Pin_5)

void OLED_Init(void);
void OLED_Clear(u8 data);
void Show_Font(u8 page, u8 col, u8 ch);
void Show_String(u8 page, u8 col, u8 *str);
u8 Show_Pic(u8 page, u8 col, u8 *pic);
void Show_Chinese(u8 page, u8 col, u8 num,u8 *str);
void String_Roll(u8 page, u8 col, u8 num,u8 *str);
#endif

