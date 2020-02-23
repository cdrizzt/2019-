/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_32LCD.h
 * @brief      		2.8TFTҺ����������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note	
					TFT���߶��壺
					------------------------------------ 
					    ģ��ܽ�       	��Ƭ���ܽ�
					    DB0-DB15        D0-D15
                        RS              E7
                        WR              E10
                        CS              E6
                        RST             E12
                        RD              E11
					------------------------------------ 
 ********************************************************************************************************************/
 
 
#ifndef _SEEKFREE_28LCD_H
#define _SEEKFREE_28LCD_H		

#include "headfile.h"


//�����Ƿ�ʹ�ú��� 		0,��ʹ��.1,ʹ��.
#define USE_HORIZONTAL  0	


//����16λ���ݿڣ�ʹ��ָ�����D����������Ĵ����˿ڣ�ǿ��Ϊ(uint16 *)���ͼ���ʾ���ʵ���16λ
#define LCD_DataPort    *(uint16 *)(PTD_BASE)     
//���������IO��
#define  RS             E7
#define  WR             E10
#define  RD             E11
#define  CS             E6
#define  RST            E12
//����������������λ�Ĵ���
#define  LCD_RS         PEout(7)
#define  LCD_WR         PEout(10)
#define  LCD_RD         PEout(11)
#define  LCD_CS         PEout(6)
#define  LCD_RESET      PEout(12)






//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//LCD��Ҫ������
typedef struct  
{										    
	uint16 width;			//LCD ���
	uint16 height;			//LCD �߶�
	uint16 id;				//LCD ID
	uint8  dir;			//���������������ƣ�0��������1��������	
	uint16	 wramcmd;		//��ʼдgramָ��
	uint16  setxcmd;		//����x����ָ��
	uint16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	


extern const uint8 timer[];
extern const unsigned char gImage_36[34602];
extern const uint8 unlocked[];
extern const uint8 locked[];
extern const uint8 timer_running[];
extern const uint8 timer_stop[];
extern const uint8 now_time[];
extern const uint8 lap_length[];
extern const uint8 fin_speed[];


//LCD����  ����LCD��Ҫ����
extern _lcd_dev lcddev;	

void LCD_Init(void); 
void LCD_Clear(uint16 Color);
void LCD_WR_DATA(int Data);
void LCD_WR_REG(int Reg);
void LCD_SetCursor(uint16 Xpos, uint16 Ypos);//���ù��λ��
void LCD_SetWindows(uint16 xStar, uint16 yStar,uint16 xEnd,uint16 yEnd);//������ʾ����
void LCD_DrawPoint(uint16 x,uint16 y);//����
void LCD_WriteRAM_Prepare(void);
void LCD_SetParam(void);
void LCD_Display_16x8Str(uint16 x, uint16 y, int8 * str, uint16 color);
void LCD_Display_32x16Str(uint16 x, uint16 y, int8 * str, uint16 color);
void LCD_Display_16x8Num(uint16 x, uint16 y, int dat, uint8 number, uint16 color);
void LCD_Display_32x16Num(uint16 x, uint16 y, int dat, uint8 number, uint16 color);
void LCD_Displayimage032(uint8 *p);
void LCD_Displayimage7725(uint8 *p);
void LCD_Display_Chinese(uint16 x, uint16 y, uint8 number, uint8 size, const uint8 * p, uint16 color);
void LCD_Display_image(uint16 x, uint16 y, uint16 size_x, uint16 size_y, const uint8 *p);

#endif  
	 
	 



