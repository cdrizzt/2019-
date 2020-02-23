/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2016,��ɿƼ�
* All rights reserved.
* ��������QQȺ��179029047
*
* �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file       		SEEKFREE_MT9V032.c
* @brief      		�����(�Ҷ�����ͷ)������
* @company	   		�ɶ���ɿƼ����޹�˾
* @author     		Go For It(1325536866)
* @version    		v1.0
* @Software 		IAR 7.2 or MDK 5.17
* @Target core		MK60DN512VLL10
* @Taobao   		https://seekfree.taobao.com/
* @date       		2016-02-25
* @note	
MT9V032���߶��壺
------------------------------------ 
ģ��ܽ�            ��Ƭ���ܽ�
SDA(51��RX)         C17
SCL(51��TX)         C16
���ж�              C6
�����ж�            C18           
���ݿ�              C8-C15 
------------------------------------ 

Ĭ�Ϸֱ�����            188*120
Ĭ��FPS                 50֡
********************************************************************************************************************/



#include "SEEKFREE_MT9V032.h"


#define MT9V032_COF_UART    uart3


uint8   image[ROW][COL];      //ͼ������
uint8   receive[3];
uint8   receive_num = 0;
uint8   uart_receive_flag = 1;
uint8   mt9v032_finish_flag;  //һ��ͼ��ɼ���ɱ�־λ

uint8 SendHead[4] = {0xFF, 0xFA, 0xFD, 0xF6};
void TransToU16(uint8 data[], uint16 n)
{
    if(n >= 256)
        data[0] = n / 256;
    else
        data[0] = 0;
    data[1] = n - 256 * (uint16)data[0];
}


volatile IMG_STATUS_e      MT9V032_img_flag = IMG_FINISH;
#define EXP_Time_Initial	180 //150֡ 210�ع�
uint16 EXP_Time=150;
//��Ҫ���õ�����ͷ������
//��Ҫ���õ�����ͷ������
int16 MT9V032_CFG[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      ��Χ1-63 0Ϊ�ر� ����Զ��ع⿪��  EXP_TIME�������õ����ݽ����Ϊ����ع�ʱ�䣬Ҳ�����Զ��ع�ʱ�������
    //һ������ǲ���Ҫ����������ܣ���Ϊ�������ع���һ�㶼�ȽϾ��ȣ�����������߷ǳ������ȵ�������Գ������ø�ֵ������ͼ���ȶ���
    {EXP_TIME,          EXP_Time_Initial}, //�ع�ʱ��          ����ͷ�յ�����Զ����������ع�ʱ�䣬������ù���������Ϊ�������������ع�ֵ
    {FPS,               150},  //ͼ��֡��          ����ͷ�յ�����Զ���������FPS���������������Ϊ������������FPS
    {SET_COL,           COL}, //ͼ��������        ��Χ1-752     K60�ɼ���������188
    {SET_ROW,           ROW}, //ͼ��������        ��Χ1-480
   
#if CarMode==AE87
	{UD_OFFSET,         52},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
	{LR_OFFSET,         22},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
#else
	{UD_OFFSET,         31},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ120 240 480ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
	{LR_OFFSET,         -36},   //ͼ������ƫ����    ��ֵ ��ƫ��   ��ֵ ��ƫ��  ��Ϊ188 376 752ʱ�޷�����ƫ��    ����ͷ��ƫ�����ݺ���Զ��������ƫ�ƣ�������������ü�����������ƫ��
#endif
	{GAIN,              64},  //ͼ������          ��Χ16-64     ����������ع�ʱ��̶�������¸ı�ͼ�������̶�
	
    
    {INIT,              0}    //����ͷ��ʼ��ʼ��
};

