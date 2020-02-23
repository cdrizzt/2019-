/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_ftm
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_ftm_h
#define _MK60_ftm_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"


typedef enum
{
    ftm0,
    ftm1,
    ftm2,
    ftm3,
} FTMn;


typedef enum
{
    ftm_ch0,
    ftm_ch1,
    ftm_ch2,
    ftm_ch3,
    ftm_ch4,
    ftm_ch5,
    ftm_ch6,
    ftm_ch7,
} FTM_CHn;

//------------------------ PWM ------------------------

#define FTM0_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM1_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM2_PRECISON 1000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON
#define FTM3_PRECISON 10000u     //����ռ�ձȾ��ȣ���˼��ʹ��һ�����ڷֳɶ��ٷ֣��ֶ�Խ����������Խ�ߣ�����ռ�ձȵ�ʱ��duty�ķ�Χ����0-FTM0_PRECISON

//------------------------ ���벶׽ ------------------------
//FTM ���벶׽����
typedef enum
{
    FTM_Rising=1,               //�����ز�׽
    FTM_Falling=2,              //�½��ز�׽
    FTM_Rising_or_Falling=3     //�����ز�׽
} FTM_Input_cfg;


//------------------------ ���벶׽�жϱ�־λ��� ------------------------
#define FTM0_INPUT_FLAG_CLR(FTM_CHn)    FTM0->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM1_INPUT_FLAG_CLR(FTM_CHn)    FTM1->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM2_INPUT_FLAG_CLR(FTM_CHn)    FTM2->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;
#define FTM3_INPUT_FLAG_CLR(FTM_CHn)    FTM3->CONTROLS[FTM_CHn].CnSC &= ~FTM_CnSC_CHF_MASK;



//------------------------ ��ȡ���벶׽ͨ�����жϱ�־λ ------------------------
//�Ĵ����ĵͰ�λ�������Ű˸�ͨ�����жϱ�־λ���üĴ���ֻ�ܶ�����д�����λ��Ӧͨ��0
#define FTM0_INPUT_FLAG_READ            FTM0->STATUS
#define FTM1_INPUT_FLAG_READ            FTM1->STATUS
#define FTM2_INPUT_FLAG_READ            FTM2->STATUS
#define FTM3_INPUT_FLAG_READ            FTM3->STATUS


void ftm_pwm_init(FTMn ftmn, FTM_CHn ch, uint32 freq, uint32 duty);
void ftm_pwm_duty(FTMn ftmn, FTM_CHn ch, uint32 duty);
void ftm_pwm_freq(FTMn ftmn, uint32 freq);
void ftm_input_init(FTMn ftmn, FTM_CHn ch, FTM_Input_cfg cfg,uint32 freq_max);
uint16 ftm_input_get (FTMn ftmn, FTM_CHn ch);
void ftm_input_clean (FTMn ftmn);
void ftm_quad_init(FTMn ftmn);
int16 ftm_quad_get(FTMn ftmn);
void ftm_quad_clean(FTMn ftmn);

#endif
