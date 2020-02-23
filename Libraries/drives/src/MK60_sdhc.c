/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		MK60_sdhc
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v2.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 ********************************************************************************************************************/
 

 
#include "MK60_sdhc.h"

#define TRUE    1
#define FALSE	0


//SD����Ϣȫ�ֱ���
SDCARD_STRUCT_PTR sdcard_ptr;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDHCģ����ص�GPIO���ų�ʼ��
//  @param      init    PCR�Ĵ�������
//  @return     void   
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static void sdhc_initgpio(uint32 init)
{  
    port_init (E0, init & (ALT4 | PULLUP | HDS));
    port_init (E1, init & (ALT4 | PULLUP | HDS));
    port_init (E2, init & (ALT4 | HDS));
    port_init (E3, init & (ALT4 | PULLUP | HDS));
    port_init (E4, init & (ALT4 | PULLUP | HDS));
    port_init (E5, init & (ALT4 | PULLUP | HDS));
  

    SIM->SCGC3 |= SIM_SCGC3_SDHC_MASK;

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����SDHC������
//  @param      baud    SDHC����ʱ��Ƶ�ʣ���λHz
//  @return     void   
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static void sdhc_setbaudrate(uint32 baud)
{
    uint32 pres, div, min, minpres = 0x80, mindiv = 0x0F;
    int32  val;
    uint32 clock = bus_clk_mhz*1000*1000;
    //�ҵ�����ķ�Ƶ����
    min = (uint32)-1;
    for (pres = 2; pres <= 256; pres <<= 1)
    {
        for (div = 1; div <= 16; div++)
        {
            val = pres * div * baud - clock;
            if (val >= 0)
            {
                if (min > val)
                {
                  min = val;
                  minpres = pres;
                  mindiv = div;
                }
            }
        }
    }
    
    //��ֹSDHCģ��ʱ��
    SDHC->SYSCTL &= (~ SDHC_SYSCTL_SDCLKEN_MASK);
    
    //�޸ķ�Ƶ����
    div = SDHC->SYSCTL & (~ (SDHC_SYSCTL_DTOCV_MASK | SDHC_SYSCTL_SDCLKFS_MASK | SDHC_SYSCTL_DVS_MASK));
    SDHC->SYSCTL = div | (SDHC_SYSCTL_DTOCV(0x0E) | SDHC_SYSCTL_SDCLKFS(minpres >> 1) | SDHC_SYSCTL_DVS(mindiv - 1));
    
    //����ʱ���ȶ�
    while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_SDSTB_MASK));

    //ʹ��SDHCģ��ʱ��
    SDHC->SYSCTL |= SDHC_SYSCTL_SDCLKEN_MASK;
    SDHC->IRQSTAT |= SDHC_IRQSTAT_DTOE_MASK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡSDHCģ������״̬
