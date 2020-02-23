#include "Motor.h"

int16  Pulse_FTM1,Pulse_FTM2;//��������Ĵ�����ֵ
int16  Pulse_Last_FTM1,Pulse_Last_FTM2;//��������Ĵ�����ֵ
int32  Get_Pules_R,Get_Pules_L; 
brake Brake;
int32 LSpeedSet=0;//����Ԥ��ֵ
int32 RSpeedSet=0;//����Ԥ��ֵ
int32 SpeedSet=0;
int32 SpeedSet_High=0;
float SpeedError_L=0;
float SpeedError_R=0;
float SpeedLastError_L=0;
float SpeedLastError_R=0;
float SpeedPerError_L=0;
float SpeedPerError_R=0;
uint8  Reduce_ADD=0;
uint8  RBangBang_Flag = 0;   
uint8  LBangBang_Flag = 0;   
int ErrorM=0;
int SpeedDropRow=0;
int LastSpeedDropRow;
uint8  Meeting_Curve_First=60;
uint8  Meeting_Curve_Second=60;
uint8  Meeting_Curve_End=60 	;
float  SpeedP;//50.0;40
float  SpeedI;//16.0;50,0.0006
float  SpeedD;//1.3,10.0

int Stright_Flag=0;
int LPwmAdd=0;
int RPwmAdd=0;
int MotorPwmRight=0;
int MotorPwmLeft=0;

int MotorPwmRight_Con=0;
int MotorPwmLeft_Con=0;
int16	AA;
int32	Angle=0;
float	Temp_Orr; 

