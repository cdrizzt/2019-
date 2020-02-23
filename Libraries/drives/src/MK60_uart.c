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
 
 
 
#include "MK60_uart.h"




UART_Type * uart[5] = UART_BASE_PTRS;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      uart ��ʼ��
//  @param      uartn           ѡ�񴮿�(uart0,uart1,uart2,uart3,uart4,uart5)
//  @param      baud            ���ڲ�����
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_init(uart3,9600);   //��ʼ������3Ϊ1λ��ʼλ��8λ����λ��1λֹͣλ��������9600
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTn uartn, uint32 baud)
{
    vuint32 uart_input_clk;
    vuint32 sbr,brfa;
    //���йܽŸ�������
    switch(uartn)
    {
        case uart0:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��
            if(UART0_RX_PIN == A1)                                                              port_init( UART0_RX_PIN, ALT2);
            else if((UART0_RX_PIN == A15) || (UART0_RX_PIN == B16) || (UART0_RX_PIN == D6) )    port_init( UART0_RX_PIN, ALT3);

            if(UART0_TX_PIN == A2)                                                              port_init( UART0_TX_PIN, ALT2);
            else if((UART0_TX_PIN == A14) || (UART0_TX_PIN == B17) || (UART0_TX_PIN == D7) )    port_init( UART0_TX_PIN, ALT3);
        }break;
        
        case uart1:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;      //ʹ�� UART1 ʱ��
            if((UART1_RX_PIN == C3) || (UART1_RX_PIN == E1))                                    port_init( UART1_RX_PIN, ALT3);
            if((UART1_TX_PIN == C4) || (UART1_TX_PIN == E0))                                    port_init( UART1_TX_PIN, ALT3);
        }break;
        
        case uart2:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;      //ʹ�� UART2 ʱ��
            if(UART2_TX_PIN == D3)                                                              port_init( UART2_TX_PIN, ALT3);
            if(UART2_RX_PIN == D2)                                                              port_init( UART2_RX_PIN, ALT3);
        }break;
        
        case uart3:
        {
            SIM->SCGC4 |= SIM_SCGC4_UART3_MASK;      //ʹ�� UART3 ʱ��
            if((UART3_RX_PIN == B10) || (UART3_RX_PIN == C16) || (UART3_RX_PIN == E5) )         port_init( UART3_RX_PIN, ALT3);
            if((UART3_TX_PIN == B11) || (UART3_TX_PIN == C17) || (UART3_TX_PIN == E4) )         port_init( UART3_TX_PIN, ALT3);
        }break;
        
        case uart4:
        {
            SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;      //ʹ�� UART4 ʱ��
            if((UART4_RX_PIN == C14) || (UART4_RX_PIN == E25)  )                                port_init( UART4_RX_PIN, ALT3);
            if((UART4_TX_PIN == C15) || (UART4_TX_PIN == E24)  )                                port_init( UART4_TX_PIN, ALT3);
        }break;
        
        default:
        return;
    }
    
    //���õ�ʱ��Ӧ�ý�ֹ���ͺͽ���
    uart[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);
    
    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    uart[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������9λ��λ8��UARTx_C3�
                        //| UART_C1_PE_MASK                     //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C1_PT_MASK                     //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                       );
    
    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ��busʱ��
    if ((uartn == uart0) || (uartn == uart1)) uart_input_clk = core_clk_mhz * 1000000;  //�ں�ʱ��
    else                                            uart_input_clk = bus_clk_mhz * 1000000;   //busʱ��

    //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA/32))
    //������ BRFA ������£� SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = (uint16)(uart_input_clk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR �� 13bit�����Ϊ 0x1FFF

    //��֪ SBR ���� BRFA = UART ģ��ʱ�� * 2/ UART ������ - 32 ��SBR[12:0]
    brfa = (uart_input_clk * 2 / baud)  - (sbr * 32);
    if(brfa>0x1F)   brfa = 0x1F;
    
    //д SBR
    uart[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //���ԭ��������
    uart[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //��д��SBR��λ
    uart[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //��д��SBR��λ

    //д BRFD
    uart[uartn]->C4 &= ~UART_C4_BRFA_MASK;
    uart[uartn]->C4 |= UART_C4_BRFA(brfa);

    //����FIFO(FIFO���������Ӳ�������ģ������������)
    uart[uartn]->PFIFO |= (0
                           | UART_PFIFO_TXFE_MASK               //ʹ��TX FIFO(ע�ͱ�ʾ��ֹ)
                           //| UART_PFIFO_TXFIFOSIZE(0)         //��ֻ����TX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                           | UART_PFIFO_RXFE_MASK               //ʹ��RX FIFO(ע�ͱ�ʾ��ֹ)
                           //| UART_PFIFO_RXFIFOSIZE(0)         //��ֻ����RX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                           );

    // �����ͺͽ���
    
    uart[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //����ʹ��
                        | UART_C2_RE_MASK                       //����ʹ��
                        //| UART_C2_TIE_MASK                    //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_TCIE_MASK                   //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_RIE_MASK                    //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        );
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڷ���һ���ֽ�
//  @param      uartn           ѡ�񴮿�
//  @param      ch              Ҫ���͵��ַ�
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putchar(uart3,0x5a);   
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTn uartn, uint8 ch)
{
    while( !((uart[uartn]->S1) & UART_S1_TDRE_MASK) );  //�ȴ����ͻ�������
    uart[uartn]->D = (uint8)ch;                         //��������
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڷ�������
//  @param      uartn           ѡ�񴮿�
//  @param      *buff           Ҫ���͵������ַ
//  @param      len             ���ͳ���
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putbuff(uart3,&a[0],sizeof(a));  
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڷ����ַ���
//  @param      uartn           ѡ�񴮿�
//  @param      *str            Ҫ���͵��ַ�����ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_putstr(uart3,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڵȴ�����1���ַ�
//  @param      uartn           ѡ�񴮿�
//  @param      *str            ���յĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_getchar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
void uart_getchar (UARTn uartn, uint8 *ch) 
{
    while(!(uart[uartn]->S1 & UART_S1_RDRF_MASK));  //�ȴ���������   
    *ch =  uart[uartn]->D;                          // ��ȡ���յ���8λ����
    // ��ȡ 9λ����ʱch Ӧ����uint16 *���͵ķ������ݻ������
    // *ch =   ((( UARTx_C3_REG(UARTN[uartn]) & UART_C3_R8_MASK ) >> UART_C3_R8_SHIFT ) << 8)   |   UART_D_REG(UARTN[uartn]);  //����9bit
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���ڲ�ѯ����1���ַ�
//  @param      uartn           ѡ�񴮿�
//  @param      *str            ���յĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_querychar(uart3,&dat);
//-------------------------------------------------------------------------------------------------------------------
char uart_querychar (UARTn uartn, uint8 *ch)
{
    if( uart[uartn]->RCFIFO )         //��ѯ�Ƿ���ܵ�����
    {
        *ch  =   uart[uartn]->D;      //���ܵ�8λ������
        return  1;                    //���� 1 ��ʾ���ճɹ�
    }
    return 0;                         //����0��ʾ����ʧ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_RIE_MASK;                                        //ʹ��UART�����ж�
    if(uartn<=3)enable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));       //ʹ��IRQ�ж�
    else        enable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                       //ʹ��IRQ�ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_en(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_en(UARTn uartn)
{
    uart[uartn]->C2 |= UART_C2_TCIE_MASK;                                       //ʹ��UART��������ж�
    if(uartn<=3)enable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));       //ʹ��IRQ�ж�
    else        enable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                      //ʹ��IRQ�ж�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ֹ�����ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_rx_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_rx_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_RIE_MASK;                                       //��ֹUART�����ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(uart[uartn]->C2 & (UART_C2_TIE_MASK | UART_C2_TCIE_MASK)) )
    {
        if(uartn<=3)disable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //��IRQ�ж�
        else        disable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                 //��IRQ�ж�
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ֹ��������ж�
//  @param      uartn           ѡ�񴮿�
//  @return     void
//  @since      v1.0
//  Sample usage:               uart_txc_irq_dis(uart3);
//-------------------------------------------------------------------------------------------------------------------
void uart_txc_irq_dis(UARTn uartn)
{
    uart[uartn]->C2 &= ~UART_C2_TCIE_MASK;                                      //��ֹUART��������ж�

    //��������жϻ�û�йأ��򲻹ر�IRQ
    if(!(uart[uartn]->C2 & UART_C2_RIE_MASK) )
    {
        if(uartn<=3)disable_irq((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //��IRQ�ж�
        else        disable_irq((IRQn_Type)(UART4_RX_TX_IRQn));                 //��IRQ�ж�
    }
}

//�����жϺ����뵽isr.c�ļ��ο�


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ض���printf ������
//  @param      ch      ��Ҫ��ӡ���ֽ�
//  @param      stream  ������
//  @since      v1.0
//  @note       �˺����ɱ������Դ������printf������
//-------------------------------------------------------------------------------------------------------------------
int fputc(int ch, FILE *stream)
{
    uart_putchar(DEBUG_PORT, (char)ch);
    return(ch);
}

