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
 
 
#include "MK60_flash.h"


// flash commands 
#define RD1BLK    0x00  // read 1 block
#define RD1SEC    0x01  // read 1 section
#define PGMCHK    0x02  // program check 
#define RDRSRC    0x03  // read resource 
#define PGM4      0x06  // program phase program 4 byte 
#define PGM8      0x07  // program phase program 8 byte 
#define ERSBLK    0x08  // erase flash block 
#define ERSSCR    0x09  // erase flash sector 
#define PGMSEC    0x0B  // program section 
#define RD1ALL    0x40  // read 1s all block 
#define RDONCE    0x41  // read once 
#define PGMONCE   0x43  // program once 
#define ERSALL    0x44  // erase all blocks 
#define VFYKEY    0x45  // verift backdoor key 
#define PGMPART   0x80  // program paritition 
#define SETRAM    0x81  // set flexram function 
#define NORMAL_LEVEL 0x0


// disable interrupt before lunch command 
#define CCIF    (1<<7)
#define ACCERR  (1<<5)
#define FPVIOL  (1<<4)
#define MGSTAT0 (1<<0)






volatile uint8 s_flash_command_run[] = {0x00, 0xB5, 0x80, 0x21, 0x01, 0x70, 0x01, 0x78, 0x09, 0x06, 0xFC, 0xD5,0x00, 0xBD};
typedef void (*flash_run_entry_t)(volatile uint8 *reg);
flash_run_entry_t s_flash_run_entry;
    

//�ڲ�ʹ��
__STATIC_INLINE uint8 FlashCmdStart(void)
{
	//�����������־λ
    FTF->FSTAT = ACCERR | FPVIOL;
    s_flash_run_entry = (flash_run_entry_t)((uint32)s_flash_command_run + 1);
    s_flash_run_entry(&FTF->FSTAT);
    
    if(FTF->FSTAT & (ACCERR | FPVIOL | MGSTAT0)) return 1;	//���ִ���
    return 0;												//�ɹ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��ʼ��
//  @return     				����һ�������Ĵ�С
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize();
//-------------------------------------------------------------------------------------------------------------------
uint32 FLASH_GetSectorSize(void)
{
    return SECTOR_SIZE;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��ʼ��
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void FLASH_Init(void)
{
    //���״̬��ʶ
    FTF->FSTAT = ACCERR | FPVIOL;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH��������
//  @param      SectorNum 		��Ҫ�������������
//  @return     				����1����ʧ�ܣ�����0�����ɹ�
//  @since      v1.0
//  Sample usage:               uint32 dat = FLASH_GetSectorSize(10);
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_EraseSector(uint32 SectorNum)
{
    int ret;
	
	union
	{
		uint32  word;
		uint8   byte[4];
	} dest;

	dest.word = (uint32)SectorNum*SECTOR_SIZE;

	//��������
	FTF->FCCOB0 = ERSSCR; 
	FTF->FCCOB1 = dest.byte[2];
	FTF->FCCOB2 = dest.byte[1];
	FTF->FCCOB3 = dest.byte[0];
    __disable_irq();
    ret = FlashCmdStart();
    __enable_irq();
    
    return ret;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      FLASH����д�뵽����
//  @param      SectorNum 		��Ҫд����������
//  @param      *buf	 		�����׵�ַ
//  @param      len		 		д����ֽ��� �ر�����һ��uint16�����������ֽ� һ��uint32�������ĸ��ֽ�
//  @param      offset		 	����Ϊ8��������
//  @return     				����1д��ʧ�ܣ�����0д��ɹ�
//  @since      v1.0
//  Sample usage:               FLASH_WriteSector(10,(const uint8 *)buf,8,0);//��buf���������ǰ��λ����д�뵽10������(ÿһλ��uint8����)
//-------------------------------------------------------------------------------------------------------------------
uint8 FLASH_WriteSector(uint32 SectorNum, const uint8 *buf, uint32 len, uint32 offset)
{
    uint16 step, ret, i;
	union
	{
		uint32  word;
		uint8   byte[4];
	} dest;
	dest.word = (uint32)SectorNum*SECTOR_SIZE + offset;

	FTF->FCCOB0 = PROGRAM_CMD;

    step = 8;

	for(i=0; i<len; i+=step)
	{
        //���õ�ַ
		FTF->FCCOB1 = dest.byte[2];
		FTF->FCCOB2 = dest.byte[1];
		FTF->FCCOB3 = dest.byte[0];
		//��������
		FTF->FCCOB4 = buf[3];
		FTF->FCCOB5 = buf[2];
		FTF->FCCOB6 = buf[1];
		FTF->FCCOB7 = buf[0];
        FTF->FCCOB8 = buf[7];
        FTF->FCCOB9 = buf[6];
        FTF->FCCOBA = buf[5];
        FTF->FCCOBB = buf[4];
        
		dest.word += step; buf += step;

        DisableInterrupts;
        ret = FlashCmdStart();
        EnableInterrupts;
        
		if(ret) return ret;
    }
    return ret;
}

