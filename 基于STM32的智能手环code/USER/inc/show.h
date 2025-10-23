#ifndef __SHOW_H
#define __SHOW_H
#include "stm32f4xx.h"
#include "includes.h"

extern int key_enter;
extern int key_ok; 
extern u8 setclk_flag;
extern u8 rate_flag;
extern u8 Bm_flag;

void show_tim(void);
void show_TH(void);
void show_rate(void);
void show_press(void);
void show_step(void);
void show_menu(void);
void show_setclock(void);
void show_curclock(void);
void kaiji_gif(void);
void end_work(void);

#endif


