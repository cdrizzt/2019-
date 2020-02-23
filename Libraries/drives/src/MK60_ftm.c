
 #include "MK60_ftm.h"

void FTM_port_mux(FTMn ftmn, FTM_CHn ch)
{

    switch(ftmn)
    {
    case ftm0:
        SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;       //ʹ��FTM0ʱ��
        switch(ch)
        {
        case ftm_ch0:
            if(FTM0_CH0_PIN == C1)
            {
                port_init(FTM0_CH0_PIN, ALT4);
            }
            else if(FTM0_CH0_PIN == A3)
            {
                port_init(FTM0_CH0_PIN, ALT3);
            }
            break;

        case ftm_ch1:
            if(FTM0_CH1_PIN == C2)
            {
                port_init(FTM0_CH1_PIN, ALT4);
            }
            else if(FTM0_CH1_PIN == A4)
            {
                port_init(FTM0_CH1_PIN, ALT3);
            }
            break;

        case ftm_ch2:
            if(FTM0_CH2_PIN == C3)
            {
                port_init(FTM0_CH2_PIN, ALT4);
            }
            else if(FTM0_CH2_PIN == A5)
            {
                port_init(FTM0_CH2_PIN, ALT3);
            }
            break;

        case ftm_ch3:
            if(FTM0_CH3_PIN == C4)
            {
                port_init(FTM0_CH3_PIN, ALT4);
            }
            else if(FTM0_CH3_PIN == A6)
            {
                port_init(FTM0_CH3_PIN, ALT3);
            }
            break;

        case ftm_ch4:
            if(FTM0_CH4_PIN == D4)
            {
                port_init(FTM0_CH4_PIN, ALT4);
            }
            else if(FTM0_CH4_PIN == A7)
            {
                port_init(FTM0_CH4_PIN, ALT3);
            }
            break;

        case ftm_ch5:
            if(FTM0_CH5_PIN == D5)
            {
                port_init(FTM0_CH5_PIN, ALT4);
            }
            else if(FTM0_CH5_PIN == A0)
            {
                port_init(FTM0_CH5_PIN, ALT3);
            }
            break;

        case ftm_ch6:
            if(FTM0_CH6_PIN == D6)
            {
                port_init(FTM0_CH6_PIN, ALT4);
            }
            else if(FTM0_CH6_PIN == A1)
            {
                port_init(FTM0_CH6_PIN, ALT3);
            }
            break;

        case ftm_ch7:
            if(FTM0_CH7_PIN == D7)
            {
                port_init(FTM0_CH7_PIN, ALT4);
            }
            else if(FTM0_CH7_PIN == A2)
            {
                port_init(FTM0_CH7_PIN, ALT3);
            }
            break;
        default:
            return;
        }
        break;

    case ftm1:
        SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;       //ʹ��FTM1ʱ��
        switch(ch)
        {
        case ftm_ch0:
            if((FTM1_CH0_PIN == A8) || (FTM1_CH0_PIN == A12) || (FTM1_CH0_PIN == B0) )
            {
                port_init(FTM1_CH0_PIN, ALT3);
            }
            break;


        case ftm_ch1:
            if((FTM1_CH1_PIN == A9) || (FTM1_CH1_PIN == A13) || (FTM1_CH1_PIN == B1) )
            {
                port_init(FTM1_CH1_PIN, ALT3);
            }
            break;

        default:
            return;
        }
        break;

    case ftm2:
        SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case ftm_ch0:
            if((FTM2_CH0_PIN == A10) || (FTM2_CH0_PIN == B18) )
            {
                port_init(FTM2_CH0_PIN, ALT3);
            }
            else
            break;

        case ftm_ch1:
            if((FTM2_CH1_PIN == A11) || (FTM2_CH1_PIN == B19))
            {
                port_init(FTM2_CH1_PIN, ALT3);
            }
            break;

        default:
            return;
        }
        break;
        
    case ftm3:
        SIM->SCGC3 |= SIM_SCGC3_FTM3_MASK;                           //ʹ��FTM2ʱ��
        switch(ch)
        {
        case ftm_ch0:
            if(FTM3_CH0_PIN == E5)
            {
                port_init(FTM3_CH0_PIN, ALT6);
            }
            else if(FTM3_CH0_PIN == D0)
            {
                port_init(FTM3_CH0_PIN, ALT4);
            }
            break;

        case ftm_ch1:
            if(FTM3_CH1_PIN == E6)
            {
                port_init(FTM3_CH1_PIN, ALT6);
            }
            else if(FTM3_CH1_PIN == D1)
            {
                port_init(FTM3_CH1_PIN, ALT4);
            }
            break;

        case ftm_ch2:
            if(FTM3_CH2_PIN == E7)
            {
                port_init(FTM3_CH2_PIN, ALT6);
            }
            else if(FTM3_CH2_PIN == D2)
            {
                port_init(FTM3_CH2_PIN, ALT4);
            }
            break;

        case ftm_ch3:
            if(FTM3_CH3_PIN == E8)
            {
                port_init(FTM3_CH3_PIN, ALT6);
            }
            else if(FTM3_CH3_PIN == D3)
            {
                port_init(FTM3_CH3_PIN, ALT4);
            }
            break;

        case ftm_ch4:
            if(FTM3_CH4_PIN == E9)
            {
                port_init(FTM3_CH4_PIN, ALT6);
            }
            else if(FTM3_CH4_PIN == C8)
            {
                port_init(FTM3_CH4_PIN, ALT3);
            }
            break;

        case ftm_ch5:
            if(FTM3_CH5_PIN == E10)
            {
                port_init(FTM3_CH5_PIN, ALT6);
            }
            else if(FTM3_CH5_PIN == C9)
            {
                port_init(FTM3_CH5_PIN, ALT3);
            }
            break;

        case ftm_ch6:
            if(FTM3_CH6_PIN == E11)
            {
                port_init(FTM3_CH6_PIN, ALT6);
            }
            else if(FTM3_CH6_PIN == C10)
            {
                port_init(FTM3_CH6_PIN, ALT3);
            }
            break;

        case ftm_ch7:
            if(FTM3_CH7_PIN == E12)
            {
                port_init(FTM3_CH7_PIN, ALT6);
            }
            else if(FTM3_CH7_PIN == C11)
            {
                port_init(FTM3_CH7_PIN, ALT3);
            }
            break;
        default:
            return;
        }
        break;
    }

}


