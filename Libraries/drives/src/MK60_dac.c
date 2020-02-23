/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_dac
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_dac.h"

DAC_Type * const DACX[2] = DAC_BASE_PTRS; //��������ָ�����鱣�� DACN �ĵ�ַ

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      dacn         ѡ��DACͨ��
//  @param      volta        ��Ҫ����ĵ�ѹ
//  @return     void
//  @since      v1.0
//  Sample usage:               dac_init(dac0,1000);//�����ѹ����(1000/(1<<12)*3.3)V
//-------------------------------------------------------------------------------------------------------------------
void dac_init(DACn dacn,uint16 volta)
{
    volta &= 0x0FFF; 
    
    // ʹ��ʱ�� 
    SIM->SCGC2 |= (SIM_SCGC2_DAC0_MASK << dacn) ;    //ʹ��DACģ��
    
    //  ����DAC�Ĵ���  

    //����DAC_C0�Ĵ���
    DACX[dacn]->C0  = ( 0
                        |  DAC_C0_DACTRGSEL_MASK                //ѡ���������
                        |  DAC_C0_DACRFS_MASK                   //ѡ��ο�VDD��ѹ(3.3V)
                        |  DAC_C0_DACEN_MASK                    //ʹ��DACģ��
                      );

    //����DAC_C1�Ĵ���
    DACX[dacn]->C1 = ( 0 ) ;
    //����DAC_C2�Ĵ���
    DACX[dacn]->C2 = ( 0
                       | DAC_C2_DACBFRP(0)             //���û�������ָ��ָ��0
                     );

    DACX[dacn]->DAT[0].DATH = volta>>8;   //Ĭ�������͵�ѹ
    DACX[dacn]->DAT[0].DATL = (uint8)volta;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ı������ѹ
//  @param      dacn         ѡ��DACͨ��
//  @param      volta        ��Ҫ����ĵ�ѹ
//  @return     void
//  @since      v1.0
//  Sample usage:               dac_out(dac0,1000);//�����ѹ����(1000/(1<<12)*3.3)V
//-------------------------------------------------------------------------------------------------------------------
void dac_out(DACn dacn, uint16 volta)
{
    volta &= 0x0FFF; 
    DACX[dacn]->DAT[0].DATH = volta>>8;   //Ĭ�������͵�ѹ
    DACX[dacn]->DAT[0].DATL = (uint8)volta;
}









