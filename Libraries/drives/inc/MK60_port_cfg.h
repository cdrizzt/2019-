/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_port_cfg
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#ifndef _MK60_port_cfg_h
#define _MK60_port_cfg_h

#include "common.h"

//-----------------------------------  UART  -----------------------------------

//      ģ��ͨ��        �˿�          ��ѡ��Χ                ����
#define UART0_RX_PIN    D6        //A1��A15��B16��D6          A1��Ҫ�ã���Jtag��ͻ��
#define UART0_TX_PIN    D7        //A2��A14��B17��D7          A2��Ҫ�ã���Jtag��ͻ��

#define UART1_RX_PIN    E1        //C3��E1
#define UART1_TX_PIN    E0        //C4��E0

#define UART2_RX_PIN    D2        //D2
#define UART2_TX_PIN    D3        //D3

#define UART3_RX_PIN    B10       //B10��C16��E5
#define UART3_TX_PIN    B11       //B11��C17��E4

#define UART4_RX_PIN    E25       //C14��E25
#define UART4_TX_PIN    E24       //C15��E24

//-----------------------------------  FTM  -----------------------------------

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM0_CH0_PIN    C1        //C1��A3                A3��Ҫ�ã���Jtag��SWD��ͻ��
#define FTM0_CH1_PIN    C2        //C2��A4    
#define FTM0_CH2_PIN    C3        //C3��A5    
#define FTM0_CH3_PIN    C4        //C4��A6    
#define FTM0_CH4_PIN    D4        //D4��A7    
#define FTM0_CH5_PIN    D5        //D5��A0                A0��Ҫ�ã���Jtag��SWD��ͻ��
#define FTM0_CH6_PIN    D6        //D6��A1                A1��Ҫ�ã���Jtag��ͻ��
#define FTM0_CH7_PIN    D7        //D7��A2                A2��Ҫ�ã���Jtag��ͻ��


//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM1_CH0_PIN    A8      //A8��A12��B0
#define FTM1_CH1_PIN    A9       //A9��A13��B1

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM2_CH0_PIN    A10       //A10��B18
#define FTM2_CH1_PIN    A11       //A11��B19

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define FTM3_CH0_PIN    E5        //D0 
#define FTM3_CH1_PIN    E6        //D1 
#define FTM3_CH2_PIN    E7        //D2 
#define FTM3_CH3_PIN    E8        //D3 
#define FTM3_CH4_PIN    E9        //C8 
#define FTM3_CH5_PIN    E10       //C9 
#define FTM3_CH6_PIN    E11       //C10
#define FTM3_CH7_PIN    E12       //C11          

//��������ģ��ͨ��      �˿�      ��ѡ��Χ                ����
#define FTM1_QDPHA_PIN  A8       //A8��A12��B0
#define FTM1_QDPHB_PIN  A9       //A9��A13��B1

#define FTM2_QDPHA_PIN  A10       //A10��B18
#define FTM2_QDPHB_PIN  A11       //A11��B19


//-----------------------------------  I2C  -----------------------------------

//      ģ��ͨ��        �˿�      ��ѡ��Χ                ����
#define I2C0_SCL_PIN    B2        // B0��B2��D8
#define I2C0_SDA_PIN    B3        // B1��B3��D9

#define I2C1_SCL_PIN    C10       // E1��C10
#define I2C1_SDA_PIN    C11       // E0��C11


//-----------------------------------  SPI  -----------------------------------
//�����ʹ��Ƭѡ�ӿڵĻ�����ҪƬѡ�ӿ�������������ʱ����Ҫע�͵����Ͳ�����г�ʼ����Ӧ�Ĺܽ�
//      ģ��ͨ��        �˿�      ��ѡ��Χ              ����

#define SPI0_SCK_PIN    A15       // A15��C5��D1        ȫ������ ALT2
#define SPI0_SOUT_PIN   A16       // A16��C6��D2        ȫ������ ALT2
#define SPI0_SIN_PIN    A17       // A17��C7��D3        ȫ������ ALT2

#define SPI0_PCS0_PIN   A14       // A14��C4��D0��      ȫ������ ALT2
#define SPI0_PCS1_PIN   C3        // C3��D4             ȫ������ ALT2
#define SPI0_PCS2_PIN   C2        // C2��D5             ȫ������ ALT2
#define SPI0_PCS3_PIN   C1        // C1��D6             ȫ������ ALT2
#define SPI0_PCS4_PIN   C0        // C0��               ȫ������ ALT2
#define SPI0_PCS5_PIN   B23       // B23                ALT3


#define SPI1_SCK_PIN    E2       // E2��B11��          ȫ������ ALT2
#define SPI1_SOUT_PIN   E1       // E1��B16��          ȫ������ ALT2
#define SPI1_SIN_PIN    E3       // E3��B17��          ȫ������ ALT2

#define SPI1_PCS0_PIN   B10       // E4��B10��          ȫ������ ALT2
#define SPI1_PCS1_PIN   E0        // E0��B9��           ȫ������ ALT2
#define SPI1_PCS2_PIN   E5        // E5��               ȫ������ ALT2
#define SPI1_PCS3_PIN   E6        // E6��               ȫ������ ALT2


#define SPI2_SCK_PIN    B21       // B21��D12           ȫ������ ALT2
#define SPI2_SOUT_PIN   B22       // B22��D13           ȫ������ ALT2
#define SPI2_SIN_PIN    B23       // B23��D14           ȫ������ ALT2
#define SPI2_PCS0_PIN   B20       // B20��D11           ȫ������ ALT2
#define SPI2_PCS1_PIN   D15       // D15                ȫ������ ALT2


//-----------------------------------  CAN  -----------------------------------
#define CAN0_TX_PIN     A12       //A12��B18            ȫ������ ALT2
#define CAN0_RX_PIN     A13       //A13��B19            ȫ������ ALT2

#define CAN1_TX_PIN     E24       //E24��C17            ȫ������ ALT2
#define CAN1_RX_PIN     E25       //E25��C16            ȫ������ ALT2


#endif


