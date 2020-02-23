/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_18TFT.h
 * @brief      		TFTҺ��������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note		
					1.8 TFTҺ�����߶��壺
					------------------------------------ 
						ģ��ܽ�            ��Ƭ���ܽ�
						SCL                 A15
						SDA                 A16
						RES                 B16
						DC                  B17
						CS                  A14
						
						��Դ����
						BL  3.3V��Դ������������ţ�Ҳ���Խ�PWM���������ȣ�
						VCC 3.3V��Դ
						GND ��Դ��
	
					------------------------------------ 

 ********************************************************************************************************************/



#ifndef _SEEKFREE_18TFT_H
#define _SEEKFREE_18TFT_H

#include "headfile.h"


#define	TFT_X_MAX	128	//Һ��X�����
#define TFT_Y_MAX	160 //Һ��Y�����


#define DC_PIN		D15	//Һ������λӲ������		
#define REST_PIN	E5 //Һ����λ���Ŷ���
#define DC(x)   	gpio_set(DC_PIN,x);
#define REST(x) 	gpio_set(REST_PIN,x);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define BGCOLOR	 RED
	 
extern const uint8 tft_ascii[95][16];
extern const uint8  asc2_1608[1520];

void lcd_init(void);
void dsp_single_colour(int color);
void displayimage032(uint8 *p);
void displayimage7725(unsigned char *p);

void  LCD_WriteData_16Bit(uint16 dat);	//��Һ����дһ��16λ����;
#endif
