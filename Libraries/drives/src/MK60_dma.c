/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_dma
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 
 
 
#include "MK60_dma.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA��ʼ������IO������������˿ڵ����ݵ��ڴ�
//  @param      DMA_CHn         ͨ���ţ�DMA_CH0 ~ DMA_CH15��
//  @param      SADDR           Դ��ַ( (void * )&X_IN_DATA
//  @param      DADDR           Ŀ�ĵ�ַ
//  @param      PTxn            �����˿�
//  @param      DMA_BYTEn       ÿ��DMA�����ֽ���
//  @param      count           һ����ѭ�������ֽ���
//  @param      cfg             DMA�������ã���DMA_cfg��ѡ��
//  @return     void
//  @since      v1.0
//  Sample usage:   uint8 dat[10];
//                  dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(0), dat, A7, DMA_BYTE1, 10, DADDR_RECOVER);
//                  												//DMA��ʼ����Դ��ַ��C_IN_DATA(��C0-C7)��Ŀ�ĵ�ַ��dat,A7����(Ĭ��������)��ÿ�δ���1�ֽڣ������� 10�� �����������ָ���ַ
//                  port_init(A7,ALT1 | DMA_FALLING);               //Ĭ�ϴ���Դ�������أ��˴���Ϊ �½��ش���
//                  DMA_EN(DMA_CH0);                				//��Ҫʹ�� DMA ����ܴ�������
//-------------------------------------------------------------------------------------------------------------------
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg)
{

    uint8 BYTEs = 1;
    uint8 i = DMA_BYTE1;
    uint8 n, tmp;
    uint8 ptxn_s;
    uint8 ptx0 = ((((uint32)SADDR) & 0x1C0) >> 6 ) << 5;
    
    while(i)//���㴫���ֽ���
    {
        BYTEs *= 2;
        i--;
    }
    //DMA �Ĵ��� ����

    // ����ʱ�� 
    SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;                        //��DMAģ��ʱ��
    SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                    //��DMA��·������ʱ��

    // ���� DMA ͨ�� �� ������ƿ� TCD ( Transfer Control Descriptor ) 
    DMA0->TCD[CHn].SADDR = 0;
    DMA0->TCD[CHn].SADDR = (uint32)SADDR;                   // ����  Դ��ַ
    DMA0->TCD[CHn].DADDR = 0;                               
    DMA0->TCD[CHn].DADDR = (uint32)DADDR;                   // ����Ŀ�ĵ�ַ
    DMA0->TCD[CHn].SOFF = 0;                                
    DMA0->TCD[CHn].SOFF = 0x00u;                            // ����Դ��ַƫ�� = 0x0, ������
    DMA0->TCD[CHn].DOFF = 0;                                
    DMA0->TCD[CHn].DOFF = BYTEs;                            // ÿ�δ����Ŀ�ĵ�ַ�� BYTEs
    DMA0->TCD[CHn].ATTR = 0;
    DMA0->TCD[CHn].ATTR = (0
                         | DMA_ATTR_SMOD(0x0)               // Դ��ַģ����ֹ  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // Դ����λ�� ��DMA_BYTEn  ��    SSIZE = 0 -> 8-bit ��SSIZE = 1 -> 16-bit ��SSIZE = 2 -> 32-bit ��SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // Ŀ���ַģ����ֹ
                         | DMA_ATTR_DSIZE(byten)            // Ŀ������λ�� ��DMA_BYTEn  ��  ���òο�  SSIZE
                        );
    DMA0->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);   //��ǰ��ѭ������ ��ǰѭ�������ľ����ٴμ�����ʼ��ѭ������
    DMA0->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);   //��ʼ��ѭ������
    DMA0->CR &= ~DMA_CR_EMLM_MASK;                                // CR[EMLM] = 0
    DMA0->TCD[CHn].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(BYTEs); // ͨ��ÿ�δ����ֽ�������������ΪBYTEs���ֽڡ�ע��ֵΪ0��ʾ����4GB 
    
    
    // ���� DMA ���������Ĳ��� 
    DMA0->TCD[CHn].SLAST = 0;//����  Դ��ַ�ĸ���ֵ,��ѭ��������ָ�  Դ��ַ
    DMA0->TCD[CHn].DLAST_SGA = (uint32)( (cfg & DADDR_KEEPON ) == 0 ? (-count)  : 0 ); //����Ŀ�ĵ�ַ�ĸ���ֵ,��ѭ��������ָ�Ŀ�ĵ�ַ���߱��ֵ�ַ;
    DMA0->TCD[CHn].CSR = 0;
    DMA0->TCD[CHn].CSR = (0
                             | DMA_CSR_BWC(3)               //�������,ÿ��һ�Σ�eDMA ����ֹͣ 8 �����ڣ�0��ֹͣ��1������2ֹͣ4���ڣ�3ֹͣ8���ڣ�
                             | DMA_CSR_DREQ_MASK            //��ѭ��������ֹͣӲ������
                             | DMA_CSR_INTMAJOR_MASK        //��ѭ������������ж�
                            );
    DMA0->TCD[CHn].SLAST = 0;
    
    
    // ���� DMA ����Դ 


    DMAMUX0->CHCFG[CHn] = (0
                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                            //| DMAMUX_CHCFG_TRIG_MASK                      // Trigger Mode: Periodic   PIT���ڴ�������ģʽ   ͨ��1��ӦPIT1������ʹ��PIT1����������Ӧ��PIT��ʱ���� 
                            | DMAMUX_CHCFG_SOURCE( PTX(ptxn) + DMA_PORTA)  // ͨ����������Դ:    
                          );
    

    //���ô���Դ��Ĭ���� �����ش�����
    port_init(ptxn, ALT1 | DMA_RISING);

    //  ��������Դ   
    
    n = (uint8)(((uint32)SADDR - ((uint32)(&A_IN_DATA(0)))) & 0x3f) * 8;       //��С�����ź�

    ptxn_s = ptx0 + n;
    tmp = ptxn_s + (BYTEs * 8 ) - 1;                                          //�������ź�
    while(ptxn_s <= tmp)
    {
        //���������ǰѶ˿����ݴ��͵��ڴ��У������Ҫ���ö˿�Ϊ����״̬
        gpio_init((PTX_n )ptxn_s, GPI, 0);
        port_init((PTX_n )ptxn_s , ALT1 | PULLDOWN );
        ptxn_s ++;
    }
    
    DMA_DIS(CHn);                                    //ʹ��ͨ��CHn Ӳ������
    DMA_IRQ_CLEAN(CHn);

    // �����ж� 
    //DMA_EN(CHn);                                    //ʹ��ͨ��CHn Ӳ������
    //DMA_IRQ_EN(CHn);                                //����DMAͨ������
}