//  @return     void   
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static uint8 sdhc_isrunning(void)
{
    return (0 != (SDHC->PRSSTAT & (SDHC_PRSSTAT_RTA_MASK | SDHC_PRSSTAT_WTA_MASK | SDHC_PRSSTAT_DLA_MASK | SDHC_PRSSTAT_CDIHB_MASK | SDHC_PRSSTAT_CIHB_MASK)));
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ȴ�ָ��״̬��־λ��λ
//  @param      mask    ״̬��־λ����
//  @return     void   
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static uint32 sdhc_waitstatus(uint32 mask)
{
    uint32 result;
    do
    {
        result = SDHC->IRQSTAT & mask;
    }
    while (0 == result);
    return result;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDHCģ���ʼ������
//  @param      baud    SDHC����ʱ��Ƶ�ʣ���λHz
//  @return     SDHCSTA_OK--״̬����
//              SDHCSTA_NOINIT--����δ��ʼ��
//              SDHCSTA_NODISK--Ϊ���뿨
//              SDHCSTA_PROTECT--��д����
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static SDHCRES sdhc_init(uint32 baud)
{
  
    sdcard_ptr->CARD = ESDHC_CARD_NONE;
    
    //����GPIO��SDHC���ù���
    sdhc_initgpio (0);
    
    //��λSDHCģ��
    SDHC->SYSCTL = SDHC_SYSCTL_RSTA_MASK | SDHC_SYSCTL_SDCLKFS(0x80);
    while (SDHC->SYSCTL & SDHC_SYSCTL_RSTA_MASK){};
    
    //��ʼ���Ĵ���ֵ
    SDHC->VENDOR = 0;
    SDHC->BLKATTR = SDHC_BLKATTR_BLKCNT(1) | SDHC_BLKATTR_BLKSIZE(512);
    SDHC->PROCTL = SDHC_PROCTL_EMODE(ESDHC_PROCTL_EMODE_LITTLE) | SDHC_PROCTL_D3CD_MASK;
    SDHC->WML = SDHC_WML_RDWML(2) | SDHC_WML_WRWML(1);
    
    //����SDHC��ʼ��ʱ�ӣ���ò�Ҫ����400kHz
    sdhc_setbaudrate (baud);
    
    //�ȴ�
    while (SDHC->PRSSTAT & (SDHC_PRSSTAT_CIHB_MASK | SDHC_PRSSTAT_CDIHB_MASK)){};
    
    //ʹ��GPIO��SDHC����
    sdhc_initgpio (0xFFFF);
    
    //ʹ�ܸ�������
    SDHC->IRQSTAT = 0xFFFF;
    SDHC->IRQSTATEN = SDHC_IRQSTATEN_DEBESEN_MASK | SDHC_IRQSTATEN_DCESEN_MASK | SDHC_IRQSTATEN_DTOESEN_MASK
      | SDHC_IRQSTATEN_CIESEN_MASK | SDHC_IRQSTATEN_CEBESEN_MASK | SDHC_IRQSTATEN_CCESEN_MASK | SDHC_IRQSTATEN_CTOESEN_MASK
        | SDHC_IRQSTATEN_BRRSEN_MASK | SDHC_IRQSTATEN_BWRSEN_MASK | SDHC_IRQSTATEN_CRMSEN_MASK
          | SDHC_IRQSTATEN_TCSEN_MASK | SDHC_IRQSTATEN_CCSEN_MASK;
    
    //�ȴ�80����ʼʱ��
    SDHC->SYSCTL |= SDHC_SYSCTL_INITA_MASK;
    while (SDHC->SYSCTL & SDHC_SYSCTL_INITA_MASK){};
    
    //��鿨�Ƿ����
    if (SDHC->PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
    {
        sdcard_ptr->CARD = ESDHC_CARD_UNKNOWN;
    }
    else
    {
        sdcard_ptr->STATUS = SDHCSTA_NODISK;
    }
    SDHC->IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
    
    return SDHCRES_OK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��SD������ָ��CMD����
//  @param      command    SDHC������Ϣ�ṹ��
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static SDHCRES sdhc_sendcommand(ESDHC_COMMAND_STRUCT_PTR command)
{
    uint32 xfertyp;
    uint32 blkattr;
    
    //�������
    xfertyp = command->COMMAND;
    
    if (ESDHC_XFERTYP_CMDTYP_RESUME == ((xfertyp & SDHC_XFERTYP_CMDTYP_MASK) >> SDHC_XFERTYP_CMDTYP_SHIFT))
    {
      //�ָ����������������DPSELλ
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
    }
    
    if ((0 != command->BLOCKS) && (0 != command->BLOCKSIZE))
    {
        xfertyp |= SDHC_XFERTYP_DPSEL_MASK;
        if (command->BLOCKS != 1)
        {
          //��鴫��
            xfertyp |= SDHC_XFERTYP_MSBSEL_MASK;
        }
        if ((uint32)-1 == command->BLOCKS)
        {
          //��������
            blkattr = SDHC_BLKATTR_BLKSIZE(command->BLOCKSIZE) | SDHC_BLKATTR_BLKCNT(0xFFFF);
        }
        else
        {
            blkattr = SDHC_BLKATTR_BLKSIZE(command->BLOCKSIZE) | SDHC_BLKATTR_BLKCNT(command->BLOCKS);
            xfertyp |= SDHC_XFERTYP_BCEN_MASK;
        }
    }
    else
    {
        blkattr = 0;
    }
    
    //���Ƴ�״̬���
    SDHC->IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
    
    //�ȴ�CMD�߿���
    while (SDHC->PRSSTAT & SDHC_PRSSTAT_CIHB_MASK);
    
    //��ʼ������
    SDHC->CMDARG = command->ARGUMENT;
    SDHC->BLKATTR = blkattr;
    SDHC->DSADDR = 0;
    
    //��������
    SDHC->XFERTYP = xfertyp;
    
    //�ȴ���Ӧ
    if (sdhc_waitstatus (SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK) != SDHC_IRQSTAT_CC_MASK)
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CIE_MASK | SDHC_IRQSTAT_CEBE_MASK | SDHC_IRQSTAT_CCE_MASK | SDHC_IRQSTAT_CC_MASK;
        return SDHCRES_ERROR;
    }
    
    //��鿨�Ƿ��Ƴ�
    if (SDHC->IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        sdcard_ptr->STATUS = SDHCSTA_NODISK;
        return SDHCRES_NOTRDY;
    }
    
    //��ȡ��Ӧ
    if (SDHC->IRQSTAT & SDHC_IRQSTAT_CTOE_MASK)
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_CTOE_MASK | SDHC_IRQSTAT_CC_MASK;
        return SDHCRES_NONRSPNS;
    }
    if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) != SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_NO))
    {
        command->RESPONSE[0] = SDHC->CMDRSP[0];
        if ((xfertyp & SDHC_XFERTYP_RSPTYP_MASK) == SDHC_XFERTYP_RSPTYP(ESDHC_XFERTYP_RSPTYP_136))
        {
            command->RESPONSE[1] = SDHC->CMDRSP[1];
            command->RESPONSE[2] = SDHC->CMDRSP[2];
            command->RESPONSE[3] = SDHC->CMDRSP[3];
        }
    }
    SDHC->IRQSTAT |= SDHC_IRQSTAT_CC_MASK;
    
    return SDHCRES_OK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDHCģ���������Ʒ�����
