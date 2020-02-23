/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_wdog
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
#ifndef _MK60_wdog_h
#define _MK60_wdog_h

#include "misc.h"
#include "common.h"
#include "stdlib.h"


//wdog ���룬���� LPO ʱ�ӣ�����Ƶ��

void wdog_init_ms(uint32 ms);   //��ʼ�����Ź�������ι��ʱ�� ms
void wdog_feed(void);           //ι��


void wdog_disable(void);        //���ÿ��Ź�
void wdog_enable(void);         //���ÿ��Ź�

#endif