static FTM_Type * const FTMX[] = FTM_BASE_PTRS;
uint16 period[3] = {0,0,0};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      FTM��ʼ��
//  @param      ftmn            ѡ��ģ��
//  @param      ch              ѡ��ͨ��
//  @param      freq            ��Ҫ���õ�Ƶ�ʣ���λHZ��
//  @param      duty            ��Ҫ���õ�ռ�ձ�
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_pwm_init(ftm0,ftm_ch0,50,500);		//ftm0ģ�飬0ͨ����50HZ���ٷ�֮50��ռ�ձ�
//-------------------------------------------------------------------------------------------------------------------
void ftm_pwm_init(FTMn ftmn, FTM_CHn ch, uint32 freq, uint32 duty)
{
    uint32 clk_hz ;
    uint16 mod;
    uint8  ps;
    uint16 cv;
    FTM_port_mux(ftmn,ch);
    
    // �� CPWMS = 0 �������߲�׽����Ϊ��
    clk_hz = (bus_clk_mhz * 1000 * 1000) ;   // busƵ��
    
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // ��ʱ�� mod ����һ��
    ps = 0;
    while((mod >> ps) >= 1)                 // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
        ps++;
    if(ps>0x07) return ;                    //�������÷�Χ��ֱ�Ӳ�����������������ԭ��һ��������PWMƵ��̫�ͣ���������Ƶ��̫�ߵ���
    
    mod = (clk_hz >> ps) / freq;            // �� MOD ��ֵ
    period[ftmn] = mod;
    switch(ftmn)                            // ��ֵ CNTIN ��Ϊ0 �������ȣ�CnV - CNTIN ���� CnV ���� �������ˡ�
    {
        // EPWM������ �� MOD - CNTIN + 0x0001 == MOD - 0 + 1
        // �� CnV = (MOD - 0 + 1) * ռ�ձ� = (MOD - 0 + 1) * duty/ FTM_PRECISON
    case ftm0:
        cv = (duty * (mod - 0 + 1)) / FTM0_PRECISON;
        break;

    case ftm1:
        cv = (duty * (mod - 0 + 1)) / FTM1_PRECISON;
        break;

    case ftm2:
        cv = (duty * (mod - 0 + 1)) / FTM2_PRECISON;
        break;
        
    case ftm3:
        cv = (duty * (mod - 0 + 1)) / FTM3_PRECISON;
        break;
    default:
        break;
    }
    
    FTMX[ftmn]->MOD = mod;                  //����PWM����
    
    FTMX[ftmn]->CONTROLS[ch].CnSC &= ~FTM_CnSC_ELSA_MASK;
    FTMX[ftmn]->CONTROLS[ch].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    
    // set FTM clock to system clock 
    FTMX[ftmn]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                       | FTM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                       | FTM_SC_CLKS(1)            //ʱ��ѡ�� 0��ûѡ��ʱ�ӣ����ã� 1��bus ʱ�ӣ� 2��MCGFFCLK�� 3��EXTCLK�� ��SIM_SOPT4 ѡ������ܽ� FTM_CLKINx��
                       //| FTM_SC_TOIE_MASK        //����ж�ʹ�ܣ�ע���˱�ʾ ��ֹ����жϣ�
                     );

    
    FTMX[ftmn]->CNTIN = 0;                  //���ü�������ֵ��һ��û������;������
    FTMX[ftmn]->CONTROLS[ch].CnV = cv;      //����ռ�ձ�
    FTMX[ftmn]->CNT = 0;                    //��������д����ֵ�������CNTIN��ֵ

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ռ�ձ�����
//  @param      ftmn            ѡ��ģ��
//  @param      ch              ѡ��ͨ��
//  @param      duty            ��Ҫ���õ�ռ�ձ�
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_pwm_duty(ftm0,ftm_ch0,500);		//ftm0ģ�飬0ͨ�����ٷ�֮50��ռ�ձ�
//-------------------------------------------------------------------------------------------------------------------
void ftm_pwm_duty(FTMn ftmn, FTM_CHn ch, uint32 duty)
{
    uint32 cv;
    //ռ�ձ� = (CnV-CNTIN)/(MOD-CNTIN+1)
    switch(ftmn)
    {
    case ftm0:
        cv = (duty * (period[ftmn] - 0 + 1)) / FTM0_PRECISON;
        break;

    case ftm1:
        cv = (duty * (period[ftmn] - 0 + 1)) / FTM1_PRECISON;
        break;

    case ftm2:
        cv = (duty * (period[ftmn] - 0 + 1)) / FTM2_PRECISON;
        break;

    case ftm3:
        cv = (duty * (period[ftmn] - 0 + 1)) / FTM3_PRECISON;
        break;
    default:
        break;
    }
    
    FTMX[ftmn]->CONTROLS[ch].CnV = cv;      //����ռ�ձ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ƶ������
//  @param      ftmn            ѡ��ģ��
//  @param      freq            ��Ҫ���õ�Ƶ�ʣ���λHZ��
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_pwm_freq(ftm0,50);		//ftm0ģ�飬50HZ
//-------------------------------------------------------------------------------------------------------------------
void ftm_pwm_freq(FTMn ftmn, uint32 freq)             //����FTM��Ƶ��
{
    uint32 clk_hz;        //busƵ��/2
    uint32 mod;
    uint8 ps;

    // �� CPWMS = 0 �������߲�׽����Ϊ��
    clk_hz = (bus_clk_mhz * 1000 * 1000) ;   // busƵ��
    
    //clk_hz/(ps*mod) = freq        =>>     clk_hz/freq = (ps*mod)      =>>
    //clk_hz/freq < ((1<<n)*65536)  =>>    (clk_hz/freq)/65536 < (1>>n) =>> ((clk_hz/freq)/65536)>>n < 1
    mod = (clk_hz >> 16 ) / freq ;          // ��ʱ�� mod ����һ��
    ps = 0;
    while((mod >> ps) >= 1)                 // �� (mod >> ps) < 1 ���˳� while ѭ�� ������ PS ����Сֵ
        ps++;
    if(ps>0x07) return ;                    //�������÷�Χ��ֱ�Ӳ�����������������ԭ��һ��������PWMƵ��̫�ͣ���������Ƶ��̫�ߵ���
    
    mod = (clk_hz >> ps) / freq;            // �� MOD ��ֵ
    period[ftmn] = mod;
    
    
    FTMX[ftmn]->MOD = mod;                  //����PWM����
    
    // set FTM clock to system clock 
    FTMX[ftmn]->SC = ( 0
                       //| FTM_SC_CPWMS_MASK       //0�������ؼ���ģʽ ��1�� �����ؼ���ģʽѡ�� ��ע���˱�ʾ 0��
                       | FTM_SC_PS(ps)             //��Ƶ���ӣ���Ƶϵ�� = 2^PS
                       | FTM_SC_CLKS(1)            //ʱ��ѡ�� 0��ûѡ��ʱ�ӣ����ã� 1��bus ʱ�ӣ� 2��MCGFFCLK�� 3��EXTCLK�� ��SIM_SOPT4 ѡ������ܽ� FTM_CLKINx��
                       //| FTM_SC_TOIE_MASK        //����ж�ʹ�ܣ�ע���˱�ʾ ��ֹ����жϣ�
                     );

    
    FTMX[ftmn]->CNTIN = 0;                  //���ü�������ֵ��һ��û������;������
    FTMX[ftmn]->CNT = 0;                    //��������д����ֵ�������CNTIN��ֵ
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���벶׽ģʽ
//  @param      ftmn            ѡ��ģ��
//  @param      ch              ѡ��ͨ��
//  @param      cfg             ���ô�����ʽ FTM_Rising FTM_Falling FTM_Rising_or_Falling
//  @param      freq_max        ��������Ƶ�ʣ���λHZ��
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_input_init(ftm0,ftm_ch0,FTM_Rising,5000);		//ftm0ģ�飬0ͨ���������ش��������5KHZ
//-------------------------------------------------------------------------------------------------------------------
void ftm_input_init(FTMn ftmn, FTM_CHn ch, FTM_Input_cfg cfg,uint32 freq_max)
{
    uint8 ps;
    uint32 temp;
    FTM_port_mux(ftmn,ch);

    //------------------------ ����Ϊ���벶׽���� ------------------------
    //���벶׽ģʽ�£�DECAPEN = 0 �� DECAPEN = 0 ��CPWMS = 0�� MSnB:MSnA = 0

    // ELSnB:ELSnA         1          10          11
    // ����             ������      �½���      ������
    FTMX[ftmn]->CONTROLS[ch].CnSC &=  ~(FTM_CnSC_ELSA_MASK | FTM_CnSC_ELSB_MASK | FTM_CnSC_MSA_MASK | FTM_CnSC_MSB_MASK);
    FTMX[ftmn]->CONTROLS[ch].CnSC |=  cfg << FTM_CnSC_ELSA_SHIFT;
    //ʹ��ͨ�����벶׽�ж�
    FTMX[ftmn]->CONTROLS[ch].CnSC |= FTM_CnSC_CHIE_MASK;
    
    ps = 0;
    temp = (bus_clk_mhz * 1000 * 1000) >>16;
    while((temp>>ps) >= freq_max)   ps++;
    if(ps>7)    ps = 7;
    // set FTM clock to system clock 
    FTMX[ftmn]->SC = ( 0 | FTM_SC_PS(ps) | FTM_SC_CLKS(1) );
    
    FTMX[ftmn]->MODE |= FTM_MODE_WPDIS_MASK;        //��ֹд���� 
    FTMX[ftmn]->COMBINE = 0;
    FTMX[ftmn]->MODE &= ~FTM_MODE_FTMEN_MASK;       //ʹ��FTM
    FTMX[ftmn]->CNTIN = 0;

    //�������ȼ��������Ҫ�������ȼ�����ע��
    //set_irq_priority((IRQn_Type)(FTM0_IRQn + ftmn),5);
    //ʹ��FTM�ж�
    enable_irq((IRQn_Type)(FTM0_IRQn + (IRQn_Type)ftmn));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ����ͨ����ֵ
//  @param      ftmn            ѡ��ģ��
//  @param      ch              ѡ��ͨ��
//  @return     void
//  @since      v1.0
//  Sample usage:               uint16 dat = ftm_input_get(ftm0,ftm_ch0);	
//-------------------------------------------------------------------------------------------------------------------
uint16 ftm_input_get (FTMn ftmn, FTM_CHn ch)
{
    return FTMX[ftmn]->CONTROLS[ch].CnV;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���CNTͨ����ֵ
//  @param      ftmn            ѡ��ģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_input_clean(ftm0);	
//-------------------------------------------------------------------------------------------------------------------
void ftm_input_clean (FTMn ftmn)
{
    FTMX[ftmn]->CNT = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���������ʼ��
//  @param      ftmn            ѡ��ģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_quad_init(ftm1);	
//-------------------------------------------------------------------------------------------------------------------
void ftm_quad_init(FTMn ftmn)
{
    switch(ftmn)
    {

    case ftm1:
        SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;                          //ʹ��FTM1ʱ��
        if((FTM1_QDPHA_PIN == A8) || (FTM1_QDPHA_PIN == B0) )                 //�ܽŸ���
        {
            port_init(FTM1_QDPHA_PIN, ALT6);
        }
        else if(FTM1_QDPHA_PIN == A12)
        {
            port_init(FTM1_QDPHA_PIN, ALT7);
        }

        if((FTM1_QDPHB_PIN == A9) ||(FTM1_QDPHB_PIN == B1))
        {
            port_init(FTM1_QDPHB_PIN, ALT6);
        }
        else if(FTM1_QDPHB_PIN == A13)
        {
            port_init(FTM1_QDPHB_PIN, ALT7);
        }
        break;

    case ftm2:
        SIM->SCGC3 |= SIM_SCGC3_FTM2_MASK;                           //ʹ��FTM2ʱ��
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;                           //ʹ��FTM2ʱ��
        if((FTM2_QDPHA_PIN == A10) || (FTM2_QDPHA_PIN == B18))                  //�ܽŸ���
        {
            port_init(FTM2_QDPHA_PIN, ALT6);
        }
        if((FTM2_QDPHB_PIN == A11) || (FTM2_QDPHB_PIN == B19) )                 //�ܽŸ���
        {
            port_init(FTM2_QDPHB_PIN, ALT6);
        }
        break;
        
    default:
        break;
    }

    FTMX[ftmn]->MODE |= FTM_MODE_WPDIS_MASK;        //��ֹд����
    FTMX[ftmn]->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;
    FTMX[ftmn]->CNTIN = 0;
    FTMX[ftmn]->MOD = FTM_MOD_MOD_MASK;
    FTMX[ftmn]->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    FTMX[ftmn]->MODE |= FTM_QDCTRL_QUADEN_MASK;
    FTMX[ftmn]->CNT = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����������ݶ�ȡ
//  @param      ftmn            ѡ��ģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               int16 dat = ftm_quad_get(ftm1);	
//-------------------------------------------------------------------------------------------------------------------
int16 ftm_quad_get(FTMn ftmn)
{
    return FTMX[ftmn]->CNT;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������
//  @param      ftmn            ѡ��ģ��
//  @return     void
//  @since      v1.0
//  Sample usage:               ftm_quad_clean(ftm1);	
//-------------------------------------------------------------------------------------------------------------------
void ftm_quad_clean(FTMn ftmn)
{
    FTMX[ftmn]->CNT = 0;
}


