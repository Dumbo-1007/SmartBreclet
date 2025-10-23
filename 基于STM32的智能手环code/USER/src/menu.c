#include "menu.h"

/*
 * ���ܣ������л��˵���ʾ����
 * ������cnt - �˵�ѡ�������(0-4)
 */
void Menu_Left(s8 cnt)
{
    switch(cnt)
    {
        case 0: // ʱ�ӽ���
        {   
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)Clock_Pic); // ��ʾʱ��ͼ��
            Show_Chinese(2, 65, 4,clock); // ��ʾ"ʱ��"����
            break;
        }
        case 1: // �Ʋ�����
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)Step_Pic); // ��ʾ�Ʋ�ͼ��
            Show_Chinese(2, 65, 4,step); // ��ʾ"�Ʋ�"����
            break;
        }
        case 2: // ʵʱ���ݽ���
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)RT_Picture); // ��ʾʵʱ����ͼ��
            Show_Chinese(2, 50, 5,rt); // ��ʾ"ʵʱ����"����
            break;
        }
        case 3: // Ѫѹ��������
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)BM_Picture); // ��ʾѪѹ����ͼ��
            Show_Chinese(2, 65, 4,Bm); // ��ʾ"Ѫѹ����"����
            break;
        }
        case 4: // ���ʲ�������
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)HR_Picture); // ��ʾ���ʲ���ͼ��
            Show_Chinese(2, 65, 4,Hr); // ��ʾ"���ʲ���"����
            break;
        }               
    }
}

/*
 * ���ܣ������л��˵���ʾ����
 * ������cnt - �˵�ѡ�������(0-4)
 */
void Menu_Right(s8 cnt)
{
    switch(cnt)
    {
        case 0: // ʱ�ӽ���
        {   
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)Clock_Pic); // ��ʾʱ��ͼ��
            Show_Chinese(2, 65, 4,clock); // ��ʾ"ʱ��"����
            break;
        }
        case 1: // �Ʋ�����
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)Step_Pic); // ��ʾ�Ʋ�ͼ��
            Show_Chinese(2, 65, 4,step); // ��ʾ"�Ʋ�"����
            break;
        }
        case 2: // ʵʱ���ݽ���
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)RT_Picture); // ��ʾʵʱ����ͼ��
            Show_Chinese(2, 50, 5,rt); // ��ʾ"ʵʱ����"����
            break;
        }
        case 3: // Ѫѹ��������
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)BM_Picture); // ��ʾѪѹ����ͼ��
            Show_Chinese(2, 65, 4,Bm); // ��ʾ"Ѫѹ����"����
            break;
        }
        case 4: // ���ʲ�������
        {
            OLED_Clear(0); // ���OLED��ʾ
            RTC_WakeUpCmd(DISABLE); // �ر�WAKE UP
            Show_Pic(0, 0, (u8*)HR_Picture); // ��ʾ���ʲ���ͼ��
            Show_Chinese(2, 65, 4,Hr); // ��ʾ"���ʲ���"����
            break;
        }               
    }
}