//������ͷ�ڲ���ȡ������������
int16 GET_CFG[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},   //�Զ��ع�����      
    {EXP_TIME,          0},   //�ع�ʱ��          
    {FPS,               0},   //ͼ��֡��          
    {SET_COL,           0},   //ͼ��������        
    {SET_ROW,           0},   //ͼ��������        
    {LR_OFFSET,         0},   //ͼ������ƫ����    
    {UD_OFFSET,         0},   //ͼ������ƫ����    
    {GAIN,              0},   //ͼ������          
};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ�����жϺ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	
//  @note       �ú�����ISR�ļ� ����3�жϳ��򱻵���
//-------------------------------------------------------------------------------------------------------------------
void mt9v032_cof_uart_interrupt(void)
{
    uart_getchar(MT9V032_COF_UART,&receive[receive_num]);;
    receive_num++;
    
    if(1==receive_num && 0XA5!=receive[0])  receive_num = 0;
    if(3 == receive_num)
    {
        receive_num = 0;
        uart_receive_flag = 1;
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�ڲ�������Ϣ
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
    uint16 temp, i;
    uint8  send_buffer[4];
    
    for(i=0; i<CONFIG_FINISH-1; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = GET_CFG[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        
        //�ȴ����ܻش�����
        while(!uart_receive_flag);
        uart_receive_flag = 0;
        
        GET_CFG[i][1] = receive[1]<<8 | receive[2];
        
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͷ�̼��汾
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				���øú���ǰ���ȳ�ʼ��uart2
//-------------------------------------------------------------------------------------------------------------------
uint16 get_version(void)
{
    uint16 temp;
    uint8  send_buffer[4];
    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    return ((uint16)(receive[1]<<8) | receive[2]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������ͷ�ع�ʱ��
//  @param      light   �����ع�ʱ��Խ��ͼ��Խ��������ͷ�յ������ݷֱ��ʼ�FPS��������ع�ʱ��������õ����ݹ�����ô����ͷ��������������ֵ
//  @return     uint16  ��ǰ�ع�ֵ������ȷ���Ƿ���ȷд��
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_exposure_time(uint16 light)
{
    uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ͷ�ڲ��Ĵ�������д����
//  @param      addr    ����ͷ�ڲ��Ĵ�����ַ
//  @param      data    ��Ҫд�������
//  @return     uint16  �Ĵ�����ǰ���ݣ�����ȷ���Ƿ�д��ɹ�
//  @since      v1.0
//  Sample usage:				
//-------------------------------------------------------------------------------------------------------------------
uint16 set_mt9v032_reg(uint8 addr, uint16 data)
{
    uint16 temp;
    uint8  send_buffer[4];
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    systick_delay_ms(10);
    
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8)temp;
    
    uart_putbuff(MT9V032_COF_UART,send_buffer,4);
    
    //�ȴ����ܻش�����
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    
    temp = receive[1]<<8 | receive[2];
    return temp;
    
}

uint16 version;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ��ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		
//-------------------------------------------------------------------------------------------------------------------
void camera_init(void)
{
    //���ò���    ������ο�ʹ���ֲ�
    uint16 temp, i;
    uint8  send_buffer[4];
    
    uart_init (MT9V032_COF_UART, 9600);	//��ʼ������ ��������ͷ    
    uart_rx_irq_en(MT9V032_COF_UART);   //���ô����ж�
    EnableInterrupts;                   //�����жϵȴ�51��������
    
    //�ȴ�����ͷ�ϵ��ʼ���ɹ�
    systick_delay_ms(1000);
    uart_receive_flag = 0;
	
	EXP_Time=flash_read(252,0,uint16);
	if(EXP_Time==0xffff)
	{
		while(FLASH_EraseSector(252));	//��������
		systick_delay_ms(10);
		EXP_Time=EXP_Time_Initial;
		FLASH_WriteSector(252,(const uint8 *)&EXP_Time,16,0);//ȷ������flash	253 254�������ڵ�� 253������������ͷ�ع�
	}

     MT9V032_CFG[1][1]=EXP_Time;//EXP_Time;//�����ع�
	 
    //��ʼ��������ͷ�����³�ʼ��
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = MT9V032_CFG[i][0];
        temp = MT9V032_CFG[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8)temp;
        
        uart_putbuff(MT9V032_COF_UART,send_buffer,4);
        systick_delay_ms(2);
    }
    //�ȴ�����ͷ��ʼ���ɹ�
    while(!uart_receive_flag);
    uart_receive_flag = 0;
    while((0xff != receive[1]) || (0xff != receive[2]));
    //���ϲ��ֶ�����ͷ���õ�����ȫ�����ᱣ��������ͷ��51��Ƭ����eeprom��
    //����set_exposure_time�����������õ��ع����ݲ��洢��eeprom��
    
    
    //��ȡ���ñ��ڲ鿴�����Ƿ���ȷ
    get_config();
    
    
    //����ͷ�ɼ���ʼ��
    DisableInterrupts;          //�ر������ж�����DMA0��PORTA
    //DMAͨ��0��ʼ����PTA27����Դ(Ĭ��������)��Դ��ַΪB_IN_DATA(0)(PTB1-PTB8)��Ŀ�ĵ�ַΪ��image��ÿ�δ���1Byte ������ϱ���Ŀ�ĵ�ַ
    
    dma_portx2buff_init(DMA_CH0, (void *)&B_IN_DATA(0), (void *)image, A29, DMA_BYTE1, COL*ROW, DADDR_KEEPON);
    port_init(A29, ALT1 | DMA_RISING | PULLDOWN);  			//PCLK  ����Դ����
    DMA_DIS(DMA_CH0);                                     		//����DMAͨ��
    DMA_IRQ_CLEAN(DMA_CH0);                               		//���ͨ�������жϱ�־λ
    DMA_IRQ_EN(DMA_CH0);                                  		//����DMAͨ���ж�
    DMA_EN(DMA_CH0);  											//ʹ��DMA
    
    disable_irq(PORTB_IRQn);                             		//�ر�PTC���ж�
    //port_init(C7, ALT1 | IRQ_FALLING | PULLDOWN);      			//���ж�
    port_init(B17, ALT1 | IRQ_FALLING | PULLDOWN);              //���жϣ��½��ش����жϡ�����
    enable_irq (PORTB_IRQn);
    
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷ���ж�
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void VSYNC(void)
{
    if(MT9V032_img_flag == IMG_START)                   //��Ҫ��ʼ�ɼ�ͼ��
    {
		disable_irq(PORTB_IRQn);
		MT9V032_img_flag = IMG_GATHER;                  //���ͼ��ɼ���
		dma_repeat(DMA_CH0,(void *)&B_IN_DATA(0),(void *)image[0],COL*ROW);
		
	}
	else                                        //ͼ��ɼ�����
	{
		disable_irq(PORTB_IRQn);                        //�ر�PTA���ж�
		MT9V032_img_flag = IMG_FAIL;                    //���ͼ��ɼ�ʧ��
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ͷͼ��������λ���鿴ͼ��
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:		���øú���ǰ���ȳ�ʼ��uart4
//-------------------------------------------------------------------------------------------------------------------
void seekfree_sendimg_032(uint8  *image1)//���
{	//ͨѶЭ��
    uart_putchar(uart4,0x00);uart_putchar(uart4,0xff);uart_putchar(uart4,0x01);uart_putchar(uart4,0x01);//��������
    uart_putbuff(uart4, image1, ROW*COL);  //����ͼ��
    
}
//����
void SendPicData(UARTn uartn, uint8 * data, uint16 size, uint16 width, uint16 height)
{
    uint8 psize[2] = {0};
    uart_putbuff(uartn, SendHead, 4);
    uart_putchar(uartn, 250);
    TransToU16(psize, size + 4);
    uart_putbuff(uartn, psize, sizeof(psize));
    TransToU16(psize, width);
    uart_putbuff(uartn, psize, sizeof(psize));
    TransToU16(psize, height);
    uart_putbuff(uartn, psize, sizeof(psize));
    uart_putbuff(uartn, data, size);
    uart_putchar(uartn, 0x0A); ////���һλУ��
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ɼ�һ��ͼ��
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				�ɼ������鱣����image��
//-------------------------------------------------------------------------------------------------------------------
void Camera_Get_image(void)
{
    MT9V032_img_flag = IMG_START;
    PORTB->ISFR = ~0;                        //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
    enable_irq(PORTB_IRQn);
    
    while(MT9V032_img_flag!= IMG_FINISH)            //�ȴ�ͼ��ɼ����
    {
        if(MT9V032_img_flag == IMG_FAIL)            //����ͼ��ɼ����������¿�ʼ�ɼ�
        {
            MT9V032_img_flag = IMG_START;           //��ʼ�ɼ�ͼ��
            PORTB->ISFR = ~0;                       //д1���жϱ�־λ(����ģ���Ȼ�ص���һ���жϾ����ϴ����ж�)
            enable_irq(PORTB_IRQn);                 //����PTA���ж�
        }
    }
    
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V032����ͷDMA����
//  @param      NULL
//  @return     void			
//  @since      v1.0
//  Sample usage:				��isr.c�����ȴ�����Ӧ���жϺ�����Ȼ����øú���(֮�����������жϱ�־λ)
//-------------------------------------------------------------------------------------------------------------------
void cream_dma(void)
{
    MT9V032_img_flag= IMG_FINISH;
    DMA_IRQ_CLEAN(DMA_CH0);
    //һ��ͼ��Ӳɼ���ʼ���ɼ�������ʱ3.33MS����(50FPS��188*120�ֱ���)
}
