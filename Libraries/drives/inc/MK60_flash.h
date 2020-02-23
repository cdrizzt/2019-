/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_flash
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
#ifndef _MK60_flash_h
#define _MK60_flash_h

#include "misc.h"
#include "common.h"



#define FTF    					    FTFE

#define PROGRAM_CMD      			PGM8
#define SECTOR_SIZE     			(4096)
#define FLASH_SECTOR_NUM        	(256)                   //������
#define FLASH_ALIGN_ADDR        	4                       //��ַ����������
typedef uint32                  	FLASH_WRITE_TYPE;       //flash_write ����д�� ����������


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ʹ�ú궨���flash�������ݶ�ȡ
//  @param      SectorNum 		��Ҫд����������
//  @param      offset	 		��ַƫ��
//  @param      type		 	��ȡ����������
//  @return     				���ظ�����ַ������
//  @since      v1.0
//  Sample usage:               flash_read(20,0,uint32);//��ȡ20������ƫ��0��������Ϊuint32
//-------------------------------------------------------------------------------------------------------------------
#define     flash_read(SectorNum,offset,type)        (*(type *)((uint32)(((SectorNum)*SECTOR_SIZE) + (offset))))


void FLASH_Init(void);
uint32 FLASH_GetSectorSize(void);
uint8 FLASH_EraseSector(uint32 SectorNum);
uint8 FLASH_WriteSector(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset);







#endif
