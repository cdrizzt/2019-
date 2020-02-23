/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_rtc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 

#ifndef _MK60_rtc_h
#define _MK60_rtc_h



#include "misc.h"
#include "common.h"
#include "stdlib.h"


#define rtc_alarm_irq_en()      RTC->IER |= RTC_IER_TAIE_MASK      //ʹ�������ж�
#define rtc_alarm_irq_dis()     RTC->IER &= ~RTC_IER_TAIE_MASK     //��ֹ�����ж�


#define rtc_overflow_irq_en()   RTC->IER |= RTC_IER_TOIE_MASK      //ʹ������ж�
#define rtc_overflow_irq_dis()  RTC->IER &= ~RTC_IER_TOIE_MASK     //��ֹ����ж�

#define rtc_invalid_irq_en()    RTC->IER |= RTC_IER_TIIE_MASK      //ʹ����Ч�����ж�
#define rtc_invalid_irq_dis()   RTC->IER &= ~RTC_IER_TIIE_MASK     //��ֹ��Ч�����ж�



void     rtc_init(void);                         //��ʼ��

void     rtc_set_time(uint32 seconds);           //����ʱ��
uint32   rtc_get_time(void);                     //��ȡʱ��

uint8    rtc_set_alarm(uint32 alarm);            //��������
void     rtc_close_alarm(void);                  //�ر�����









#endif
