/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_lptmr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_lptmr.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      lptmr��ʼ��
//  @param      altn            ѡ��ѡ������ģ��
//  @param      count           ����Ƚ�ֵ
//  @param      cfg             ������ʽ
//  @return     void
//  @since      v1.0
//  Sample usage:               lptmr_pulse_init(lpt0_alt2,0xFFFF,LPT_Falling);		//���������������0XFFFF�����жϣ��½��ؼ���
//-------------------------------------------------------------------------------------------------------------------
void lptmr_pulse_init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{
    // ����ģ��ʱ��
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;                   //ʹ��LPTģ��ʱ��
    
    //��������ܽ�    
    if(altn == lpt0_alt1)       port_init(A19, ALT6 | PULLUP);      //��A19��ʹ�� ALT6
    else if(altn == lpt0_alt2)  port_init(C5, ALT3 | PULLUP);       //��C5��ʹ�� ALT4
	

    // ��״̬�Ĵ���
    LPTMR0->CSR = 0x00;                                     //�ȹ���LPT��������������ʱ�ӷ�Ƶ,��ռ���ֵ��

    //ѡ��ʱ��Դ
    LPTMR0->PSR  =  ( 0
                      | LPTMR_PSR_PCS(1)                    //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK                 //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)             //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                     );

    // �����ۼӼ���ֵ
    LPTMR0->CMR  =   LPTMR_CMR_COMPARE(count);              //���ñȽ�ֵ
	
	LPTMR_Flag_Clear();
    // �ܽ����á�ʹ���ж�
    LPTMR0->CSR  =  (0
                     | LPTMR_CSR_TPS(altn)       // ѡ������ܽ� ѡ��
                     | LPTMR_CSR_TMS_MASK        // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                     | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                     | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                     | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                     //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                    );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ص�ǰ����ֵ
//  @return     ���ص�ǰ����ֵ
//  @since      v1.0
//  Sample usage:               uint16 dat = lptmr_pulse_get();		
//-------------------------------------------------------------------------------------------------------------------
uint16 lptmr_pulse_get(void)
{
    uint16 data;
    if(LPTMR0->CSR & LPTMR_CSR_TCF_MASK)	data = 0xFFFF;  //�Ѿ������  ���� 0xffff ��ʾ����
    else       								
    {
        LPTMR0->CNR = 0;//����д��һ��ֵ������ȡ��ȡ
        data = LPTMR0->CNR;
    }
 
	return data;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ֵ
//  @return     void
//  @since      v1.0
//  Sample usage:               lptmr_pulse_clean();	
//-------------------------------------------------------------------------------------------------------------------
void lptmr_pulse_clean(void)
{
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;        //����LPT��ʱ��ͻ��Զ����������ֵ
	LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
}