//  @param      cmd             SDHCģ���������
//  @param      *param_ptr      ���Ʋ���
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       
//-------------------------------------------------------------------------------------------------------------------
SDHCRES sdhc_ioc(uint32 cmd, void *param_ptr)
{
  
    ESDHC_COMMAND_STRUCT    command;
    uint8                 mem, mp, hc;
    int32                  val;
    SDHCRES                 result = SDHCRES_OK;
    uint32 *             param32_ptr = param_ptr;
    
    switch (cmd)
    {
      case IO_IOCTL_ESDHC_INIT:  
        //��ʼ��SDHCģ��
        result = sdhc_init (400000);
        if (SDHCRES_OK != result)
        {
            break;
        }
        
        mem = FALSE;
        hc = FALSE;
        mp = FALSE;
        
        //CMD0 - ���������λ��
        command.COMMAND = ESDHC_CMD0;
        command.ARGUMENT = 0;
        command.BLOCKS = 0;
        result = sdhc_sendcommand (&command);
        if (result!=SDHCRES_OK)
        {
            sdcard_ptr->STATUS = SDHCSTA_NOINIT;
            break;
        }
        
        //CMD8 - ���ͽӿ�״̬������Ƿ�֧�ָ�����
        command.COMMAND = ESDHC_CMD8;
        command.ARGUMENT = 0x000001AA;
        command.BLOCKS = 0;
        result = sdhc_sendcommand (&command);
        if (result==SDHCRES_ERROR)
        {
            sdcard_ptr->STATUS = SDHCSTA_NOINIT;
            break;
        }
        if (result == SDHCRES_OK)
        {
            if (command.RESPONSE[0] != command.ARGUMENT)
            {
                sdcard_ptr->STATUS = SDHCSTA_NOINIT;
                result = SDHCRES_ERROR;
                break;
            }
            hc = TRUE;
        }
        
        //CMD5 - ���Ͳ���״̬������IO
        command.COMMAND = ESDHC_CMD5;
        command.ARGUMENT = 0;
        command.BLOCKS = 0;      
        result = sdhc_sendcommand (&command);
        if (result==SDHCRES_ERROR)
        {
            sdcard_ptr->STATUS = SDHCSTA_NOINIT;
            break;
        }
        if (result == SDHCRES_OK)
        {
            if (((command.RESPONSE[0] >> 28) & 0x07) && (command.RESPONSE[0] & 0x300000))
            {
                command.COMMAND = ESDHC_CMD5;
                command.ARGUMENT = 0x300000;
                command.BLOCKS = 0;
                val = 0;
                do
                {
                    val++;
                    result = sdhc_sendcommand (&command);
                    if (result)
                    {
                        break;
                    }
                } while ((0 == (command.RESPONSE[0] & 0x80000000)) && (val < ESDHC_ALARM_FREQUENCY));
                if (SDHCRES_OK != result)
                {
                    break;
                }
                if (command.RESPONSE[0] & 0x08000000)
                {
                    mp = TRUE;
                }
            }
        }
        else
        {
            mp = TRUE;
        }
        
        if (mp)
        {
          //CMD55 - ����Ӧ��������MMC��
          command.COMMAND = ESDHC_CMD55;
          command.ARGUMENT = 0;
          command.BLOCKS = 0;
          result = sdhc_sendcommand (&command);
          if (result==SDHCRES_ERROR)
          {
              break;
          }
          if (result != SDHCRES_NONRSPNS)
          {
            //���ΪSD��
            //ACMD41 - ���Ͳ���״̬
            command.COMMAND = ESDHC_ACMD41;
            command.ARGUMENT = 0;
            command.BLOCKS = 0;
            result = sdhc_sendcommand (&command);
            if (result)
            {
                sdcard_ptr->STATUS = SDHCSTA_NOINIT;
                break;
            }
            if (command.RESPONSE[0] & 0x300000)
            {
              val = 0;
              do
              {
                  val++;
                  
                  //CMD55 + ACMD41 - ����OCR
                  command.COMMAND = ESDHC_CMD55;
                  command.ARGUMENT = 0;
                  command.BLOCKS = 0;
                  result = sdhc_sendcommand (&command);
                  if (result)
                  {
                      break;
                  }
                  
                  command.COMMAND = ESDHC_ACMD41;
                  if (hc)
                  {
                      command.ARGUMENT = 0x40300000;
                  }
                  else
                  {
                      command.ARGUMENT = 0x00300000;
                  }
                  command.BLOCKS = 0;
                  result = sdhc_sendcommand (&command);
                  if (result)
                  {
                      break;
                  }
              } while ((0 == (command.RESPONSE[0] & 0x80000000)) && (val < ESDHC_ALARM_FREQUENCY));
              if (SDHCRES_OK != result)
              {
                  break;
              }
              if (val >= ESDHC_ALARM_FREQUENCY)
              {
                  hc = FALSE;
              }
              else
              {
                  mem = TRUE;
                  if (hc)
                  {
                      hc = FALSE;
                      if (command.RESPONSE[0] & 0x40000000)
                      {
                          hc = TRUE;
                      }
                  }
              }
            }
          }
        }
        if (mem)
        {
            sdcard_ptr->CARD = ESDHC_CARD_SD;
            if (hc)
            {
                sdcard_ptr->CARD = ESDHC_CARD_SDHC;
            }
        }

        
        //����GPIO��SDHC����
        //sdhc_initgpio (0);
        
        //����SDHC����״̬�µ�Ĭ�ϲ�����
        sdhc_setbaudrate (25000000);
        
        //ʹ��GPIO��SDHC����
        sdhc_initgpio (0xFFFF);
        
        if(result == SDHCRES_OK)
        {
            sdcard_ptr->STATUS = SDHCSTA_OK;
        }
        break;
        
      case IO_IOCTL_ESDHC_SEND_COMMAND:
        result = sdhc_sendcommand ((ESDHC_COMMAND_STRUCT_PTR)param32_ptr);
        break;
        
      case IO_IOCTL_ESDHC_GET_BAUDRATE:
        if (NULL == param32_ptr)
        {
            result = SDHCRES_ERROR;
        }
        else
        {
            //��ȡ������
            val = ((SDHC->SYSCTL & SDHC_SYSCTL_SDCLKFS_MASK) >> SDHC_SYSCTL_SDCLKFS_SHIFT) << 1;
            val *= ((SDHC->SYSCTL & SDHC_SYSCTL_DVS_MASK) >> SDHC_SYSCTL_DVS_SHIFT) + 1;
            *param32_ptr = (uint32)((bus_clk_mhz*1000*1000 / val));
        }
        break;
        
      case IO_IOCTL_ESDHC_SET_BAUDRATE:
        if (NULL == param32_ptr)
        {
            result = SDHCRES_ERROR;
        }
        else if (0 == (*param32_ptr))
        {
            result = SDHCRES_ERROR;
        }
        else
        {
            if (! sdhc_isrunning ())
            {
                //����GPIO��SDHC����
                sdhc_initgpio (0);
                
                //���ò�����
                sdhc_setbaudrate (*param32_ptr);
                
                //ʹ��GPIO��SDHC����
                sdhc_initgpio (0xFFFF);
            }
            else
            {
                result = SDHCRES_ERROR;
            }
        }
        break;
        
      case IO_IOCTL_ESDHC_GET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = SDHCRES_ERROR;
        }
        else
        {
            //���SDHC���߿��
            val = (SDHC->PROCTL & SDHC_PROCTL_DTW_MASK) >> SDHC_PROCTL_DTW_SHIFT;
            if (ESDHC_PROCTL_DTW_1BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_1BIT;
            }
            else if (ESDHC_PROCTL_DTW_4BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_4BIT;
            }
            else if (ESDHC_PROCTL_DTW_8BIT == val)
            {
                *param32_ptr = ESDHC_BUS_WIDTH_8BIT;
            }
            else
            {
                result = SDHCRES_ERROR;
            }
        }
        break;
        
      case IO_IOCTL_ESDHC_SET_BUS_WIDTH:
        if (NULL == param32_ptr)
        {
            result = SDHCRES_ERROR;
        }
        else
        {
            //����SDHC���߿��
            if (! sdhc_isrunning ())
            {
                if (ESDHC_BUS_WIDTH_1BIT == *param32_ptr)
                {
                    SDHC->PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC->PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_1BIT);
                }
                else if (ESDHC_BUS_WIDTH_4BIT == *param32_ptr)
                {
                    SDHC->PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC->PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_4BIT);
                }
                else if (ESDHC_BUS_WIDTH_8BIT == *param32_ptr)
                {
                    SDHC->PROCTL &= (~ SDHC_PROCTL_DTW_MASK);
                    SDHC->PROCTL |= SDHC_PROCTL_DTW(ESDHC_PROCTL_DTW_8BIT);
                }
                else
                {
                    result = SDHCRES_ERROR;
                }
            }
            else
            {
                result = SDHCRES_ERROR;
            }
        }
        break;
        
      case IO_IOCTL_ESDHC_GET_CARD:
        if (NULL == param32_ptr)
        {
            result = SDHCRES_ERROR;
        }
        else
        {
            //�ȴ�80��ʱ��
            SDHC->SYSCTL |= SDHC_SYSCTL_INITA_MASK;
            while (SDHC->SYSCTL & SDHC_SYSCTL_INITA_MASK);
            
            //���²����ؿ�ʵ��״̬
            if (SDHC->IRQSTAT & SDHC_IRQSTAT_CRM_MASK)
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_CRM_MASK;
                sdcard_ptr->CARD = ESDHC_CARD_NONE;
                sdcard_ptr->STATUS = SDHCSTA_NODISK;
            }
            if (SDHC->PRSSTAT & SDHC_PRSSTAT_CINS_MASK)
            {
                if (ESDHC_CARD_NONE == sdcard_ptr->CARD)
                {
                    sdcard_ptr->CARD = ESDHC_CARD_UNKNOWN;
                }
            }
            else
            {
                sdcard_ptr->CARD = ESDHC_CARD_NONE;
            }
            *param32_ptr = sdcard_ptr->CARD;
        }
        break;
        
      case IO_IOCTL_FLUSH_OUTPUT:
        //�ȴ��������
        sdhc_waitstatus (SDHC_IRQSTAT_TC_MASK);
        if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
            result = SDHCRES_ERROR;
        }
        SDHC->IRQSTAT |= SDHC_IRQSTAT_TC_MASK | SDHC_IRQSTAT_BRR_MASK | SDHC_IRQSTAT_BWR_MASK;
        break;
        
        default:
        result = SDHCRES_ERROR;
        break;
    }
    
    
    return result;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDHC������
