/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_gpio
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/



#ifndef _MK60_gpio_h
#define _MK60_gpio_h

#include "common.h"

// λ������ �ڴ�ƫ�Ƽ��� ��� Cortex-M4 Generic User Guide 2.25 
// CM4����2��bitband���� 0x2000_0000-0x200F_FFFF ӳ���� 0x2200_0000-0x23FF_FFFF
//                       0x4000_0000-0x4000_FFFF ӳ���� 0x4200_0000-0x43FF_FFFF

#define PAout(n)   BITBAND_REG(PTA->PDOR, n)
#define PAin(n)    BITBAND_REG(PTA->PDIR, n)
  
#define PBout(n)   BITBAND_REG(PTB->PDOR, n)
#define PBin(n)    BITBAND_REG(PTB->PDIR, n)

#define PCout(n)   BITBAND_REG(PTC->PDOR, n)
#define PCin(n)    BITBAND_REG(PTC->PDIR, n)

#define PDout(n)   BITBAND_REG(PTD->PDOR, n)
#define PDin(n)    BITBAND_REG(PTD->PDIR, n)

#define PEout(n)   BITBAND_REG(PTE->PDOR, n)
#define PEin(n)    BITBAND_REG(PTE->PDIR, n)




#define A_PDOR8(x)    *(uint8 *)(PTA_BASE+0x00+x) //����A�˿ڵ�8λ�����ַ
#define A_PSOR8(x)    *(uint8 *)(PTA_BASE+0x04+x) //����A�˿ڵ�8λ��λ��ַ
#define A_PCOR8(x)    *(uint8 *)(PTA_BASE+0x08+x) //����A�˿ڵ�8λ�����ַ
#define A_PTOR8(x)    *(uint8 *)(PTA_BASE+0x0C+x) //����A�˿ڵ�8λȡ����ַ
#define A_PDIR8(x)    *(uint8 *)(PTA_BASE+0x10+x) //����A�˿ڵ�8λ�����ַ
#define A_PDDR8(x)    *(uint8 *)(PTA_BASE+0x14+x) //����A�˿ڵ�8λ�����ַ
                                                         
                                                         
#define B_PDOR8(x)    *(uint8 *)(PTB_BASE+0x00+x) //����B�˿ڵ�8λ�����ַ
#define B_PSOR8(x)    *(uint8 *)(PTB_BASE+0x04+x) //����B�˿ڵ�8λ��λ��ַ
#define B_PCOR8(x)    *(uint8 *)(PTB_BASE+0x08+x) //����B�˿ڵ�8λ�����ַ
#define B_PTOR8(x)    *(uint8 *)(PTB_BASE+0x0C+x) //����B�˿ڵ�8λȡ����ַ
#define B_PDIR8(x)    *(uint8 *)(PTB_BASE+0x10+x) //����B�˿ڵ�8λ�����ַ
#define B_PDDR8(x)    *(uint8 *)(PTB_BASE+0x14+x) //����B�˿ڵ�8λ�����ַ


#define C_PDOR8(x)    *(uint8 *)(PTC_BASE+0x00+x) //����C�˿ڵ�8λ�����ַ
#define C_PSOR8(x)    *(uint8 *)(PTC_BASE+0x04+x) //����C�˿ڵ�8λ��λ��ַ
#define C_PCOR8(x)    *(uint8 *)(PTC_BASE+0x08+x) //����C�˿ڵ�8λ�����ַ
#define C_PTOR8(x)    *(uint8 *)(PTC_BASE+0x0C+x) //����C�˿ڵ�8λȡ����ַ
#define C_PDIR8(x)    *(uint8 *)(PTC_BASE+0x10+x) //����C�˿ڵ�8λ�����ַ
#define C_PDDR8(x)    *(uint8 *)(PTC_BASE+0x14+x) //����C�˿ڵ�8λ�����ַ


#define D_PDOR8(x)    *(uint8 *)(PTD_BASE+0x00+x) //����D�˿ڵ�8λ�����ַ
#define D_PSOR8(x)    *(uint8 *)(PTD_BASE+0x04+x) //����D�˿ڵ�8λ��λ��ַ
#define D_PCOR8(x)    *(uint8 *)(PTD_BASE+0x08+x) //����D�˿ڵ�8λ�����ַ
#define D_PTOR8(x)    *(uint8 *)(PTD_BASE+0x0C+x) //����D�˿ڵ�8λȡ����ַ
#define D_PDIR8(x)    *(uint8 *)(PTD_BASE+0x10+x) //����D�˿ڵ�8λ�����ַ
#define D_PDDR8(x)    *(uint8 *)(PTD_BASE+0x14+x) //����D�˿ڵ�8λ�����ַ


#define E_PDOR8(x)    *(uint8 *)(PTE_BASE+0x00+x) //����E�˿ڵ�8λ�����ַ
#define E_PSOR8(x)    *(uint8 *)(PTE_BASE+0x04+x) //����E�˿ڵ�8λ��λ��ַ
#define E_PCOR8(x)    *(uint8 *)(PTE_BASE+0x08+x) //����E�˿ڵ�8λ�����ַ
#define E_PTOR8(x)    *(uint8 *)(PTE_BASE+0x0C+x) //����E�˿ڵ�8λȡ����ַ
#define E_PDIR8(x)    *(uint8 *)(PTE_BASE+0x10+x) //����E�˿ڵ�8λ�����ַ
#define E_PDDR8(x)    *(uint8 *)(PTE_BASE+0x14+x) //����E�˿ڵ�8λ�����ַ


//���º궨����Ҫ���ڼ����ϰ汾������ͷ�ɼ����룬Ҳ����ɾ������ĺ궨��
//������ͷ�ɼ������н�����Ķ�궨�廻������ĺ궨��
#define A_IN_DATA(x)  *(uint8 *)(PTA_BASE+0x10+x)//����PTA��8λ����˿ڵ�ַ
#define B_IN_DATA(x)  *(uint8 *)(PTB_BASE+0x10+x)//����PTB��8λ����˿ڵ�ַ
#define C_IN_DATA(x)  *(uint8 *)(PTC_BASE+0x10+x)//����PTC��8λ����˿ڵ�ַ
#define D_IN_DATA(x)  *(uint8 *)(PTD_BASE+0x10+x)//����PTD��8λ����˿ڵ�ַ
#define E_IN_DATA(x)  *(uint8 *)(PTE_BASE+0x10+x)//����PTE��8λ����˿ڵ�ַ


void    gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat);//��ʼ��gpio
void    gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr);            //�������ŷ���
uint8   gpio_get(PTX_n ptx_n);                          //��ȡ����״̬
void    gpio_set(PTX_n ptx_n, uint8 dat);               //��������״̬
void    gpio_turn(PTX_n ptx_n);                         //��ת����״̬

#endif
