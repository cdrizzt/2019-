#include "isr.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void PORTB_IRQHandler(void)
{
	uint8  n;    //引脚号
	uint32 flag;
	
	while(!PORTB->ISFR );
	flag = PORTB->ISFR;
	PORTB->ISFR  = ~0;                                   //清中断标志位
	
	n = 17;                                             //场中断
	if(flag & (1 << n))                                 //PTA29触发中断
	{
		VSYNC();                                        //传送图像函数
	}
	PORTB->ISFR  = ~0;                                   //清中断标志位
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA0中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当DMA传送结束后产生中断
//-------------------------------------------------------------------------------------------------------------------
void DMA0_IRQHandler(void)              
{
	cream_dma();                                    //MT9v032dma函数
}

#if chronography 	//计时
uint32 Time;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      计时中断
//  @return     void   
//  @since      v1.0
//  Sample usage:    计时中断
//-------------------------------------------------------------------------------------------------------------------
void PIT1_IRQHandler(void)
{
	disable_irq(PIT1_IRQn);     //关PIT中断
	PIT_FlAG_CLR(pit1);        //清空PIT0标志
	Time++;
	//		delay_ms(10);
	PIT_FlAG_CLR(pit1);        //清空PIT0标志
	enable_irq(PIT1_IRQn);	   //使能中断
}
#endif

//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART0中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:     摄像头用    当UART0启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
	if(UART3->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
	{
		//用户需要处理接收数据
		mt9v032_cof_uart_interrupt();
	}
	if(UART3->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
	{
		//用户需要处理发送数据
		
	}
}


void UART0_RX_TX_IRQHandler(void)
{
	if(UART0->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
	{
		//用户需要处理接收数据
		Rx_Distance();
	}
	
}
/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
;
}
记得进入中断后清除标志位


DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/