//  @param      *data_ptr     �洢���ݵ�ַָ��
//  @param      n             ���������ݳ���
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static SDHCSTATUS sdhc_read(uint8 *data_ptr, int32 n)
{
    uint32 buffer;
    int32 remains;
    
    remains = n;
    if (((uint32)data_ptr & 0x03) == 0)
    {    
        //����λ�ֶ��룬�����������ٶ�ֱ�ӴӼĴ�������
        while (remains >= 4)
        {
            if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return SDHCRES_ERROR;
            }
            
            //�ȴ���ֱ���յ������ݴﵽˮӡ���Ȼ������
            while ((0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) && (SDHC->PRSSTAT & SDHC_PRSSTAT_DLA_MASK));
            
            *((uint32 *)data_ptr) = SDHC->DATPORT;
            data_ptr += 4;
            remains -= 4;
        }
    }
    else
    {
        //�Ƕ������ݣ�������ʱ�������ֽڸ���
        while (remains >= 4)
        {
            if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return SDHCRES_ERROR;
            }
            
            //�ȴ���ֱ���յ������ݴﵽˮӡ���Ȼ������
            while ((0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) && (SDHC->PRSSTAT & SDHC_PRSSTAT_DLA_MASK));
            
            buffer = SDHC->DATPORT;
            
            *data_ptr++ = buffer & 0xFF;
            *data_ptr++ = (buffer >> 8) & 0xFF;
            *data_ptr++ = (buffer >> 16) & 0xFF;
            *data_ptr++ = (buffer >> 24) & 0xFF;
            
            remains -= 4;
        }      
    }
    
    if (remains)
    {
        //ʣ�µ����ڵ��ֳ�������
        if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
            return SDHCRES_ERROR;
        }
        
        //�ȴ���ֱ���յ������ݴﵽˮӡ���Ȼ������
        while ((0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) && (SDHC->PRSSTAT & SDHC_PRSSTAT_DLA_MASK));
        
        buffer = SDHC->DATPORT;
        while (remains)
        {
          
            *data_ptr++ = buffer & 0xFF;
            buffer >>= 8;
            
            remains--;
        }
    }
    
    if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
        return SDHCRES_ERROR;
    }
    
    return (n - remains);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDHC������
