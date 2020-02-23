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
 
 

#include "MK60_port.h"


static PORT_Type * const PORTX[] = PORT_BASE_PTRS;
static GPIO_Type * const GPIOX[] = GPIO_BASE_PTRS;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PORT��ʼ��
//  @param      ptx_n           �˿�
//  @param      cfg             �˿��������ã��紥��ѡ�����������ѡ��
//  @return     void   
//  @since      v1.0
//  Sample usage:               port_init (A14, IRQ_RISING | PF | ALT1 | PULLUP );	//��ʼ�� A14 �ܽţ������ش����жϣ�����Դ�˲��������ù���ΪGPIO������Ϊ���� ����������
//								set_irq_priority(PORTA_IRQn,1);						//�������ȼ�
//								enable_irq(PORTA_IRQn);								//��PORTA�жϿ���
//								EnableInterrupts;									//���ܵ��жϿ���
//-------------------------------------------------------------------------------------------------------------------
void port_init(PTX_n ptx_n,uint32 cfg)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //ʹ�ܶ˿�ʱ��
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
    
    //����жϱ�־λ
    PORTX[ptx]->ISFR = (uint32)1<<ptn;
    
    //���ø��ù���ΪGPIO�����ö˿ڹ���
    PORTX[ptx]->PCR[ptn] = cfg;
    
    //���ö˿�Ϊ����
    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
    
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PORT��ʼ��
//  @param      ptx_n           �˿�
//  @param      cfg             �˿��������ã��紥��ѡ�����������ѡ��
//  @return     void   
//  @since      v1.0
//  Sample usage:               port_init_NoAlt (A14, IRQ_RISING | PF | ALT1 | PULLUP );//��ʼ�� A14 �ܽţ������ش����жϣ�����Դ�˲���������֮ǰ��MUX ����������
//-------------------------------------------------------------------------------------------------------------------
void port_init_NoAlt(PTX_n ptx_n,uint32 cfg)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //ʹ�ܶ˿�ʱ��
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
           
    //���ö˿ڹ���
    PORTX[ptx]->PCR[ptn] = (PORTX[ptx]->PCR[ptn] & PORT_PCR_MUX_MASK) | cfg;
}
