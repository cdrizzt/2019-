/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_port
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 

#ifndef _MK60_port_h
#define _MK60_port_h


#include "common.h"
#include "MK60_gpio.h"





// ö��PORT ���� 
typedef enum
{
    //�жϷ�ʽ��DMA����ʽ������ֻ��ѡ����һ�֣����Բ�ѡ��
    //�жϷ�ʽѡ��
    IRQ_ZERO     = 0x08 << PORT_PCR_IRQC_SHIFT,   //�͵�ƽ����
    IRQ_RISING   = 0x09 << PORT_PCR_IRQC_SHIFT,   //�����ش���
    IRQ_FALLING  = 0x0A << PORT_PCR_IRQC_SHIFT,   //�½��ش���
    IRQ_EITHER   = 0x0B << PORT_PCR_IRQC_SHIFT,   //�����ش���
    IRQ_ONE      = 0x0C << PORT_PCR_IRQC_SHIFT,   //�ߵ�ƽ����

    //DMA����ѡ��
    DMA_RISING   = 0x01 << PORT_PCR_IRQC_SHIFT,   //�����ش���
    DMA_FALLING  = 0x02 << PORT_PCR_IRQC_SHIFT,   //�½��ش���
    DMA_EITHER   = 0x03 << PORT_PCR_IRQC_SHIFT,   //�����ش���


    HDS          = 0x01 << PORT_PCR_DSE_SHIFT,    //�������������
    ODO          = 0x01 << PORT_PCR_ODE_SHIFT,    //©�����
    PF           = 0x01 << PORT_PCR_PFE_SHIFT,    //����Դ�˲���
    SSR          = 0x01 << PORT_PCR_SRE_SHIFT,    //������仯��          Slow slew rate

    //��������ѡ��
    PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //����
    PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //����

    //���ܸ���ѡ��(�������Ҫ�ı书�ܸ���ѡ�񣬱���ԭ�ȵĹ��ܸ��ã�ֱ��ѡ��ALT0 )
    //��Ҫ�� K60 Signal Multiplexing and Pin Assignments
    ALT0         = 0x00 << PORT_PCR_MUX_SHIFT,
    ALT1         = 0x01 << PORT_PCR_MUX_SHIFT,    //GPIO
    ALT2         = 0x02 << PORT_PCR_MUX_SHIFT,
    ALT3         = 0x03 << PORT_PCR_MUX_SHIFT,
    ALT4         = 0x04 << PORT_PCR_MUX_SHIFT,
    ALT5         = 0x05 << PORT_PCR_MUX_SHIFT,
    ALT6         = 0x06 << PORT_PCR_MUX_SHIFT,
    ALT7         = 0x07 << PORT_PCR_MUX_SHIFT,
} port_cfg;

#define PORTA_FLAG_CLR(PTX_n)   PORTA->ISFR = (uint32)(1<<PTn(PTX_n))
#define PORTB_FLAG_CLR(PTX_n)   PORTB->ISFR = (uint32)(1<<PTn(PTX_n))
#define PORTC_FLAG_CLR(PTX_n)   PORTC->ISFR = (uint32)(1<<PTn(PTX_n))
#define PORTD_FLAG_CLR(PTX_n)   PORTD->ISFR = (uint32)(1<<PTn(PTX_n))
#define PORTE_FLAG_CLR(PTX_n)   PORTE->ISFR = (uint32)(1<<PTn(PTX_n))

void    port_init(PTX_n ptx_n,uint32 cfg);
void    port_init_NoAlt(PTX_n ptx_n,uint32 cfg);

#endif
