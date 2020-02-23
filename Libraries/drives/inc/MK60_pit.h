/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_pit
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_pit_h
#define _MK60_pit_h

#include "misc.h"
#include "common.h"




//����PITģ���
typedef enum
{
    pit0,
    pit1,
    pit2,
    pit3,
} PITn;

#define     PIT_FlAG_CLR(PITn)          PIT->CHANNEL[PITn].TFLG |= PIT_TFLG_TIF_MASK        //���жϱ�־��д1 ��ձ�־λ��


void pit_init(PITn pitn, uint32 cnt);
void pit_delay(PITn pitn, uint32 cnt);
void pit_time_start(PITn pitn);
uint32 pit_time_get(PITn pitn);
void pit_close(PITn pitn);

//------------------------------------���´�������PIT�ж�------------------------------------
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * bus_clk_mhz*1000);          //��ʼ��PITn�������ö�ʱʱ��(��λΪ ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * bus_clk_mhz);               //��ʼ��PITn�������ö�ʱʱ��(��λΪ us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * bus_clk_mhz/1000);          //��ʼ��PITn�������ö�ʱʱ��(��λΪ ns)


//------------------------------------���´�������PIT��ʱ------------------------------------
#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * bus_clk_mhz*1000);        //PIT��ʱ ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * bus_clk_mhz);             //PIT��ʱ us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * bus_clk_mhz/1000);        //PIT��ʱ ns(100M����Ƶ��ʱ��С��λΪ10ns)


#endif
