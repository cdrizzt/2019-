/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_adc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_adc_h
#define _MK60_adc_h


#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"

typedef enum
{
    // ---------------------------------ADC0-------------------------
    ADC0_DP0,
    ADC0_DP1,
    PGA0_DP,        //��֧��ADC����Ҫ���� ADC PGA register ���÷Ŵ�����
    ADC0_DP3,
    //�������Bͨ���ǲ���ʹ�õģ������Ҫʹ���ҵ�adc_start���� ���ADC_CFG2_MUXSEL_MASKע��    ����������ֻ��ʹ��Bͨ����
	//���鲻Ҫʹ��Bͨ������ΪAͨ������ԶԶ����Bͨ��
    ADC0_SE4b,      // C2     
    ADC0_SE5b,      // D1     
    ADC0_SE6b,      // D5     
    ADC0_SE7b,      // D6     

    ADC0_SE8,       // B0
    ADC0_SE9,       // B1
    ADC0_SE10,      // A7
    ADC0_SE11,      // A8
    ADC0_SE12,      // B2
    ADC0_SE13,      // B3
    ADC0_SE14,      // C0
    ADC0_SE15,      // C1
	
    ADC0_SE16,      // ADC0_SE16
	
    ADC0_SE17,      // E24
    ADC0_SE18,      // E25
	
    ADC0_DM0,       // ADC0_DM0
    ADC0_DM1,       // ADC0_DM1
    RES0,           // ����
    RES1,           // ����
    DAC0_OUT,       // DAC0��� ��֧��ADC
    RES2,           // ����
    RES3,           // ����
    Temp0_Sensor,   // Temperature Sensor,�ڲ��¶Ȳ���������ADC����
    Bandgap0,       // �¶Ȳ����ṹ��϶��׼Դ   ��֧��ADC
    RES4,           // ����
    VREFH0,         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    VREFL0,         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
    Module0_Dis,    // ��֧�� ADC

    // ---------------------------------ADC1-------------------------
    ADC1_DP0,
    ADC1_DP1,
    PGA1_DP,        // ��֧�� ADC
    ADC1_DP3,
    ADC1_SE4a,      // E0
    ADC1_SE5a,      // E1
    ADC1_SE6a,      // E2
    ADC1_SE7a,      // E3
	
	//�������Bͨ���ǲ���ʹ�õģ������Ҫʹ���ҵ�adc_start���� ���ADC_CFG2_MUXSEL_MASKע��    ����������ֻ��ʹ��Bͨ����
	//���鲻Ҫʹ��Bͨ������ΪAͨ������ԶԶ����Bͨ��
    ADC1_SE4b = ADC1_SE4a,  // C8     
    ADC1_SE5b = ADC1_SE5a,  // C9     
    ADC1_SE6b = ADC1_SE6a,  // C10    
    ADC1_SE7b = ADC1_SE7a,  // C11    

    ADC1_SE8,       // B0
    ADC1_SE9,       // B1
    ADC1_SE10,      // B4
    ADC1_SE11,      // B5
    ADC1_SE12,      // B6
    ADC1_SE13,      // B7
    ADC1_SE14,      // B10
    ADC1_SE15,      // B11
    ADC1_SE16,      // ADC1_SE16
    ADC1_SE17,      // A17
    VREF_OUTPUT,    // VREF Output
    ADC1_DM0,       // ADC1_DM0
    ADC1_DM1,       // ADC1_DM1
    RES5,           //����
    RES6,
    DAC1_OUT,
    RES7,           //����
    RES8,
    Temp1_Sensor,
    Bandgap1,       // �¶Ȳ����ṹ��϶��׼Դ   ��֧��ADC
    RES9,
    VREFH1,         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    VREFL1,         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
    Module1_Dis,    // ��֧�� ADC

} ADCn_Ch;

typedef enum  //ADCģ��
{
    adc0,
    adc1
} ADCn;

//�ֱ���
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


void adc_init(ADCn_Ch adcn_ch);
uint16 adc_once(ADCn_Ch adcn_ch, ADC_nbit bit);
void adc_stop(ADCn adcn);













#endif
