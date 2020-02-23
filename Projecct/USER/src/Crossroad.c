#include "Crossroad.h"

crossroad Crossroad;
uint16 newedge[140]={0};
//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ж�ʮ�ֺ���
//  @return     void   
//  @since      v1.0
//  Sample usage: ����   &Crossroad      
//-------------------------------------------------------------------------------------------------------------------
void Judge_Crossroad(crossroad *prt)
{
	if(!prt->Flag)//�ж�ʮ��
	{
		//ֱ�� �����йյ� ȫ����>3 �յ������в�Ϊ�߽�
		if(InflectionPointL.Flag==1&&InflectionPointR.Flag==1
		   &&AllLose>=1&&ABS(InflectionPointR.Row-InflectionPointL.Row)<=20
			   &&Solve_Prewitt(InflectionPointL.Row-5,InflectionPointL.Col-1)!=1&&Solve_Prewitt(InflectionPointR.Row-5,InflectionPointR.Col+1)!=1)    //  &&((RightEdge[InflectionPointL.Row-2]==ColumnMax&&LeftEdge[InflectionPointL.Row-2]==0)||AllLose>=3 
		{
			prt->Flag=1;
			prt->Direction=Straight; 
			prt->State=1;
		}
		
		//��б�� ֻ����յ�||��յ���ҹյ����ϴ�
		else if((InflectionPointL.Flag==1&&InflectionPointR.Flag==0)||((InflectionPointL.Row-InflectionPointR.Row)>=15&&InflectionPointL.Flag==1&&InflectionPointR.Flag==1))
		{//�ұ���ʼ�н�С �Ҷ���+ȫ����>45 �ұ߽������� �ҹյ��ϲ�Ϊ�߽�
			uint16 infrow=InflectionPointL.Row<7?7:InflectionPointL.Row;
				   infrow=InflectionPointL.Row>135?135:InflectionPointL.Row;
			uint16 infcol=InflectionPointL.Col>135?135:InflectionPointL.Col;
				   infcol=InflectionPointL.Col<5?5:InflectionPointL.Col;
			if(RightStart<=100 &&(RightLose+AllLoseClose)>=45&&Right_Quantity<=30      
			   &&Solve_Prewitt(infrow-5,infcol-3)==0&&Solve_Prewitt(infrow-6,infcol-3)==0)
			{//��յ��ϵĵ��������ڵ�ɫ�����(Ϊ������)
				if(ABS(image[infrow-3][infcol-2]-image[infrow+3][infcol+4])<=15
				   &&ABS(image[infrow-3][infcol-3]-image[infrow+3][infcol+4])<=15)
				{//��յ��Ӧ���ұ߽� ���� ����յ�ȽϿ������½�
					if(RightEdge[infrow+1]==ColumnMax&&RightEdge[infrow-2]==ColumnMax
					   &&RightEdge[infrow-3]==ColumnMax&&RightEdge[infrow+2]==ColumnMax   
						   &&infcol>=20&&infrow>=70)
					{
						prt->Flag=1;
						prt->Direction=Leftside; 
						prt->State=1;
					}
				}
			}
			
		}
		
		//��б�� ֻ���ҹյ�||���ҹյ����ϴ�
		else if((InflectionPointL.Flag==0&&InflectionPointR.Flag==1)||((InflectionPointR.Row-InflectionPointL.Row)>=15&&InflectionPointL.Flag==1&&InflectionPointR.Flag==1))
		{//�����ʼ�н�С ���ߴ�  ��߽���  ���ҹյ��ϲ�Ϊ�߽�
			uint16 infrow=InflectionPointR.Row<7?7:InflectionPointR.Row;
				   infrow=InflectionPointR.Row>135?135:InflectionPointR.Row;
			uint16 infcol=InflectionPointR.Col>135?135:InflectionPointR.Col;
				   infcol=InflectionPointR.Col<5?5:InflectionPointR.Col;
			if(LeftStart<=100&&(LeftLose+AllLoseClose)>=45&&Left_Quantity<=30
			   &&Solve_Prewitt(infrow-5,infcol+3)==0&&Solve_Prewitt(infrow-6,infcol+3)==0)
			{//�ҹյ��ϵĵ��������ڵ�ɫ�����(Ϊ������)
				if(ABS(image[infrow-4][infcol+2]-image[infrow+3][infcol-4])<=15
				   &&ABS(image[infrow-4][infcol+3]-image[infrow+3][infcol-4])<=15)
				{//�ҹյ��Ӧ����߽� ���� ���ҹյ�ȽϿ������½�
					if(LeftEdge[infrow+1]==0&&LeftEdge[infrow-2]==0
					   &&LeftEdge[infrow+2]==0&&LeftEdge[infrow-3]==0
						   &&infcol<=120&&infrow>=70)
					{
						prt->Flag=1;
						prt->Direction=Rightside; 
						prt->State=1;
					}
				}
			}
		}
	}
	else//��ʮ����
	{
		if(prt->State==1)
		{
			switch(prt->Direction)
			{
				case Straight : 
					if(InflectionPointL.Row>=110||InflectionPointR.Row>=110||(InflectionPointL.Flag==0&&InflectionPointR.Flag==0)) 
						prt->State=2;
					break;
				case Leftside : 
					if((InflectionPointL.Col<=20&&InflectionPointL.Row>=110)||(InflectionPointL.Flag==0))  
						prt->State=2;	
					break;
				case Rightside: 
					if((InflectionPointR.Col>=120&&InflectionPointR.Row>=110)||(InflectionPointR.Flag==0)) 
						prt->State=2;	
					break;
			}
		}
		else if(prt->State==2)
		{
			if((RightLose<20||LeftLose<20)&&AllLoseClose<32&&AllLose<10&&(RightEdge[RightStart]>135||LeftEdge[LeftStart]<5))
			{
				prt->State=0;
				prt->Flag=0;
				prt->Direction=3;
			}
		}
	}

	if(prt->Flag)
	{
		switch(prt->Direction)
		{
			case Straight: led(LED1,LED_ON); led(LED2,LED_ON); break;
			case Leftside: led(LED0,LED_ON);break;
			case Rightside:led(LED3,LED_ON);break;
		}
		//Beep_ON;
	}
	if(!prt->Flag&&!Roundabout.Flag)
	{
		led(LED_MAX,LED_OFF);
	}
		
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ж�ʮ�ֺ���
//  @return     void   
//  @since      v1.0
//  Sample usage: ����   &Crossroad      
//-------------------------------------------------------------------------------------------------------------------
void ModifyEdge_Crossroad(crossroad *prt)
{
	uint8 i,j,jj;
	if(RightStart<=DropRow&&LeftStart<=DropRow&&prt->State==2)
	{
		if(RightStart>LeftStart)
		{
			DropRow=RightStart-5;
			for(i=RightStart;i>DropRow;i--)
			{
				if(i==RightStart)
				{
					for(j=130;j<ColumnMax-2;j++)
					{
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							Right_Quantity++;
							RightEdge[i]=j;                                        //��¼��ʱ��������
							break;   
						}
					}
				}
				else
				{
					j = ((RightEdge[i+1]-CLOSE) <= 1)? 1:(RightEdge[i+1]-CLOSE);  
					jj = ((RightEdge[i+1]+FAR) >= ColumnMax-2)? ColumnMax-2:(RightEdge[i+1]+FAR);   
					while(j<=jj)
					{
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							Right_Quantity++;
							RightEdge[i]=j;                                        //��¼��ʱ��������
							break;   
						}
						j++;
					}
				}
			}
		}
		else if(RightStart<LeftStart)
		{
			DropRow=LeftStart-5;
			for(i=LeftStart;i>DropRow;i--)
			{
				if(i==LeftStart)
				{
					for(j=10;j>0;j--)
					{
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							
							Left_Quantity++;
							LeftEdge[i]=j;                                        //��¼��ʱ��������
							break;                                      //����ѭ��
						}
					}
				}
				else
				{
					j = ((LeftEdge[i+1]+CLOSE) >= ColumnMax-2)? ColumnMax-2:(LeftEdge[i+1]+CLOSE);  
					jj = ((LeftEdge[i+1]-FAR) <= 1)? 1:(LeftEdge[i+1]-FAR);  
					while(j>=jj)
					{
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							Left_Quantity++;
							LeftEdge[i]=j;                                        //��¼��ʱ��������
							break;                                      //����ѭ��
						}
						j--;
					}
				}
			}
		}
	}
	else if(prt->State==2)
	{
		if(RightStart<100)
		{
			for(i=RightStart;i>DropRow;i--)
			{
				if(ABS(RightEdge[i+1]-RightEdge[i]<4)&&RightEdge[i]<136)
				{
					RightStart=i;
					break;
				}
			}
		}
		if(LeftStart<100)
		{
			for(i=LeftStart;i>DropRow;i--)
			{
				if(ABS(LeftEdge[i]-LeftEdge[i+1]<4)&&LeftEdge[i]>4)
				{
					LeftStart=i;
					break;
				}
			}
		}
	}
}