//  @param      *data_ptr     �洢���ݵ�ַָ��
//  @param      n             ��д�����ݳ���
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       �ڲ�ʹ�ã��û��������
//-------------------------------------------------------------------------------------------------------------------
static SDHCSTATUS sdhc_write(uint8 *data_ptr, int32 n)
{
    uint8 *udata_ptr;
    uint32 buffer;
    int32 remains;
    
    //��������ָ��
    udata_ptr = (uint8 *)data_ptr;
    
    remains = n;
    if (((uint32)udata_ptr & 0x03) == 0)
    {
        //����λ�ֶ��룬�����������ٶ�ֱ�ӿ������Ĵ���
        while (remains >= 4)
        {
            if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return SDHCRES_ERROR;
            }
            
            //�ȴ���ֱ��ˮӡ�ռ���� 
            while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK));
            
            SDHC->DATPORT = *((uint32 *)udata_ptr);
            udata_ptr += 4;
            remains -= 4;
        }
    }
    else
    {
        //�Ƕ������ݣ�д����ʱ�������ֽڸ���
        while (remains >= 4)
        {
            if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return SDHCRES_ERROR;
            }
            
            //�ȴ���ֱ��ˮӡ�ռ���� 
            while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK));
            
            buffer  = (*udata_ptr++);
            buffer |= (*udata_ptr++) << 8;
            buffer |= (*udata_ptr++) << 16;
            buffer |= (*udata_ptr++) << 24;
            
            //�ȴ���ֱ��ˮӡ�ռ���� 
            while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK));
            
            SDHC->DATPORT = buffer;
            remains -= 4;
        }      
    }
    
    if (remains)
    {
        //ʣ�����ڵ��ֳ��ȵ�����
        if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
        {
            SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
            return SDHCRES_ERROR;
        }
        
        buffer = 0xFFFFFFFF;
        while (remains)
        {
            buffer <<= 8;
            buffer |= udata_ptr[remains];
            remains--;
        }
        
        //�ȴ���ֱ��ˮӡ�ռ���� 
        while (0 == (SDHC->PRSSTAT & SDHC_PRSSTAT_BWEN_MASK));
        
        SDHC->DATPORT = buffer;        
    }
    
    if (SDHC->IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
    {
        SDHC->IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
        return SDHCRES_ERROR;
    }
    
    return (n - remains);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʼ��SDHCģ�鼰SD����������������������Ϊ25MHz
//  @return     SDHCSTA_OK--״̬����
//              SDHCSTA_NOINIT--����δ��ʼ��
//              SDHCSTA_NODISK--δ���뿨
//              SDHCSTA_PROTECT--��д����
//  @since      v1.0
//  @note       
//-------------------------------------------------------------------------------------------------------------------
SDHCSTATUS sdhc_initcard(void)
{
    uint32 param, c_size, c_size_mult, read_bl_len, time_out = 0;
    ESDHC_COMMAND_STRUCT command;
    SDHCSTATUS result;
    
    //����SD����Ϣ�ṹ������ݿռ䲢��ʼ��
    sdcard_ptr = (SDCARD_STRUCT_PTR)malloc(sizeof(SDCARD_STRUCT));
    sdcard_ptr->CARD = ESDHC_CARD_NONE;
    sdcard_ptr->TIMEOUT = 0;
    sdcard_ptr->NUM_BLOCKS = 0;
    sdcard_ptr->ADDRESS = 0;
    sdcard_ptr->HC = FALSE;
    sdcard_ptr->VERSION2 = FALSE;
    sdcard_ptr->STATUS = SDHCSTA_OK;
     
    while(time_out < 1000)
    {
      
        //��ʼ��SDHCģ�鲢��⿨
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_INIT, NULL)))
        {
            continue;
        }
        
        //SDHC���
        param = 0;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_GET_CARD, &param)))
        {
            continue;
        }
        if ((ESDHC_CARD_SD == param) || (ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDCOMBO == param) || (ESDHC_CARD_SDHCCOMBO == param))
        {
            if ((ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDHCCOMBO == param))
            {
                sdcard_ptr->HC = TRUE;
                break;
            }
            else if(ESDHC_CARD_SD == param)
            {
                break;
            }
        }
        else
        {
            continue;
        }
        time_out++;
    }
    
    if(time_out >= 1000)
      return SDHCRES_NOTRDY;
    
    //��ʶ��
    command.COMMAND = ESDHC_CMD2;
    command.ARGUMENT = 0;
    command.BLOCKS = 0;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return result;
    }
    
    //��ȡ����ַ
    command.COMMAND = ESDHC_CMD3;
    command.ARGUMENT = 0;
    command.BLOCKS = 0;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return result;
    }
    sdcard_ptr->ADDRESS = command.RESPONSE[0] & 0xFFFF0000;
    
    //��ȡ������
    command.COMMAND = ESDHC_CMD9;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.BLOCKS = 0;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return result;
    }
    if (0 == (command.RESPONSE[3] & 0x00C00000))
    {
        read_bl_len = (command.RESPONSE[2] >> 8) & 0x0F;
        c_size = command.RESPONSE[2] & 0x03;
        c_size = (c_size << 10) | (command.RESPONSE[1] >> 22);
        c_size_mult = (command.RESPONSE[1] >> 7) & 0x07;
        sdcard_ptr->NUM_BLOCKS = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
    }
    else
    {
        sdcard_ptr->VERSION2 = TRUE;
        c_size = (command.RESPONSE[1] >> 8) & 0x003FFFFF;
        sdcard_ptr->NUM_BLOCKS = (c_size + 1) << 10;
    }
    
    //������������������Ϊ40MHz
    //param = 40000000;    
    param = 25000000;      
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SET_BAUDRATE, &param)))
    {
        return result;
    }
    
    //ѡ��
    command.COMMAND = ESDHC_CMD7;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.BLOCKS = 0;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return result;
    }
    
    //���ÿ��СΪ512�ֽ�
    command.COMMAND = ESDHC_CMD16;
    command.ARGUMENT = IO_SDCARD_BLOCK_SIZE;
    command.BLOCKS = 0;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return result;
    }
    
    if (ESDHC_BUS_WIDTH_4BIT == ESDHC_BUS_WIDTH_4BIT)
    {
        //����Ӧ������
        command.COMMAND = ESDHC_CMD55;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.BLOCKS = 0;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
        {
            return result;
        }
        
        //�������߿��Ϊ4bit
        command.COMMAND = ESDHC_ACMD6;
        command.ARGUMENT = 2;
        command.BLOCKS = 0;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
        {
            return result;
        }
        
        param = ESDHC_BUS_WIDTH_4BIT;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SET_BUS_WIDTH, &param)))
        {
            return result;
        }
    }
    
    return SDHCRES_OK;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ָ������ָ�����ȵĿ�
//  @param      buff            �洢�������ݵĵ�ַָ��
//  @param      sector          ��ʼ��������
//  @param      count           ��������������������
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       
//-------------------------------------------------------------------------------------------------------------------
SDHCRES sdhc_readblocks(uint8 *buff, uint32 sector, uint32 count)
{
    ESDHC_COMMAND_STRUCT command;
    int cnt;
    int32 result;
    
    //SD�����ݵ�ַ����
    if (! sdcard_ptr->HC)
    {
        sector <<= IO_SDCARD_BLOCK_SIZE_POWER;
    }
    
    //���ö�������
    if (count > 1)
    {
        command.COMMAND = ESDHC_CMD18;
    }   
    else
    {
        command.COMMAND = ESDHC_CMD17;
    }
    
    command.ARGUMENT = sector;
    command.BLOCKS = count;
    command.BLOCKSIZE = IO_SDCARD_BLOCK_SIZE;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return (SDHCRES)result;
    }
    
    //��ȡ����
    for (cnt = 0; cnt < count; cnt++)
    {
        if (IO_SDCARD_BLOCK_SIZE != (result=sdhc_read(buff,IO_SDCARD_BLOCK_SIZE)))
        {
            return (SDHCRES)result;
        }
        buff += IO_SDCARD_BLOCK_SIZE;
    }
    
    //�ȴ��������
    if (SDHCRES_OK !=(result=sdhc_ioc (IO_IOCTL_FLUSH_OUTPUT, NULL)))
    {
        return (SDHCRES)result;
    }
    
    return (SDHCRES)result;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ָ������д��ָ�����ȿ�������
//  @param      buff            �洢�������ݵĵ�ַָ��
//  @param      sector          ��ʼ��������
//  @param      count           д�����������������
//  @return     SDHCRES--���̹��ܷ���ֵ
//  @since      v1.0
//  @note       
//-------------------------------------------------------------------------------------------------------------------
SDHCRES sdhc_writeblocks(uint8* buff, uint32 sector, uint32 count)
{
    ESDHC_COMMAND_STRUCT command;
    uint8               tmp[4];
    int32             cnt;
      int32 result;
    
 
    //SD�����ݵ�ַ����
    if (! sdcard_ptr->HC)
    {
        sector <<= IO_SDCARD_BLOCK_SIZE_POWER;
    }

    //����д������
    if (count > 1)
    {
        command.COMMAND = ESDHC_CMD25;
    }
    else
    {
        command.COMMAND = ESDHC_CMD24;
    }

    command.ARGUMENT = sector;
    command.BLOCKS = count;
    command.BLOCKSIZE = IO_SDCARD_BLOCK_SIZE;
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
    {
        return (SDHCRES)result;
    }
    
    //д����
    for (cnt = 0; cnt < count; cnt++)
    {
        if (IO_SDCARD_BLOCK_SIZE != (result=sdhc_write (buff, IO_SDCARD_BLOCK_SIZE)))
        {
            return (SDHCRES)result;
        }
        buff += IO_SDCARD_BLOCK_SIZE;
    }

    //�ȴ��������
    if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_FLUSH_OUTPUT, NULL)))
    {
        return (SDHCRES)result;
    }

    //�ȴ���׼����/����״̬
    do
    {
        command.COMMAND = ESDHC_CMD13;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.BLOCKS = 0;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
        {
            return (SDHCRES)result;
        }

        //��״̬������
        if (command.RESPONSE[0] & 0xFFD98008)
        {
            count = 0; /* necessary to get real number of written blocks */
            break;
        }

    } while (0x000000900 != (command.RESPONSE[0] & 0x00001F00));

    if (cnt != count)
    {
        //����Ӧ������
        command.COMMAND = ESDHC_CMD55;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.BLOCKS = 0;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
        {
            return (SDHCRES)result;
        }
                
        //ʹ��ACMD22������д��Ŀ�����
        command.COMMAND = ESDHC_ACMD22;
        command.ARGUMENT = 0;
        command.BLOCKS = 1;
        command.BLOCKSIZE = 4;
        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_ESDHC_SEND_COMMAND, &command)))
        {
            return (SDHCRES)result;
        }
        
        if (4 != (result=sdhc_read (tmp, 4)))
        {
            return (SDHCRES)result;
            
        }

        if (SDHCRES_OK != (result=sdhc_ioc (IO_IOCTL_FLUSH_OUTPUT, NULL)))
        {
            return (SDHCRES)result;
        }

        count = (tmp[0] << 24) | (tmp[1] << 16) | (tmp[2] << 8) | tmp[3];
        if ((cnt < 0) || (cnt > count))
            return SDHCRES_ERROR;
    }
    
    return SDHCRES_OK;
}