//------------------------------------------------------------------------------------------------------------------
//  @brief      �����ʼ��
//  @param      NULL
//  @return     void					
//  @since      v1.0    //A10 A11 A19
//  Sample usage:	0��ת	right FTM0-CH0 left FTM0_CH1
//-------------------------------------------------------------------------------------------------------------------
void Motor_Init()
{
	gpio_init(Right_DR,GPO,1);      //������Ƴ�ʼ�� 0��ת
	gpio_init(Left_DR,GPO,1);       //������Ƴ�ʼ�� 0��ת
	
	ftm_pwm_init(Motor_ftm,Right_CH,Motor_HZ,0);         //PWM����ʼ�� 15kHz
	ftm_pwm_init(Motor_ftm,Left_CH ,Motor_HZ,0);         //PWM����ʼ�� 15kHz       
	
	if(Rampway.Start)
	{
		Meeting_Curve_First=120;
		Meeting_Curve_Second=90;
		Meeting_Curve_End=120;
	}
	else
	{
		Meeting_Curve_First=80;
		Meeting_Curve_Second=80;
		Meeting_Curve_End=80;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ٶ�ƫ��
//  @param      NULL
//  @return     void					
//  @since      v1.0   
//  Sample usage: �����ٶ�ƫ��
//-------------------------------------------------------------------------------------------------------------------
void Calculate_Error(float LeftMotor,float RightMotor)//�ٶ�ƫ��
{ 
	SpeedPerError_L=SpeedLastError_L;
	SpeedLastError_L=SpeedError_L;
	SpeedError_L=LeftMotor-Get_Pules_L; 
	
	SpeedPerError_R=SpeedLastError_R;
	SpeedLastError_R=SpeedError_R ;
	SpeedError_R=RightMotor-Get_Pules_R;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM���
//  @param      NULL
//  @return     void					
//  @since      v1.0   
//  Sample usage: ���PWM���ҵ��0��ת 1��ת
//-------------------------------------------------------------------------------------------------------------------
void Motor_Pwm_Out(int speed_left, int speed_right)
{
	if(speed_left>=0)
	{
		//Beep_OFF;
		gpio_init(Left_DR,GPO,LeftForward);   
		ftm_pwm_duty(Motor_ftm,Left_CH,speed_left);//MotorPwmLeft
	}
	else
	{
		//Beep_ON;
		gpio_init(Left_DR,GPO,LeftBackward);   
		ftm_pwm_duty(Motor_ftm,Left_CH,-speed_left);//MotorPwmLeft
	}
	
	if(speed_right>=0)
	{
		gpio_init(Right_DR,GPO,RightForward); 
		ftm_pwm_duty(Motor_ftm,Right_CH,speed_right);//MotorPwmRight
	}
	else
	{
		gpio_init(Right_DR,GPO,RightBackward);    
		ftm_pwm_duty(Motor_ftm,Right_CH,-speed_right);//MotorPwmRight
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ɲ������
//  @param      NULL
//  @return     void					
//  @since      v1.0   
//  Sample usage: // �ٶ� ������Brake_BB(�����ٶȣ������ٶȣ����ȣ�&Brake)
//-------------------------------------------------------------------------------------------------------------------
int Brake_Flag=0;
void Brake_BB(int LSpeed,int RSpeed,float power,brake *prt)
{ 
//	static int K=0;
	if(Get_Pules_L<0)
		MotorPwmLeft=0;
	else
		MotorPwmLeft=(int)(-Get_Pules_L*power);
	if(Get_Pules_R<0)
		MotorPwmRight=0;
	else
		MotorPwmRight=(int)(-Get_Pules_R*power);
	
	if((Get_Pules_R<=RSpeed&&Get_Pules_L<=LSpeed))
	{
		Brake_Flag=1;	
		
	}
	if(Brake_Flag)
	{
		//	K++;
		//if(K>=10)
		{	
			//	K=0;
			Brake_Flag=0;
			prt->Flag=0;
			prt->State=0;
		}
	}
}

void Brake_L(int Speed,float power)
{ 
	if(SpeedError_L<-30)
		MotorPwmLeft=(int)(-Get_Pules_L*power);
	
}
void Brake_R(int Speed,float power)
{ 
	if(SpeedError_R<-30)
		MotorPwmRight=(int)(-Get_Pules_R*power);
	
}

void Brake_HD(int LSpeed,int RSpeed,float power,brake *prt)
{ 
//	static int K=0;
	if(SpeedError_L<-10)
		MotorPwmLeft=(int)(-Get_Pules_L*power);
	
	if(SpeedError_R<-10)
		MotorPwmRight=(int)(-Get_Pules_R*power);
	
	if((Get_Pules_R<=RSpeed&&Get_Pules_L<=LSpeed)||Roundabout.State==3||Roundabout.Flag==0)
	{
		Brake_Flag=1;	
		
	}
	if(Brake_Flag)
	{
		///	K++;
		//	if(K>=10)
		{	
			//	K=0;
			Brake_Flag=0;
			prt->Flag=0;
			prt->State=0;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ɲ����λѡ��
//  @param      NULL
//  @return     void					
//  @since      v1.0   
//  Sample usage: 1-5
//-------------------------------------------------------------------------------------------------------------------
uint8 TIME=0;
void Brake_Car(brake *prt)
{
	
		switch(Level)
	{
	case Fast_Level:
		{	
			if(prt->Flag)
	{
		if(prt->State==1)
		{
			Brake_HD (LSpeedSet,RSpeedSet,8,prt);
		}
		else if(prt->State==2)
		{
			Brake_BB(LSpeedSet,RSpeedSet,10,prt);
			
		}
		else if(prt->State==3)//�ᳵ������
		{
			Brake_BB(70,70,3,prt);
			if(Brake.Flag==0)
				Meeting_My.Brake=0;
		}
		else if(prt->State==4)//�ᳵ��ɲ��
		{
			if(Meeting_My.State==5)
			{
				if(Get_Pules_L<20)
					MotorPwmLeft=0;
				else
					MotorPwmLeft=-980;
				
				if(Get_Pules_R<20)
					MotorPwmRight=0;
				else
					MotorPwmRight=-980;
				if(Get_Pules_L<20&&Get_Pules_R<20)
				{
					TIME++;
				}
				if(TIME>4)
				{
					Brake.Flag=0;
					Meeting_My.Brake=0;
					TIME=0;
				}
			}
			else
			{
				Brake_BB(20,20,8,prt);
				if(Brake.Flag==0)
				{
					Meeting_My.Brake=0;
				}
			}
		}
		
		else if(prt->State==5)//�ᳵ��ͣ��
		{
			if(Get_Pules_L-LSpeedSet<10)
				MotorPwmLeft=0;
			else
				MotorPwmLeft=-980;
			
			if(Get_Pules_R-RSpeedSet<10)
				MotorPwmRight=0;
			else
				MotorPwmRight=-980;
			
			if(Meeting_My.Stop_Flag==0)
			{
				prt->Flag=0;
				prt->State=0;
			}
		}
		
	}
		else 
	{
		//��������ɲ��
		Brake_R(RSpeedSet,5);
		Brake_L(LSpeedSet,5);
	}
			break;
		}
	case Nor_Level:
		{
			if(prt->Flag)
	{
		if(prt->State==1)
		{
			Brake_HD (LSpeedSet,RSpeedSet,6,prt);
		}
		else if(prt->State==2)
		{
			Brake_BB(LSpeedSet,RSpeedSet,6.5,prt);
			
		}
		else if(prt->State==3)//�ᳵ������
		{
			Brake_BB(70,70,3,prt);
			if(Brake.Flag==0)
				Meeting_My.Brake=0;
		}
		else if(prt->State==4)//�ᳵ��ɲ��
		{
			if(Meeting_My.State==5)
			{
				if(Get_Pules_L<20)
					MotorPwmLeft=0;
				else
					MotorPwmLeft=-980;
				
				if(Get_Pules_R<20)
					MotorPwmRight=0;
				else
					MotorPwmRight=-980;
				if(Get_Pules_L<20&&Get_Pules_R<20)
				{
					TIME++;
				}
				if(TIME>4)
				{
					Brake.Flag=0;
					Meeting_My.Brake=0;
					TIME=0;
				}
			}
			else
			{
				Brake_BB(20,20,8,prt);
				if(Brake.Flag==0)
				{
					Meeting_My.Brake=0;
				}
			}
		}
		
		else if(prt->State==5)//�ᳵ��ͣ��
		{
			if(Get_Pules_L-LSpeedSet<10)
				MotorPwmLeft=0;
			else
				MotorPwmLeft=-980;
			
			if(Get_Pules_R-RSpeedSet<10)
				MotorPwmRight=0;
			else
				MotorPwmRight=-980;
			
			if(Meeting_My.Stop_Flag==0)
			{
				prt->Flag=0;
				prt->State=0;
			}
		}
		
	}
		else 
	{
		//��������ɲ��
		Brake_R(RSpeedSet,3);
		Brake_L(LSpeedSet,3);
	}
			break;
			
		}
	case FastFast_Level:
		{
			if(prt->Flag)
	{
		if(prt->State==1)
		{
			Brake_HD (LSpeedSet,RSpeedSet,6,prt);
		}
		else if(prt->State==2)
		{
			Brake_BB(LSpeedSet,RSpeedSet,6.5,prt);
			
		}
		else if(prt->State==3)//�ᳵ������
		{
			Brake_BB(70,70,3,prt);
			if(Brake.Flag==0)
				Meeting_My.Brake=0;
		}
		else if(prt->State==4)//�ᳵ��ɲ��
		{
			if(Meeting_My.State==5)
			{
				if(Get_Pules_L<20)
					MotorPwmLeft=0;
				else
					MotorPwmLeft=-980;
				
				if(Get_Pules_R<20)
					MotorPwmRight=0;
				else
					MotorPwmRight=-980;
				if(Get_Pules_L<20&&Get_Pules_R<20)
				{
					TIME++;
				}
				if(TIME>4)
				{
					Brake.Flag=0;
					Meeting_My.Brake=0;
					TIME=0;
				}
			}
			else
			{
				Brake_BB(20,20,8,prt);
				if(Brake.Flag==0)
				{
					Meeting_My.Brake=0;
				}
			}
		}
		
		else if(prt->State==5)//�ᳵ��ͣ��
		{
			if(Get_Pules_L-LSpeedSet<10)
				MotorPwmLeft=0;
			else
				MotorPwmLeft=-980;
			
			if(Get_Pules_R-RSpeedSet<10)
				MotorPwmRight=0;
			else
				MotorPwmRight=-980;
			
			if(Meeting_My.Stop_Flag==0)
			{
				prt->Flag=0;
				prt->State=0;
			}
		}
		
	}
		else 
	{
		//��������ɲ��
		Brake_R(RSpeedSet,3);
		Brake_L(LSpeedSet,3);
	}
			break;
		}
	case Low_Level:
		{
			if(prt->Flag)
	{
		if(prt->State==1)
		{
			Brake_HD (LSpeedSet,RSpeedSet,6,prt);
		}
		else if(prt->State==2)
		{
			Brake_BB(LSpeedSet,RSpeedSet,6.5,prt);
			
		}
		else if(prt->State==3)//�ᳵ������
		{
			Brake_BB(70,70,3,prt);
			if(Brake.Flag==0)
				Meeting_My.Brake=0;
		}
		else if(prt->State==4)//�ᳵ��ɲ��
		{
			if(Meeting_My.State==5)
			{
				if(Get_Pules_L<20)
					MotorPwmLeft=0;
				else
					MotorPwmLeft=-980;
				
				if(Get_Pules_R<20)
					MotorPwmRight=0;
				else
					MotorPwmRight=-980;
				if(Get_Pules_L<20&&Get_Pules_R<20)
				{
					TIME++;
				}
				if(TIME>4)
				{
					Brake.Flag=0;
					Meeting_My.Brake=0;
					TIME=0;
				}
			}
			else
			{
				Brake_BB(20,20,8,prt);
				if(Brake.Flag==0)
				{
					Meeting_My.Brake=0;
				}
			}
		}
		
		else if(prt->State==5)//�ᳵ��ͣ��
		{
			if(Get_Pules_L-LSpeedSet<10)
				MotorPwmLeft=0;
			else
				MotorPwmLeft=-980;
			
			if(Get_Pules_R-RSpeedSet<10)
				MotorPwmRight=0;
			else
				MotorPwmRight=-980;
			
			if(Meeting_My.Stop_Flag==0)
			{
				prt->Flag=0;
				prt->State=0;
			}
		}
		
	}
		else 
	{
		//��������ɲ��
		Brake_R(RSpeedSet,3);
		Brake_L(LSpeedSet,3);
	}
		//	Beep_ON;
			break;
		}
		
	}
	
	
	
	
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �����ٶ�
//  @param      NULL
//  @return     void		//���ж��е���			
//  @since      v1.0   
//  Sample usage: 8msһ�� �����ٶ�PWM �����ٶ�ƫ��
//-------------------------------------------------------------------------------------------------------------------
void Motor_Control(void)
{
	
	Calculate_Error(LSpeedSet,RSpeedSet) ;//�趨�ٶ��뵱ǰ�ٶ�֮��ƫ��
	
	
	/****************************����PID***********************************/
	RPwmAdd+=(SpeedI)*SpeedError_R+(SpeedP)*(SpeedError_R-SpeedLastError_R)+SpeedD*(SpeedError_R-2*SpeedLastError_R+SpeedPerError_R);
	LPwmAdd+=(SpeedI)*SpeedError_L+(SpeedP)*(SpeedError_L-SpeedLastError_L)+SpeedD*(SpeedError_L-2*SpeedLastError_L+SpeedPerError_L);
	
	
	if(Reduce_ADD==1)
	{
		RPwmAdd-=200;
		LPwmAdd-=200;
		Reduce_ADD=0;
	}
	MotorPwmRight=(int)(RPwmAdd);
	MotorPwmLeft=(int)(LPwmAdd);
	
	Brake_Car(&Brake);
	
	if(MotorPwmLeft<=-900)               MotorPwmLeft=-900;
	if(MotorPwmLeft>=900)             MotorPwmLeft=900;
	if(MotorPwmRight<=-900)              MotorPwmRight=-900;
	if(MotorPwmRight>=900)            MotorPwmRight=900; 
	
	
	
	Motor_Pwm_Out(MotorPwmLeft,MotorPwmRight);//�������
	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ٶ��ж�
//  @param      NULL
//  @return     void					
//  @since      v1.0   
//  Sample usage: 8msһ�� �����ٶ�PWM �����ٶ�ƫ��
//-------------------------------------------------------------------------------------------------------------------
void PIT0_IRQHandler(void)
{
	disable_irq(PIT0_IRQn);            //��PIT�ж�
	PIT_FlAG_CLR(pit0);	               //���жϱ�־λv
	
#if (CarMode==AE86)
	Pulse_FTM1=ftm_quad_get(ftm1);
	ftm_quad_clean(ftm1);//�����Ĵ�������
	Pulse_FTM2=-ftm_quad_get(ftm2);//ȡ�Ĵ�����ֵ
	ftm_quad_clean(ftm2);//�����Ĵ�������
	
	Get_Pules_L=Pulse_FTM1;  //���ֺ���β�õ�������1msһ����õ�����//���ֺ���β�õ�������1msһ����õ�����
	Get_Pules_R=Pulse_FTM2;
	
	Pulse_Last_FTM2=Pulse_FTM2;
	Pulse_Last_FTM1=Pulse_FTM1;
#else	
	Pulse_FTM1=-ftm_quad_get(ftm1);
	ftm_quad_clean(ftm1);//�����Ĵ�������
	Pulse_FTM2=ftm_quad_get(ftm2);//ȡ�Ĵ�����ֵ
	ftm_quad_clean(ftm2);//�����Ĵ�������
	
	Get_Pules_L=Pulse_FTM2;  //���ֺ���β�õ�������1msһ����õ�����//���ֺ���β�õ�������1msһ����õ�����
	Get_Pules_R=Pulse_FTM1;
	
	Pulse_Last_FTM2=Pulse_FTM2;
	Pulse_Last_FTM1=Pulse_FTM1;
	
#endif	
	
	
	
	if(Roundabout.Flag==1&&(Roundabout.State==3||Roundabout.State==4||Roundabout.State==2))
	{
		Roundabout.Distance+=(Pulse_FTM2+Pulse_FTM1)/2;//
	}
	
	Motor_Control();
//		Motor_Pwm_Out(400,00);
	PIT_FlAG_CLR(pit0);        //���PIT0��־
	enable_irq(PIT0_IRQn);//ʹ���ж�
	
	
}

void Stright_Judge()
{	
		static int time=0;
	if(DropRow_Straight<=65&&DropRow_Straight>=53&&ABS(Error)<=15&&DropRow<=65&&DropRow>=55&&(Right_Quantity+Left_Quantity)>=80&&(Stright_Flag==0)&&!Roundabout.Flag&&ABS(MiddleLine[DropRow+2]-70)<=8)	{
			time++;
			if(time>=5)
			{ 
				time=0;
				Stright_Flag=1;
				//Beep_ON;
			}
		}
		else 
		{
			time=0;
		}

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ٶȼ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void HighestSpeed_Set()
{
	
	static int Stright_Times;
	
	if(Meeting_My.State<=1&&Meeting_My.End==0)
	{
		SpeedSet_High=30;
		if(Meeting_My.Stop_Flag)
		{
			SpeedSet_High=0;
			Brake.Flag=1;
			Brake.State=5;
		}
	}
	else if(Meeting_My.State>1&&Meeting_My.Flag&&Meeting_My.End==0)
	{
		if(Meeting_My.Brake==2)//ͣ��
		{
			Brake.Flag=1;
			Brake.State=4;
			SpeedSet_High=0;
		}
		if(Meeting_My.Brake==1)//����
		{
			Brake.Flag=1;
			Brake.State=3;
			SpeedSet_High=130;
		}
		if(Meeting_My.Brake==0)//�ᳵ���ٶ�
		{
			if(Meeting_My.Stop_Flag)
			{
				SpeedSet_High=0;
				Brake.Flag=1;
				Brake.State=5;
			}
			else
			{
				if(Meeting_My.State==3)
				{
					if(Meeting_Area.Mode==0)
					{
						if(Meeting_My.Oder==1)//��һ̨���ٶ�
							SpeedSet_High=90;
						else if(Meeting_My.Oder==2)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=130;
					}
					else
					{
						if(Meeting_My.Oder==1)
							SpeedSet_High=Meeting_Curve_First;
						else if(Meeting_My.Oder==2)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=Meeting_Curve_Second;
					}
				}
				else
				{
					if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
						SpeedSet_High=90;
					else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
						SpeedSet_High=130;
				}
			}
		}
		
	}
	//������
	else
	{
     Stright_Judge();
		//��ֱ��
		if(Stright_Flag) 
		{
			//		SpeedI=0.12;
			Stright_Times++;
			
			if(Roundabout.Flag)
			{
				Stright_Flag=0;
				Brake.Flag=1;
				Stright_Times=0;
				Brake.State=1;
				
				SpeedSet_High=130;
			}
			else if(Crossroad.Flag)
			{
				Stright_Times=0;
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=140;
				}
				else
				{
					Stright_Flag=0;
					SpeedSet_High=130;
				}
			}
			else if(Rampway.Flag)
			{
				Stright_Times=0;
				Stright_Flag=0;
				//	Brake.Flag=1;
				Stright_Times=0;
				
				//	Brake.State=1;
				SpeedSet_High=90;
				
			}
			else  if((DropRow<=DropRow_Straight&Stright_Flag&&(DropRow_Straight-DropRow)>=2&&ABS(Error)>=12&&DropRow>=71))
			{
				Stright_Flag=0;
				
				if(Stright_Times>=10)
				{ 
					//LPwmAdd-=100;
					//	RPwmAdd-=100;
					Brake.Flag=1;
					Brake.State=2;
				} 
				// Beep_ON;//Beep_OFF;
				Stright_Times=0;
				SpeedSet_High=130;
			}
			else //������ֱ���ٶ�
			{
				SpeedSet_High=145;
			}
		}
		//�ǳ�ֱ��
		else if(Brake.Flag==0)
		{
			//	SpeedI=0.08;
			if(Rampway.Flag)
			{
				
				if(Rampway.State==uphill)
					SpeedSet_High=100;
				else if(Rampway.State==peak)
					SpeedSet_High=100;
				else if(Rampway.State==downhill)
				{	
					
					SpeedSet_High=100;
				}
				
			}
			else if(Roundabout.Flag)
			{
				SpeedSet_High=145;
				if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5||Roundabout.State==6))
				{
					SpeedSet_High=135;
					if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5))
					{	
						
						Brake.Flag=1;		
						Brake.State=1;
					}
				}
				else if(Roundabout.Size==0)
				{
					if(Roundabout.State==1||Roundabout.State==2||Roundabout.State==5||Roundabout.State==6)
					{
						SpeedSet_High=130;
						if(Roundabout.State==1||Roundabout.State==5)
						{
							
							Brake.Flag=1;
							Brake.State=1;
						}
					}
					else
					{
						SpeedSet_High=130;
					}
					
				}
				if(Roundabout.State==6&&Roundabout.Size==0)
				{
					SpeedSet_High=130;
				}
			}
			else if(Crossroad.Flag)
			{
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=140;
				}
				else
				{
					SpeedSet_High=130;
				}
			}
			else
			{
				SpeedSet_High=130;
				if(Meeting_My.End==1&&Meeting_Area.Mode==1&&Meeting_My.State==3)
				{
					SpeedSet_High=Meeting_Curve_End;
				}
			}
		}
		
	}
	
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ٶȼ��� �����ٶȼ��� ����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void Speed_Set()   
{
	float  S_Error;
	float  S_LastError;
	
	//�趨PID	
#if (CarMode==AE86)
	SpeedP=10;//4
	SpeedI=0.08;
	SpeedD=0;
#else
	SpeedP=10;
	SpeedI=0.08;
	SpeedD=0;  
#endif
	
	HighestSpeed_Set();//�����ʼ����
	
	//	if(ABS(Error)>=10) 
	//		SpeedSet=(int32)(1.2*SpeedDropRow-0.04*(Error*Error)-0.1*((Error)-(LastError)));//SpeedSet=105-((0.3)*ABS(Error)-(0.4)*(LastError-Error));
	//	else
	//		
	//		if(ABS(Error)>=20)
	//			SpeedSet=SpeedDropRow-0.05*(Error*Error);
	S_Error=Error;
	S_LastError=LastError;
	if(ABS(S_Error)>=40)
		S_Error=40;
	if(ABS(S_LastError)>=40)
		S_LastError=40;
	
	SpeedSet=(int32)(SpeedSet_High-0.02*(S_Error*S_Error)-0.00*(ABS(S_Error)-ABS(S_LastError)));
	
	SpeedSet=SpeedSet<=60?60:SpeedSet;
	SpeedSet=SpeedSet>=180?180:SpeedSet;
	
	/********************�������***********************/
	
	Angle = SteerPwm- SteerMiddle;
	if(Angle>10) //����
	{
		AA=(int16)(45*(Angle)*1.0/(SteerMax - SteerMiddle));  //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;         //��һ���������ԼӴ���٣��ڶ�������������ǰ����,�������������Ըı������ֵĲ��ٴ�С��ֵ
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet=(int32)(1.0 * SpeedSet * (1.0 - 1.2*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet=(int32)(1.0 * SpeedSet * (1.0 + 1.1*Temp_Orr));
		
	}
	else if(Angle<-10) //����
	{
		AA=(int16)(45*(-Angle)*1.0/(SteerMiddle - SteerMin)); //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet = (int32)(1.0 * SpeedSet * (1.0 +  1.1*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet = (int32)(1.0 * SpeedSet * (1.0 -  1.2* Temp_Orr));
		
	}
	else
	{
		LSpeedSet=SpeedSet;
		RSpeedSet=SpeedSet;
	}
	
	if(RSpeedSet<=20)    RSpeedSet=20;
	if(RSpeedSet>=220)  RSpeedSet=220; 
	if(LSpeedSet<=20)    LSpeedSet=20;
	if(LSpeedSet>=220)  LSpeedSet=220; 
	
	
	if((Meeting_My.Flag||Meeting_My.State==0))//&&(Meeting_My.Stop_Flag==0||(Meeting_My.Stop_Flag&&Meeting_My.Brake))
	{
		if(SpeedSet_High!=0)
		{
			if(SpeedError_L<=-25)
				LPwmAdd=100;
			if(SpeedError_R<=-25)
				RPwmAdd=100;
			RSpeedSet=SpeedSet_High+Error*0.5;;//+Error*0.5;
			LSpeedSet=SpeedSet_High-Error*0.5;;//-Error*0.5;
			//		RSpeedSet=(int)(SpeedSet_High+Error*0.5);
			//		LSpeedSet=(int)(SpeedSet_High-Error*0.5);
		}
		else
		{
			RSpeedSet=SpeedSet_High;//+Error*0.5;
			LSpeedSet=SpeedSet_High;//-Error*0.5;
		}
	}
	
	
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ٶȼ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void Fast_Speed_Set()
{

	static int Stright_Times;
	if(Meeting_My.State<=1&&Meeting_My.End==0)
	{
		SpeedSet_High=30;
		if(Meeting_My.Stop_Flag)
		{
			SpeedSet_High=0;
			Brake.Flag=1;
				Brake.State=5;
		}
	}
	else if(Meeting_My.State>1&&Meeting_My.Flag&&Meeting_My.End==0)
	{
		if(Meeting_My.Brake==2)//ͣ��
		{
			Brake.Flag=1;
			Brake.State=4;
			SpeedSet_High=0;
		}
		if(Meeting_My.Brake==1)//����
		{
			Brake.Flag=1;
			Brake.State=3;
			SpeedSet_High=130;
		}
		if(Meeting_My.Brake==0)//�ᳵ���ٶ�
		{
			if(Meeting_My.Stop_Flag)
			{
				SpeedSet_High=0;
				Brake.Flag=1;
				Brake.State=5;
			}
			else
			{
				if(Meeting_My.State==3)
				{
					if(Meeting_Area.Mode==0)
					{
						if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
							SpeedSet_High=90;
						else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=130;
					}
					else
					{
						if(Meeting_My.Oder==1)
							SpeedSet_High=Meeting_Curve_First;
						else if(Meeting_My.Oder==2)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=Meeting_Curve_Second;
					}
				}
				else
				{
					if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
						SpeedSet_High=90;
					else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
						SpeedSet_High=130;
				}
			}
		}
		
	}
	//������
	else
	{
	 Stright_Judge();
		//��ֱ��
		if(Stright_Flag) 
		{
			//		SpeedI=0.12;
			Stright_Times++;
			
			if(Roundabout.Flag)
			{
				Stright_Flag=0;
				Brake.Flag=1;
				Stright_Times=0;
				Brake.State=1;
				
				SpeedSet_High=130;
			}
			else if(Crossroad.Flag)
			{
				Stright_Times=0;
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=150;
				}
				else
				{
					Stright_Flag=0;
					SpeedSet_High=140;
				}
			}
			else if(Rampway.Flag)
			{
				Stright_Times=0;
				Stright_Flag=0;
				Brake.Flag=1;
				Stright_Times=0;
				
				Brake.State=1;
				SpeedSet_High=100;
				
			}
			else  if((DropRow<=DropRow_Straight&Stright_Flag&&(DropRow_Straight-DropRow)>=2&&ABS(Error)>=12&&DropRow>=71))
			{
				Stright_Flag=0;
				
				if(Stright_Times>=10)
				{ 
				//	LPwmAdd-=100;
					//RPwmAdd-=100;
					Brake.Flag=1;
					Brake.State=2;
				} 
				// Beep_ON;//Beep_OFF;
				Stright_Times=0;
				SpeedSet_High=140;
			}
			else //������ֱ���ٶ�
			{
				SpeedSet_High=160;
			}
		}
		//�ǳ�ֱ��
		else if(Brake.Flag==0)
		{
			//	SpeedI=0.08;
			if(Rampway.Flag)
			{
				
				if(Rampway.State==uphill)
					SpeedSet_High=100;
				else if(Rampway.State==peak)
					SpeedSet_High=100;
				else if(Rampway.State==downhill)
				{	
					
					SpeedSet_High=100;
				}
	
				
			}
			else if(Roundabout.Flag)
			{
				SpeedSet_High=155;
				if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5||Roundabout.State==6))
				{
					SpeedSet_High=140;
					if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5))
					{	
						
						Brake.Flag=1;		
						Brake.State=1;
					}
				}
				else if(Roundabout.Size==0)
				{
					if(Roundabout.State==1||Roundabout.State==2||Roundabout.State==5||Roundabout.State==6)
					{
						SpeedSet_High=130;
						if(Roundabout.State==1||Roundabout.State==5)
						{
							
							Brake.Flag=1;
							Brake.State=1;
						}
					}
					else
					{
						SpeedSet_High=130;
					}
					
				}
				if(Roundabout.State==6&&Roundabout.Size==0)
				{
					SpeedSet_High=130;
				}
			}
			else if(Crossroad.Flag)
			{
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=140;
				}
				else
				{
					SpeedSet_High=130;
				}
			}
			else
			{
				SpeedSet_High=140;
				if(Meeting_My.End==1&&Meeting_Area.Mode==1&&Meeting_My.State==3)
				{
					SpeedSet_High=Meeting_Curve_End;
				}
			}
		}
		
	}
	
	
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ٶȼ��� �����ٶȼ��� ����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void Fast_Set()   
{
	float  S_Error;
	float  S_LastError;
	
	//�趨PID	
#if (CarMode==AE86)
	SpeedP=12;//4
	SpeedI=0.1;
	SpeedD=0;
#else
	SpeedP=14;
	SpeedI=0.12;
	SpeedD=0;  
#endif
	
	Fast_Speed_Set();//�����ʼ����
	
	//	if(ABS(Error)>=10) 
	//		SpeedSet=(int32)(1.2*SpeedDropRow-0.04*(Error*Error)-0.1*((Error)-(LastError)));//SpeedSet=105-((0.3)*ABS(Error)-(0.4)*(LastError-Error));
	//	else
	//		
	//		if(ABS(Error)>=20)
	//			SpeedSet=SpeedDropRow-0.05*(Error*Error);
	S_Error=Error;
	S_LastError=LastError;
	if(ABS(S_Error)>=35)
		S_Error=35;
	if(ABS(S_LastError)>=35)
		S_LastError=35;
	
	SpeedSet=(int32)(SpeedSet_High-0.022*(S_Error*S_Error)-0.00*(ABS(S_Error)-ABS(S_LastError)));
	
	SpeedSet=SpeedSet<=60?60:SpeedSet;
	SpeedSet=SpeedSet>=180?180:SpeedSet;
	
	/********************�������***********************/
	
	Angle = SteerPwm- SteerMiddle;
	if(Angle>10) //����
	{
		AA=(int16)(45*(Angle)*1.0/(SteerMax - SteerMiddle));  //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;         //��һ���������ԼӴ���٣��ڶ�������������ǰ����,�������������Ըı������ֵĲ��ٴ�С��ֵ
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet=(int32)(1.0 * SpeedSet * (1.0 - 1.22*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet=(int32)(1.0 * SpeedSet * (1.0 + 1.12*Temp_Orr));
		
	}
	else if(Angle<-10) //����
	{
		AA=(int16)(45*(-Angle)*1.0/(SteerMiddle - SteerMin)); //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet = (int32)(1.0 * SpeedSet * (1.0 +  1.12*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet = (int32)(1.0 * SpeedSet * (1.0 -  1.22* Temp_Orr));
		
	}
	else
	{
		LSpeedSet=SpeedSet;
		RSpeedSet=SpeedSet;
	}
	
	if(RSpeedSet<=20)    RSpeedSet=20;
	if(RSpeedSet>=220)  RSpeedSet=220; 
	if(LSpeedSet<=20)    LSpeedSet=20;
	if(LSpeedSet>=220)  LSpeedSet=220; 
	
	
	if((Meeting_My.Flag||Meeting_My.State==0))//&&(Meeting_My.Stop_Flag==0||(Meeting_My.Stop_Flag&&Meeting_My.Brake))
	{
		if(SpeedSet_High!=0)
		{
			if(SpeedError_L<=-25)
				LPwmAdd=100;
			if(SpeedError_R<=-25)
				RPwmAdd=100;
			RSpeedSet=SpeedSet_High+Error*0.5;;//+Error*0.5;
			LSpeedSet=SpeedSet_High-Error*0.5;;//-Error*0.5;
			//		RSpeedSet=(int)(SpeedSet_High+Error*0.5);
			//		LSpeedSet=(int)(SpeedSet_High-Error*0.5);
		}
		else
		{
			RSpeedSet=SpeedSet_High;//+Error*0.5;
			LSpeedSet=SpeedSet_High;//-Error*0.5;
		}
	}
	
	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ٶȼ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void Low_Speed_Set()
{

	static int Stright_Times;
	if(Meeting_My.State<=1&&Meeting_My.End==0)
	{
		SpeedSet_High=30;
		if(Meeting_My.Stop_Flag)
		{
			SpeedSet_High=0;
			Brake.Flag=1;
				Brake.State=5;
		}
	}
	else if(Meeting_My.State>1&&Meeting_My.Flag&&Meeting_My.End==0)
	{
		if(Meeting_My.Brake==2)//ͣ��
		{
			Brake.Flag=1;
			Brake.State=4;
			SpeedSet_High=0;
		}
		if(Meeting_My.Brake==1)//����
		{
			Brake.Flag=1;
			Brake.State=3;
			SpeedSet_High=130;
		}
		if(Meeting_My.Brake==0)//�ᳵ���ٶ�
		{
			if(Meeting_My.Stop_Flag)
			{
				SpeedSet_High=0;
				Brake.Flag=1;
				Brake.State=5;
			}
			else
			{
				if(Meeting_My.State==3)
				{
					if(Meeting_Area.Mode==0)
					{
						if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
							SpeedSet_High=90;
						else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=130;
					}
					else
					{
						if(Meeting_My.Oder==1)
							SpeedSet_High=Meeting_Curve_First;
						else if(Meeting_My.Oder==2)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=Meeting_Curve_Second;
					}
				}
				else
				{
					if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
						SpeedSet_High=90;
					else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
						SpeedSet_High=130;
				}
			}
		}
		
	}
	//������
	else
	{
	 Stright_Judge();
		//��ֱ��
		if(Stright_Flag) 
		{
			//		SpeedI=0.12;
			Stright_Times++;
			
			if(Roundabout.Flag)
			{
				Stright_Flag=0;
				Brake.Flag=1;
				Stright_Times=0;
				Brake.State=1;
				
				SpeedSet_High=120;
			}
			else if(Crossroad.Flag)
			{
				Stright_Times=0;
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=130;
				}
				else
				{
					Stright_Flag=0;
					SpeedSet_High=120;
				}
			}
			else if(Rampway.Flag)
			{
				Stright_Times=0;
				Stright_Flag=0;
					Brake.Flag=1;
				Stright_Times=0;
				
				Brake.State=1;
				SpeedSet_High=90;
				
			}
			else  if((DropRow<=DropRow_Straight&Stright_Flag&&(DropRow_Straight-DropRow)>=2&&ABS(Error)>=12&&DropRow>=71))
			{
				Stright_Flag=0;
				
				if(Stright_Times>=10)
				{ 
				//	LPwmAdd-=100;
					//RPwmAdd-=100;
					Brake.Flag=1;
					Brake.State=2;
				} 
				// Beep_ON;//Beep_OFF;
				Stright_Times=0;
				SpeedSet_High=125;
			}
			else //������ֱ���ٶ�
			{
				SpeedSet_High=130;
			}
		}
		//�ǳ�ֱ��
		else if(Brake.Flag==0)
		{
			//	SpeedI=0.08;
			if(Rampway.Flag)
			{
				if(Rampway.State==uphill)
					SpeedSet_High=90;
				else if(Rampway.State==peak)
					SpeedSet_High=90;
				else if(Rampway.State==downhill)
				{	
					
					SpeedSet_High=90;
				}
			}
			else if(Roundabout.Flag)
			{
				SpeedSet_High=145;
				if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5||Roundabout.State==6))
				{
					SpeedSet_High=130;
					if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5))
					{	
						
						Brake.Flag=1;		
						Brake.State=1;
					}
				}
				else if(Roundabout.Size==0)
				{
					if(Roundabout.State==1||Roundabout.State==2||Roundabout.State==5||Roundabout.State==6)
					{
						SpeedSet_High=125;
						if(Roundabout.State==1||Roundabout.State==5)
						{
							
							Brake.Flag=1;
							Brake.State=1;
						}
					}
					else
					{
						SpeedSet_High=125;
					}
					
				}
				if(Roundabout.State==6)
				{
					SpeedSet_High=130;
				}
			}
			else if(Crossroad.Flag)
			{
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=130;
				}
				else
				{
					SpeedSet_High=120;
				}
			}
			else
			{
				SpeedSet_High=126;
				if(Meeting_My.End==1&&Meeting_Area.Mode==1&&Meeting_My.State==3)
				{
					SpeedSet_High=Meeting_Curve_End;
				}
			}
		}
		
	}
	
	
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ٶȼ��� �����ٶȼ��� ����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void Low_Set()   
{
	float  S_Error;
	float  S_LastError;
	
	//�趨PID	
#if (CarMode==AE86)
	SpeedP=12;//4
	SpeedI=0.1;
	SpeedD=0;
#else
	SpeedP=12;
	SpeedI=0.1;
	SpeedD=0;  
#endif
	
	Low_Speed_Set();//�����ʼ����
	
	//	if(ABS(Error)>=10) 
	//		SpeedSet=(int32)(1.2*SpeedDropRow-0.04*(Error*Error)-0.1*((Error)-(LastError)));//SpeedSet=105-((0.3)*ABS(Error)-(0.4)*(LastError-Error));
	//	else
	//		
	//		if(ABS(Error)>=20)
	//			SpeedSet=SpeedDropRow-0.05*(Error*Error);
	S_Error=Error;
	S_LastError=LastError;
	if(ABS(S_Error)>=35)
		S_Error=35;
	if(ABS(S_LastError)>=35)
		S_LastError=35;
	
	SpeedSet=(int32)(SpeedSet_High-0.018*(S_Error*S_Error)-0.00*(ABS(S_Error)-ABS(S_LastError)));
	
	SpeedSet=SpeedSet<=60?60:SpeedSet;
	SpeedSet=SpeedSet>=180?180:SpeedSet;
	
	/********************�������***********************/
	
	Angle = SteerPwm- SteerMiddle;
	if(Angle>10) //����
	{
		AA=(int16)(45*(Angle)*1.0/(SteerMax - SteerMiddle));  //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;         //��һ���������ԼӴ���٣��ڶ�������������ǰ����,�������������Ըı������ֵĲ��ٴ�С��ֵ
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet=(int32)(1.0 * SpeedSet * (1.0 - 1.2*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet=(int32)(1.0 * SpeedSet * (1.0 + 1.12*Temp_Orr));
		
	}
	else if(Angle<-10) //����
	{
		AA=(int16)(45*(-Angle)*1.0/(SteerMiddle - SteerMin)); //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet = (int32)(1.0 * SpeedSet * (1.0 +  1.12*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet = (int32)(1.0 * SpeedSet * (1.0 -  1.2* Temp_Orr));
		
	}
	else
	{
		LSpeedSet=SpeedSet;
		RSpeedSet=SpeedSet;
	}
	
	if(RSpeedSet<=20)    RSpeedSet=20;
	if(RSpeedSet>=220)  RSpeedSet=220; 
	if(LSpeedSet<=20)    LSpeedSet=20;
	if(LSpeedSet>=220)  LSpeedSet=220; 
	
	
	if((Meeting_My.Flag||Meeting_My.State==0))//&&(Meeting_My.Stop_Flag==0||(Meeting_My.Stop_Flag&&Meeting_My.Brake))
	{
		if(SpeedSet_High!=0)
		{
			if(SpeedError_L<=-25)
				LPwmAdd=100;
			if(SpeedError_R<=-25)
				RPwmAdd=100;
			RSpeedSet=SpeedSet_High+Error*0.5;;//+Error*0.5;
			LSpeedSet=SpeedSet_High-Error*0.5;;//-Error*0.5;
			//		RSpeedSet=(int)(SpeedSet_High+Error*0.5);
			//		LSpeedSet=(int)(SpeedSet_High-Error*0.5);
		}
		else
		{
			RSpeedSet=SpeedSet_High;//+Error*0.5;
			LSpeedSet=SpeedSet_High;//-Error*0.5;
		}
	}
	
	
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������ٶȼ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void FastFF_Speed_Set()
{

	static int Stright_Times;
	if(Meeting_My.State<=1&&Meeting_My.End==0)
	{
		SpeedSet_High=30;
		if(Meeting_My.Stop_Flag)
		{
			SpeedSet_High=0;
			Brake.Flag=1;
				Brake.State=5;
		}
	}
	else if(Meeting_My.State>1&&Meeting_My.Flag&&Meeting_My.End==0)
	{
		if(Meeting_My.Brake==2)//ͣ��
		{
			Brake.Flag=1;
			Brake.State=4;
			SpeedSet_High=0;
		}
		if(Meeting_My.Brake==1)//����
		{
			Brake.Flag=1;
			Brake.State=3;
			SpeedSet_High=130;
		}
		if(Meeting_My.Brake==0)//�ᳵ���ٶ�
		{
			if(Meeting_My.Stop_Flag)
			{
				SpeedSet_High=0;
				Brake.Flag=1;
				Brake.State=5;
			}
			else
			{
				if(Meeting_My.State==3)
				{
					if(Meeting_Area.Mode==0)
					{
						if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
							SpeedSet_High=90;
						else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=130;
					}
					else
					{
						if(Meeting_My.Oder==1)
							SpeedSet_High=Meeting_Curve_First;
						else if(Meeting_My.Oder==2)//�ڶ�̨���ٶ�  �����������Ե�
							SpeedSet_High=Meeting_Curve_Second;
					}
				}
				else
				{
					if(Meeting_My.Oder==1&&Meeting_My.End==0)//��һ̨���ٶ�
						SpeedSet_High=90;
					else if(Meeting_My.Oder==2||Meeting_My.End)//�ڶ�̨���ٶ�  �����������Ե�
						SpeedSet_High=130;
				}
			}
		}
		
	}
	//������
	else
	{
	 Stright_Judge();
		//��ֱ��
		if(Stright_Flag) 
		{
			//		SpeedI=0.12;
			Stright_Times++;
			
			if(Roundabout.Flag)
			{
				Stright_Flag=0;
				Brake.Flag=1;
				Stright_Times=0;
				Brake.State=1;
				
				SpeedSet_High=140;
			}
			else if(Crossroad.Flag)
			{
				Stright_Times=0;
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=150;
				}
				else
				{
					Stright_Flag=0;
					SpeedSet_High=140;
				}
			}
			else if(Rampway.Flag)
			{
				Stright_Times=0;
				Stright_Flag=0;
					Brake.Flag=1;
				Stright_Times=0;
				
				Brake.State=1;
				SpeedSet_High=90;
				
			}
			else  if((DropRow<=DropRow_Straight&Stright_Flag&&(DropRow_Straight-DropRow)>=2&&ABS(Error)>=12&&DropRow>=71))
			{
				Stright_Flag=0;
				
				if(Stright_Times>=10)
				{ 
					Brake.Flag=1;
					Brake.State=2;
				} 
				// Beep_ON;//Beep_OFF;
				Stright_Times=0;
				SpeedSet_High=145;
			}
			else //������ֱ���ٶ�
			{
				SpeedSet_High=165;
			}
		}
		//�ǳ�ֱ��
		else if(Brake.Flag==0)
		{
			//	SpeedI=0.08;
			if(Rampway.Flag)
			{
				if(Rampway.State==uphill)
					SpeedSet_High=90;
				else if(Rampway.State==peak)
					SpeedSet_High=90;
				else if(Rampway.State==downhill)
				{	
					
					SpeedSet_High=90;
				}
			}
			else if(Roundabout.Flag)
			{
				SpeedSet_High=155;
				if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5||Roundabout.State==6))
				{
					SpeedSet_High=140;
					if(Roundabout.Size&&(Roundabout.State==2||Roundabout.State==5))
					{	
						
						Brake.Flag=1;		
						Brake.State=1;
					}
				}
				else if(Roundabout.Size==0)
				{
					if(Roundabout.State==1||Roundabout.State==2||Roundabout.State==5||Roundabout.State==6)
					{
						SpeedSet_High=130;
						if(Roundabout.State==1||Roundabout.State==5)
						{
							
							Brake.Flag=1;
							Brake.State=1;
						}
					}
					else
					{
						SpeedSet_High=140;
					}
					
				}
				if(Roundabout.State==6)
				{
					SpeedSet_High=130;
				}
			}
			else if(Crossroad.Flag)
			{
				if(Crossroad.Direction==Straight)
				{
					SpeedSet_High=145;
				}
				else
				{
					SpeedSet_High=135;
				}
			}
			else
			{
				SpeedSet_High=145;
				if(Meeting_My.End==1&&Meeting_Area.Mode==1&&Meeting_My.State==3)
				{
					SpeedSet_High=Meeting_Curve_End;
				}
			}
		}
		
	}
	
	
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ٶȼ��� �����ٶȼ��� ����
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:	����������
//-------------------------------------------------------------------------------------------------------------------
void FastFast_Set()   
{
	float  S_Error;
	float  S_LastError;
	
	//�趨PID	
#if (CarMode==AE86)
	SpeedP=12;//4
	SpeedI=0.12;
	SpeedD=0;
#else
	SpeedP=12;
	SpeedI=0.12;
	SpeedD=0;  
#endif
	
	FastFF_Speed_Set();//�����ʼ����
	
	//	if(ABS(Error)>=10) 
	//		SpeedSet=(int32)(1.2*SpeedDropRow-0.04*(Error*Error)-0.1*((Error)-(LastError)));//SpeedSet=105-((0.3)*ABS(Error)-(0.4)*(LastError-Error));
	//	else
	//		
	//		if(ABS(Error)>=20)
	//			SpeedSet=SpeedDropRow-0.05*(Error*Error);
	S_Error=Error;
	S_LastError=LastError;
	if(ABS(S_Error)>=35)
		S_Error=35;
	if(ABS(S_LastError)>=35)
		S_LastError=35;
	
	SpeedSet=(int32)(SpeedSet_High-0.023*(S_Error*S_Error)-0.00*(ABS(S_Error)-ABS(S_LastError)));
	
	SpeedSet=SpeedSet<=60?60:SpeedSet;
	SpeedSet=SpeedSet>=180?180:SpeedSet;
	
	/********************�������***********************/
	
	Angle = SteerPwm- SteerMiddle;
	if(Angle>10) //����
	{
		AA=(int16)(45*(Angle)*1.0/(SteerMax - SteerMiddle));  //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;         //��һ���������ԼӴ���٣��ڶ�������������ǰ����,�������������Ըı������ֵĲ��ٴ�С��ֵ
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet=(int32)(1.0 * SpeedSet * (1.0 - 1.21*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet=(int32)(1.0 * SpeedSet * (1.0 + 1.12*Temp_Orr));
		
	}
	else if(Angle<-10) //����
	{
		AA=(int16)(45*(-Angle)*1.0/(SteerMiddle - SteerMin)); //angle_max   45
		if(AA>45)  AA=45;
		if(AA<0)   AA=0;
		Temp_Orr = tan((AA*3.14)/180) * 15.4 / 40;
		LSpeedSet = (int32)(1.0 * SpeedSet * (1.0 +  1.12*Temp_Orr)); //Temp_Orr 0-0.38
		RSpeedSet = (int32)(1.0 * SpeedSet * (1.0 -  1.21* Temp_Orr));
		
	}
	else
	{
		LSpeedSet=SpeedSet;
		RSpeedSet=SpeedSet;
	}
	
	if(RSpeedSet<=20)    RSpeedSet=20;
	if(RSpeedSet>=220)  RSpeedSet=220; 
	if(LSpeedSet<=20)    LSpeedSet=20;
	if(LSpeedSet>=220)  LSpeedSet=220; 
	
	
	if((Meeting_My.Flag||Meeting_My.State==0))//&&(Meeting_My.Stop_Flag==0||(Meeting_My.Stop_Flag&&Meeting_My.Brake))
	{
		if(SpeedSet_High!=0)
		{
			if(SpeedError_L<=-25)
				LPwmAdd=100;
			if(SpeedError_R<=-25)
				RPwmAdd=100;
			RSpeedSet=SpeedSet_High+Error*0.5;;//+Error*0.5;
			LSpeedSet=SpeedSet_High-Error*0.5;;//-Error*0.5;
			//		RSpeedSet=(int)(SpeedSet_High+Error*0.5);
			//		LSpeedSet=(int)(SpeedSet_High-Error*0.5);
		}
		else
		{
			RSpeedSet=SpeedSet_High;//+Error*0.5;
			LSpeedSet=SpeedSet_High;//-Error*0.5;
		}
	}
	
	
}



