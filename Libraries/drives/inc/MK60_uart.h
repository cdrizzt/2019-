/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_uart
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_uart_h
#define _MK60_uart_h



#include "misc.h"
#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_port_cfg.h"
#include "stdio.h"

typedef enum
{
    uart0,
    uart1,
    uart2,
    uart3,
    uart4,
} UARTn;



#define DEBUG_PORT  uart2
#define DEBUG_BAUD  115200

void uart_init(UARTn uartn, uint32 baud);
void uart_putchar (UARTn uartn, uint8 ch);
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len);
void uart_putstr (UARTn uartn, const uint8 *str);
void uart_getchar (UARTn uartn, uint8 *ch);
char uart_querychar (UARTn uartn, uint8 *ch);
void uart_rx_irq_en(UARTn uartn);
void uart_txc_irq_en(UARTn uartn);
void uart_rx_irq_dis(UARTn uartn);
void uart_txc_irq_dis(UARTn uartn);

#endif
