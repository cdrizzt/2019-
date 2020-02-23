/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		system_MK60
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/

#include <stdint.h>
#include "common.h"
#include "MK60_port.h"

//-------------------------------------------------------------------------------------------------------------------
//  MK66FX_CLOCK Ϊ 0  �ں�Ƶ�� 180M  ����Ƶ��90M   flex_busƵ��60M    flashƵ��25.7M
//  MK66FX_CLOCK Ϊ 1  �ں�Ƶ�� 200M  ����Ƶ��100M  flex_busƵ��66.6M  flashƵ��28.5M
//  MK66FX_CLOCK Ϊ 2  �ں�Ƶ�� 220M  ����Ƶ��110M  flex_busƵ��73.3M  flashƵ��31.4M
//-------------------------------------------------------------------------------------------------------------------
#define MK66FX_CLOCK   1



void Disable_Wdog(void)
{
    // Disable the WDOG module 
    // WDOG_UNLOCK: WDOGUNLOCK=0xC520 
    WDOG->UNLOCK = (uint16_t)0xC520u;     // Key 1 
    // WDOG_UNLOCK : WDOGUNLOCK=0xD928 
    WDOG->UNLOCK  = (uint16_t)0xD928u;    // Key 2 
    // WDOG_STCTRLH: ??=0,DISTESTWDOG=0,BYTESEL=0,TESTSEL=0,TESTWDOG=0,??=0,STNDBYEN=1,WAITEN=1,STOPEN=1,DBGEN=0,ALLOWUPDATE=1,WINEN=0,IRQRSTEN=0,CLKSRC=1,WDOGEN=0 
    WDOG->STCTRLH = (uint16_t)0x01D2u;
}

void div_set(void)
{
    uint8  temp_c5;
    uint8  temp_c6;
    uint32 temp_clkdiv;
    

    switch(MK66FX_CLOCK)
    {
        case 0:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(36-16);
            
        }break;
        
        case 1:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(40-16);
            
        }break;
        
        case 2:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV0(44-16);
            
        }break;
        
        
        
        default:
        {
            temp_clkdiv = (uint32_t)0x01260000u;
            temp_c5 = MCG_C5_PRDIV0(5-1);
            temp_c6 = MCG_C6_PLLS(1) | MCG_C6_VDIV(36-16);
            
        }break;
        
    }

    SIM->CLKDIV1 = temp_clkdiv;                 
	MCG->C5 = temp_c5;                          
	MCG->C6 = temp_c6;                          
}


void SYSTEM_CLK(void)
{
	uint32 temp_reg;
    
    // Switch to FBE Mode
	SIM->CLKDIV1 = (uint32_t)0xffffffffu;    			// Ԥ��Ƶ�Ĵ������ȶ�����Ϊ1
	//OSC->CR = (uint8_t)0x80u;                           // �����ⲿ����
	MCG->C2 &= ~MCG_C2_LP_MASK;							// �ⲿ��������Դ��ʱ��Ҫ��λMCG_C2_EREFS0_MASK ����������ʹ��A19����
	MCG->C2 |= MCG_C2_RANGE0(2);

	MCG->C1 = (uint8_t)0xBAu;                           // ѡ���ⲿʱ����ΪMCGOUTCLK������Դ��1536��Ƶ��FLL����ԴΪ�ⲿʱ�ӣ�����MCGIRCLK���ڲ��ο�ʱ����ֹͣģʽ�½�ֹ
	MCG->C4 &= (uint8_t)~(uint8_t)0xE0u;
	MCG->C6 &= (uint8_t)( ~(uint8_t)MCG_C6_PLLS_MASK ); // MCG������Դѡ��FLL
	while((MCG->S & MCG_S_IREFST_MASK) != 0u) {  }      // �ȴ���ֱ��FLL��ʱ��Դ���ⲿʱ��
	while((MCG->S & MCG_S_CLKST_MASK) != 0x08u) {  }    // �ȴ���MCGOUTCLK��ʱ��Դ��ѡ��Ϊ�ⲿʱ��
	
    
    //����Ĵ���ֵ
    temp_reg = FMC->PFAPR;

    //����M0PFD��M7PFDΪ1������Ԥȡ����
    FMC->PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
                 | FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
                 | FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;
    
	
    // Switch to PBE Mode 
	div_set();
	//�ָ��Ĵ�����ֵ
    FMC->PFAPR = temp_reg; 
    
	while((MCG->S & MCG_S_PLLST_MASK) == 0u) {  }       // Wait until the source of the PLLS clock has switched to the PLL 
	while((MCG->S & MCG_S_LOCK0_MASK) == 0u) {  }       // Wait until locked 
	
	// Switch to PEE Mode 
	MCG->C1 = (uint8_t)0x1Au;
	while((MCG->S & 0x0Cu) != 0x0Cu) {  }               // Wait until output of the PLL is selected 
	while((MCG->S & MCG_S_LOCK0_MASK) == 0u) {  }       // Wait until locked
}

void Start (void) 
{
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));      // set CP10, CP11 Full Access
    
    Disable_Wdog();
    SYSTEM_CLK();
	gpio_init(A4,GPO,1);  //��ʼ��Ϊ���1���� ������ NMI �ж�
}



void NMI_Handler(void)
{
	
}


