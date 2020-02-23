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
 
 
 
#include "MK60_adc.h"

ADC_Type * const ADCX[] = ADC_BASE_PTRS; //��������ָ�����鱣�� ADCN �ĵ�ַ




void adc_start(ADCn_Ch adcn_ch, ADC_nbit bit)
{
    ADCn adcn = (ADCn)(adcn_ch >> 5) ;
    uint8 ch = (uint8)(adcn_ch & 0x1F);

    //��ʼ��ADCĬ������
    ADCX[adcn]->CFG1 = (0
                        //| ADC_CFG1_ADLPC_MASK         //ADC�������ã�0Ϊ�������ģ�1Ϊ�͹���
                        | ADC_CFG1_ADIV(2)              //ʱ�ӷ�Ƶѡ��,��Ƶϵ��Ϊ 2^n,2bit
                        | ADC_CFG1_ADLSMP_MASK          //����ʱ�����ã�0Ϊ�̲���ʱ�䣬1 Ϊ������ʱ��
                        | ADC_CFG1_MODE(bit)
                        | ADC_CFG1_ADICLK(0)            //0Ϊ����ʱ��,1Ϊ����ʱ��/2,2Ϊ����ʱ�ӣ�ALTCLK����3Ϊ �첽ʱ�ӣ�ADACK����
                       );


    ADCX[adcn]->CFG2  = (0
                         //| ADC_CFG2_MUXSEL_MASK       	//ADC����ѡ��,0Ϊaͨ����1Ϊbͨ����
                         //| ADC_CFG2_ADACKEN_MASK      //�첽ʱ�����ʹ��,0Ϊ��ֹ��1Ϊʹ�ܡ�
                         | ADC_CFG2_ADHSC_MASK          //��������,0Ϊ����ת�����У�1Ϊ����ת������
                         | ADC_CFG2_ADLSTS(0)           //������ʱ��ѡ��ADCKΪ4+n������ѭ��������ѭ����0Ϊ20��1Ϊ12��2Ϊ6��3Ϊ2
                        );

    //д�� SC1A ����ת��
    ADCX[adcn]->SC1[0] = (0
                          | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                          //| ADC_SC1_DIFF_MASK        // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                          | ADC_SC1_ADCH( ch )
                         );

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC��ʼ��
//  @param      adcn_ch         ѡ��ADCͨ��
//  @return     void
//  @since      v1.0
//  Sample usage:               adc_init(ADC0_SE8);
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCn_Ch adcn_ch)
{
    uint8 adcn = adcn_ch >> 5 ;

    switch(adcn)
    {
    case adc0:       //   ADC0
        SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK );        //����ADC0ʱ��
        SIM->SOPT7 &= ~(SIM_SOPT7_ADC0ALTTRGEN_MASK  | SIM_SOPT7_ADC0PRETRGSEL_MASK);
        SIM->SOPT7 |= SIM_SOPT7_ADC0TRGSEL(0);
        break;
    case adc1:       //   ADC1
        SIM->SCGC3 |= (SIM_SCGC3_ADC1_MASK );
        SIM->SOPT7 &= ~(SIM_SOPT7_ADC1ALTTRGEN_MASK  | SIM_SOPT7_ADC1PRETRGSEL_MASK) ;
        SIM->SOPT7 |= SIM_SOPT7_ADC1TRGSEL(0);
        break;
    default:
        break;
    }

    switch(adcn_ch)
    {
    case ADC0_SE8:       // B0
        port_init(B0, ALT0);
        break;
    case ADC0_SE9:       // B1
        port_init(B1, ALT0);
        break;
    case ADC0_SE10:      // A7
        port_init(A7, ALT0);
        break;
    case ADC0_SE11:      // A8
        port_init(A8, ALT0);
        break;
    case ADC0_SE12:      // B2
        port_init(B2, ALT0);
        break;
    case ADC0_SE13:      // B3
        port_init(B3, ALT0);
        break;
    case ADC0_SE14:      // C0
        port_init(C0, ALT0);
        break;
    case ADC0_SE15:      // C1
        port_init(C1, ALT0);
        break;
    case ADC0_SE17:      // E24
        port_init(E24, ALT0);
        break;
    case ADC0_SE18:      // E25
        port_init(E25, ALT0);
        break;

    case ADC0_DP0:
    case ADC0_DP1:
    case ADC0_DP3:
    case ADC0_DM0:       // ADC0_DM0
    case ADC0_DM1:       // ADC0_DM1
    case ADC0_SE16:      // ADC0_SE16
    case Temp0_Sensor:   // Temperature Sensor,�ڲ��¶Ȳ���������ADC����
    case VREFH0:         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    case VREFL0:         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
        break;          //�ⲿ�ֹܽŲ������ø���

        // ---------------------------------ADC1-------------------------
    case ADC1_DP0:
    case ADC1_DP1:
    case ADC1_DP3:
        break;
    case ADC1_SE4a:      // E0
        port_init(E0, ALT0);
        break;
    case ADC1_SE5a:      // E1
        port_init(E1, ALT0);
        break;
    case ADC1_SE6a:      // E2
        port_init(E2, ALT0);
        break;
    case ADC1_SE7a:      // E3
        port_init(E3, ALT0);
        break;

    case ADC1_SE8:       // B0
        port_init(B0, ALT0);
        break;
    case ADC1_SE9:       // B1
        port_init(B1, ALT0);
        break;
    case ADC1_SE10:      // B4
        port_init(B4, ALT0);
        break;
    case ADC1_SE11:      // B5
        port_init(B5, ALT0);
        break;
    case ADC1_SE12:      // B6
        port_init(B6, ALT0);
        break;
    case ADC1_SE13:      // B7
        port_init(B7, ALT0);
        break;
    case ADC1_SE14:      // B10
        port_init(B10, ALT0);
        break;
    case ADC1_SE15:      // B11
        port_init(B11, ALT0);
        break;
    case ADC1_SE17:      // A17
        port_init(A17, ALT0);
        break;

    case ADC1_SE16:      // ADC1_SE16
    case VREF_OUTPUT:    // VREF Output
    case ADC1_DM0:       // ADC1_DM0
    case ADC1_DM1:       // ADC1_DM1
    case Temp1_Sensor:
    case VREFH1:         // �ο��ߵ�ѹ,����ADC���� ,�����Ϊ 2^n-1
    case VREFL1:         // �ο��͵�ѹ,����ADC���� ,�����Ϊ 0
        break;

    default:
        break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @param      bit             ѡ��ֱ���ADC_8bit��ADC_10bit��ADC_12bit��ADC_16bit
//  @return     void
//  @since      v1.0
//  Sample usage:               uint16 dat = adc_once(ADC0_SE8,ADC_16bit);//�ɼ�ADC0_SE8��ѹ���ֱ���16λ
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_once(ADCn_Ch adcn_ch, ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
    uint16 result = 0;
    ADCn adcn = (ADCn)(adcn_ch >> 5) ;
    adc_start(adcn_ch, bit);      //����ADCת��
    
    while (( ADCX[adcn]->SC1[0] & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK);   //ֻ֧�� Aͨ��
    result = ADCX[adcn]->R[0];
    ADCX[adcn]->SC1[0] &= ~ADC_SC1_COCO_MASK;
    return result;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADCֹͣ�ɼ�
//  @param      adcn_ch         ѡ��ADCͨ��
//  @return     void
//  @since      v1.0
//  Sample usage:               adc_stop(adc0);
//-------------------------------------------------------------------------------------------------------------------
void adc_stop(ADCn adcn)
{
    ADCX[adcn]->SC1[0] = (0
                          | ADC_SC1_AIEN_MASK                       // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                          //| ADC_SC1_DIFF_MASK                     // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                          | ADC_SC1_ADCH(Module0_Dis)               //����ͨ��ѡ��,�˴�ѡ���ֹͨ��
                         );
}

