#include "menu.h"

/*
 * 功能：向左切换菜单显示界面
 * 参数：cnt - 菜单选项计数器(0-4)
 */
void Menu_Left(s8 cnt)
{
    switch(cnt)
    {
        case 0: // 时钟界面
        {   
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)Clock_Pic); // 显示时钟图标
            Show_Chinese(2, 65, 4,clock); // 显示"时钟"文字
            break;
        }
        case 1: // 计步界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)Step_Pic); // 显示计步图标
            Show_Chinese(2, 65, 4,step); // 显示"计步"文字
            break;
        }
        case 2: // 实时数据界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)RT_Picture); // 显示实时数据图标
            Show_Chinese(2, 50, 5,rt); // 显示"实时数据"文字
            break;
        }
        case 3: // 血压测量界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)BM_Picture); // 显示血压测量图标
            Show_Chinese(2, 65, 4,Bm); // 显示"血压测量"文字
            break;
        }
        case 4: // 心率测量界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)HR_Picture); // 显示心率测量图标
            Show_Chinese(2, 65, 4,Hr); // 显示"心率测量"文字
            break;
        }               
    }
}

/*
 * 功能：向右切换菜单显示界面
 * 参数：cnt - 菜单选项计数器(0-4)
 */
void Menu_Right(s8 cnt)
{
    switch(cnt)
    {
        case 0: // 时钟界面
        {   
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)Clock_Pic); // 显示时钟图标
            Show_Chinese(2, 65, 4,clock); // 显示"时钟"文字
            break;
        }
        case 1: // 计步界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)Step_Pic); // 显示计步图标
            Show_Chinese(2, 65, 4,step); // 显示"计步"文字
            break;
        }
        case 2: // 实时数据界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)RT_Picture); // 显示实时数据图标
            Show_Chinese(2, 50, 5,rt); // 显示"实时数据"文字
            break;
        }
        case 3: // 血压测量界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)BM_Picture); // 显示血压测量图标
            Show_Chinese(2, 65, 4,Bm); // 显示"血压测量"文字
            break;
        }
        case 4: // 心率测量界面
        {
            OLED_Clear(0); // 清除OLED显示
            RTC_WakeUpCmd(DISABLE); // 关闭WAKE UP
            Show_Pic(0, 0, (u8*)HR_Picture); // 显示心率测量图标
            Show_Chinese(2, 65, 4,Hr); // 显示"心率测量"文字
            break;
        }               
    }
}

