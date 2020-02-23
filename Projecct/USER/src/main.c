#include "headfile.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ģ���ʼ������
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��ʼ������ģ��
//-------------------------------------------------------------------------------------------------------------------
void HardWare_Init(void)
{
	DisableInterrupts;                  //�ر������жϽ��г�ʼ��
	get_clk();                          //�ϵ���������һ�������������ȡ����Ƶ����Ϣ�����ں������ģ��Ĳ�������
	
	Steer_Init();                       //�����ʼ�� FTM3
	Motor_Init();						//�����ʼ�� FTM0 1000
	Switch_Init();                      //���뿪�س�ʼ��
	Speed_Level();
	Buzzer_Init();                      //��������ʼ��
	led_init(LED_MAX);                  //LED��ʼ��
	Button_Init();					//������ʼ��
	//Beep_ON;
	ADC_init();							//��г�ʼ��
	FLASH_Init();						//��ʼ��flashģ��.
	AD_Normalization();					//��һ����ʼ��//��ȡ���ֵ��Сֵ
	
	camera_init();                      //����ͷ��ʼ��
	
	lcd_init();  						//lcd��ʼ��
	//Input_Information();				//���� ��������
	//dsp_single_colour(WHITE);
	
	NRF_Dev_Init();						//NRF��ʼ�� ����ģ��SPI��ʼ��
	//��������ʼ��
	ftm_quad_init(ftm2);  //FTM2  PTA10 �� PTA11 �����������ʼ��
	ftm_quad_init(ftm1);
	ftm_quad_clean(ftm2);//�����Ĵ�������
	ftm_quad_clean(ftm1);//�����Ĵ�������
	
	//���ڳ�ʼ��
	//uart_init (uart4, 115200); 		//��ʼ�����������ͨ��
	uart_init (uart0, 115200);          //��ʼ��swm1000
	uart_rx_irq_en(uart0);
	
	if(Switch_3==0)
		Mode=1;
	else
	{
		Mode=0;
		Meeting_My.Flag=0;
		Meeting_My.State=2;
	}
	
	if(Switch_9==0)
	{
		Roundabout.Mode=1;
	}
	else
	{
		Roundabout.Mode=0;
	}
	
	if(Switch_10==0)
	{
		Rampway.Start=1;
	}
	else
	{
		Rampway.Start=0;
	}
	
	//�����ж�������
#if chronography 	//��ʱ
	
	pit_init_ms(pit0, 8);                   //��ʼ��PIT0����ʱʱ��Ϊ�� 8ms
	pit_init_ms(pit1, 1);					  //��ʼ��PIT1	���ڼ�ʱ��1ms
	NVIC_SetPriorityGrouping(5);            //�������ȼ�����,4bit ��ռ���ȼ�,û��������
	NVIC_SetPriority(PORTB_IRQn , 1);       //�������ȼ�  ͼ���ж�  PORTA
	NVIC_SetPriority(DMA0_IRQn  , 2);       //�������ȼ�  ͼ��  DMA0
	NVIC_SetPriority(PIT1_IRQn  , 0);       //�������ȼ�  ������   PIT0 
	NVIC_SetPriority(PIT0_IRQn  , 3);       //�������ȼ�  ������   PIT0 
	NVIC_SetPriority(UART0_RX_TX_IRQn,4); //�������ȼ�  ��0
	enable_irq(PIT1_IRQn);				  //ʹ��PIT1�ж�
	enable_irq(PIT0_IRQn);	              //ʹ��PIT0�ж�
	uart_rx_irq_en(uart0);
	
#else
	pit_init_ms(pit0, 8);                   //��ʼ��PIT0����ʱʱ��Ϊ�� 8ms
	NVIC_SetPriorityGrouping(4);            //�������ȼ�����,4bit ��ռ���ȼ�,û��������
	NVIC_SetPriority(PORTB_IRQn , 2);       //�������ȼ�  ͼ���ж�  PORTA
	NVIC_SetPriority(DMA0_IRQn  , 1);       //�������ȼ�  ͼ��  DMA0
	NVIC_SetPriority(PIT0_IRQn  , 0);       //�������ȼ�  ������   PIT0 
	NVIC_SetPriority(UART0_RX_TX_IRQn,3); //�������ȼ�  ��0
	enable_irq(PIT0_IRQn);	              //ʹ��PIT0�ж�
	uart_rx_irq_en(uart0);
	
#endif
	EnableInterrupts;                   	  //�������ж�	
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      running  �����������г���
//  @return     int   
//  @since      v1.0
//  Sample usage:               xxyxxx
//-------------------------------------------------------------------------------------------------------------------

void Running(void)
{

	
	Camera_Get_image();
	if(Mode)
		Read_ADC();

	if(Meeting_My.Flag||Meeting_My.State<2)
	{
		if(!ZebraCrossing.Second_Flag)
		Search_CenterBlackline_Meeting();	//�ᳵ��ֻѰ�ҵ��ߣ���ߣ�
		
		if(ZebraCrossing.Start_Flag)	
		{  //ֻ���ڻᳵ��0 1 5 6��Ѱ�Ұ����� 
			Judge_ZebraCrossing(&ZebraCrossing);
			if(ZebraCrossing.Flag&&Meeting_My.Track_Mode==0)//Ѱ�Ұ����ߺ�ı߽�
				ModifyEdge_ZebraCrossing(&ZebraCrossing);
		}
	}
	else
	{
		Search_CenterBlackline();     		 	//Ѱ�ұ���
		
		
		if(!Roundabout.Flag)					//Ѱ�ҹյ�
		{
			FindInflectionPointL(&InflectionPointL ,LeftStart,DropRow+2);   
			FindInflectionPointR(&InflectionPointR ,RightStart,DropRow+2);
		}
		
		
		//�е�
		if(Meeting_Area.Start_Flag&&Roundabout.State<2&&!Rampway.Flag&&Crossroad.State<2&&Mode)//�жϻᳵ��
			Judge_Meetingarea(&Meeting_Area); 
		
		if(Crossroad.State<2&&Mode)
			Judge_Roundabout(&Roundabout);       //�жϻ���

		if(!Roundabout.Flag&&!Rampway.Flag&&Crossroad.Start_Flag)
			Judge_Crossroad(&Crossroad);

		if(Roundabout.State<1)
			Judge_Rampway(&Rampway);
		//����
		
		
		if(Roundabout.Flag)
			ModifyEdge_Roundabout(&Roundabout);  //��������
		
		if(Crossroad.Flag&&Roundabout.Flag==0)
			ModifyEdge_Crossroad(&Crossroad);
		
	}

	Calculation_Middleline();			 //���¼�������
	Get_EndLine(MidPri);                 	 //Ѱ�ҽ�ֹ��
	Steer_Ctrl();                        //�������
	if(Rampway.Flag||(Roundabout.Size==0&&Roundabout.State==1))
		Beep_ON;
	else
		Beep_OFF;

	switch(Level)
	{
	case Fast_Level:
		{	
			Fast_Set();
			break;
		}
	case Nor_Level:
		{
			Speed_Set();
			break;
			
		}
	case FastFast_Level:
		{
			FastFast_Set();
			break;
		}
	case Low_Level:
		{
			Low_Set();
		//	Beep_ON;
			break;
		}
		
	}

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      main����
//  @return     int   
//  @since      v1.0
//  Sample usage:               xxyxxx
//------------------------------------------------------------------------------------------------------------------
int main(void)
{
	HardWare_Init();   //��ʼ������
	//ѡ��ģʽ
	if(Switch_4)
	{
		//Mode=1;
		Meeting_My.Flag=0;
		Meeting_My.State=2;
		Meeting_Other.Flag=0;
		Meeting_Other.State=2;
		//Meeting_Other.Oder=1;
	}
	if(Switch_5)
	{
		Meeting_My.Flag=0;
		Meeting_My.State=4;
		Meeting_Other.Flag=0;
		Meeting_Other.State=4;
	}
	while(1)
	{
/********************************************/
		//��ʱ��
#if chronography
		static int a=0;
		a=!a;
		gpio_init(E24, GPO,a);
		
#endif
/********************************************/
		//get_config();
		if(Mode)//3ms
		{	//�������� ���
			Communication_Accept(&Meeting_My,&Meeting_Other);
			Read_Distance();
			//�ᳵ���ж�
			Judge_Meeting(&Meeting_My,&Meeting_Other,&Meeting_Area);
		}
		
		Running();//С���ܳ��� 10ms
		//Beep_ON;
		if(Meeting_My.Send_Flag&&Mode)	
		{	//���ͳ���
			Communication_Send(&Meeting_My,&Meeting_Other);
		}
		
/********************************************/		
		if(Switch_1)
		{
			Display();
		}
		//Beep_OFF;
		
	}
	

	
	
}