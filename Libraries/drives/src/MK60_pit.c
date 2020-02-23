/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_pit
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_pit.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��
//  @param      pitn            ѡ��pitģ��
//  @param      cnt             ��ʱ�жϵ�ʱ��
//  @return     void
//  @since      v1.0
//  Sample usage:               pit_init(pit0,1000);								//��ʱ1000��busʱ�Ӻ��ж�
//								set_irq_priority(PIT0_IRQn,1);						//�������ȼ�,�����Լ�����������
//								enable_irq(PIT0_IRQn);								//��pit0���жϿ���
//								EnableInterrupts;									//���ܵ��жϿ���
//-------------------------------------------------------------------------------------------------------------------
void pit_init(PITn pitn, uint32 cnt)
{
	SIM->SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
	PIT->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
	PIT->CHANNEL[pitn].LDVAL  = cnt;              			//��������ж�ʱ��
	PIT_FlAG_CLR(pitn);										//����жϱ�־λ
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
								| PIT_TCTRL_TIE_MASK        //��PITn�ж�
								);
	//enable_irq( (IRQn_Type)((int)pitn + PIT0_IRQn) );       //���ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��
//  @param      pitn            ѡ��pitģ��
//  @param      cnt             ��ʱ��ʱ��ʱ��
//  @return     void
//  @since      v1.0
//  Sample usage:               pit_init(pit0,1000);		//��ʱ1000��busʱ��
//-------------------------------------------------------------------------------------------------------------------
void pit_delay(PITn pitn, uint32 cnt)
{
	SIM->SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
	PIT->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
	PIT->CHANNEL[pitn].LDVAL  = cnt;              			//��������ж�ʱ��
	PIT_FlAG_CLR(pitn);										//����жϱ�־λ
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
								//| PIT_TCTRL_TIE_MASK        //��PITn�ж�
								);
	while( !((PIT->CHANNEL[pitn].TFLG) & PIT_TFLG_TIF_MASK) ){}//�ȴ�ʱ�䵽

    PIT_FlAG_CLR(pitn);                                     //���жϱ�־λ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               pit_time_start(pit0);		//pit0��ʼ��ʱ
//-------------------------------------------------------------------------------------------------------------------
void pit_time_start(PITn pitn)
{
	SIM->SCGC6       |= SIM_SCGC6_PIT_MASK;        			//ʹ��PITʱ��
	PIT->MCR         = 0;									//ʹ��PIT��ʱ��ʱ�� ������ģʽ�¼�������
	PIT->CHANNEL[pitn].LDVAL  = ~0;              			//��������ж�ʱ��
	PIT_FlAG_CLR(pitn);										//����жϱ�־λ
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
    PIT->CHANNEL[pitn].TCTRL  = ( 0
								| PIT_TCTRL_TEN_MASK        //ʹ�� PITn��ʱ��
								//| PIT_TCTRL_TIE_MASK        //��PITn�ж�
								);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               pit_time_get(pit0);			//��ȡ��ǰpit0��ʱʱ��
//-------------------------------------------------------------------------------------------------------------------
uint32 pit_time_get(PITn pitn)
{
	uint32 val;
	val = (uint32)(~0) - PIT->CHANNEL[pitn].CVAL;
	if(PIT->CHANNEL[pitn].TFLG &  PIT_TFLG_TIF_MASK)		//�ж�ʱ���Ƿ�ʱ
	{
		PIT_FlAG_CLR(pitn);									//����жϱ�־λ
		PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;   //��ֹPITn��ʱ����������ռ���ֵ��
		return 0XFFFFFFFF;									//��ʱ�������ֵ
	}
	return val;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pit��ʼ��
//  @param      pitn            ѡ��pitģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               pit_close(pit0);			//pit0ֹͣ��ʱ
//-------------------------------------------------------------------------------------------------------------------
void pit_close(PITn pitn)
{
	PIT_FlAG_CLR(pitn);										//����жϱ�־λ
	PIT->CHANNEL[pitn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;       //��ֹPITn��ʱ����������ռ���ֵ��
}
