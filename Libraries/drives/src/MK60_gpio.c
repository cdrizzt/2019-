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



#include "MK60_gpio.h"

static PORT_Type * const PORTX[] = PORT_BASE_PTRS;
static GPIO_Type * const GPIOX[] = GPIO_BASE_PTRS;



//-------------------------------------------------------------------------------------------------------------------
//  @brief      gpio ��ʼ��
//  @param      ptx_n           ѡ�������   
//  @param      ddr             ���ŷ���    GPI������ GPO�����
//  @param      dat             ����������ݣ��������Ϊ�����
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_init(A5,GPO,0);   //��ʼ��A5Ϊgpioģʽ������Ϊ���ģʽ����ʼ������͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_init(PTX_n ptx_n, GPIO_MOD ddr, uint8 dat)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //ʹ�ܶ˿�ʱ��
	SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << ptx);
    
    //���֮ǰ�ĸ��ù���
    PORTX[ptx]->PCR[ptn] &= ~(uint32)PORT_PCR_MUX_MASK;
    //���ø��ù���ΪGPIO����ͨIO��
    PORTX[ptx]->PCR[ptn] |= PORT_PCR_MUX(1);
    
    //���ö˿ڷ���
    if(ddr) GPIOX[ptx]->PDDR |= (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
    
    //���ö˿�״̬
    if(ddr)
    {
        if(dat) GPIOX[ptx]->PDOR |=  (uint32)(1<<ptn);
        else    GPIOX[ptx]->PDOR &= ~(uint32)(1<<ptn);   
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ŷ���
//  @param      ptx_n           ѡ�������   
//  @param      ddr             ���ŷ���    GPI������ GPO�����
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_ddr(A5,GPO);   //����A5Ϊ���ģʽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_ddr(PTX_n ptx_n, GPIO_MOD ddr)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //���ö˿ڷ���
    if(ddr) GPIOX[ptx]->PDDR |= (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDDR &= ~(uint32)(1<<ptn);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����״̬
//  @param      ptx_n           ѡ�������   
//  @return     �ܽŵ�״̬      1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @since      v1.0
//  Sample usage:               uint8 status = gpio_get(A5);   //��ȡA5Ϊ״̬
//-------------------------------------------------------------------------------------------------------------------
uint8 gpio_get(PTX_n ptx_n)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //���ö˿�״̬
    return ( (GPIOX[ptx]->PDIR >> ptn) & 0x1 );
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��������״̬
//  @param      ptx_n           ѡ�������   
//  @param      dat             1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_set(A5,0);   //����A5Ϊ�͵�ƽ
//-------------------------------------------------------------------------------------------------------------------
void gpio_set(PTX_n ptx_n, uint8 dat)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //���ö˿�״̬
    if(dat) GPIOX[ptx]->PDOR |=  (uint32)(1<<ptn);
    else    GPIOX[ptx]->PDOR &= ~(uint32)(1<<ptn);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ת����״̬
//  @param      ptx_n           ѡ�������   
//  @return     void
//  @since      v1.0
//  Sample usage:               gpio_turn(A5);   //��תA5
//-------------------------------------------------------------------------------------------------------------------
void gpio_turn(PTX_n ptx_n)
{
    vuint8 ptx,ptn;
    
    ptx = PTX(ptx_n);//��¼ģ���    //A,B......
    ptn = PTn(ptx_n);//��¼���ź�    //0,1,2,3......
    
    //���ö˿�״̬
    GPIOX[ptx]->PTOR = (uint32)(1<<ptn);
}


