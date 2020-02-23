#include "Tracktype.h"

InflectionPoint  InflectionPointL;//������߹յ�ṹ�����
InflectionPoint  InflectionPointR;//�����ұ߹յ�ṹ�����
//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ѱ���ҹյ�
//  @param      �ҹյ�ṹ�� ��ʼ�� ��ֹ��
//  @return     void					
//  @since      v1.0
//  Sample usage:    FindInflectionPointR(&InflectionPointR ,90,60);
//-------------------------------------------------------------------------------------------------------------------
void FindInflectionPointR(InflectionPoint *inf ,unsigned char Start ,unsigned char End )
{
	int i;
	inf->Row=0;
	inf->Col=0;
	inf->Flag=0;
	inf->Number=0;
	for(i=Start;i>End;i--)
	{
		if(inf->Flag==0)
		{
			if(RightEdge[i]>0&&RightEdge[i-1]>0&&RightEdge[i+1]>0&&RightEdge[i+2]>0&&
			   RightEdge[i]<ColumnMax-1&&RightEdge[i-1]<ColumnMax-1&&RightEdge[i+1]<ColumnMax-1&&RightEdge[i+2]<ColumnMax-1)
				/*  &&(RightEdge[i]>10&&RightEdge[i-1]>10&&RightEdge[i+1]>10&&RightEdge[i-2]>10)*///RightEdge[i-2]>0&&&&RightEdge[i-2]<ColumnMax-1
			{//���в�����
				//�������ҹ�
				if(RightEdge[i]-RightEdge[i+1]<=0&&RightEdge[i-1]-RightEdge[i]>0&&(RightEdge[i-2]-RightEdge[i]>0||RightEdge[i-2]==ColumnMax))
				{//��һ��ƫ�� ����һ�в�ƫ�� �������в�����
					//�ҵ��ұ����йյ�
					inf->Row=i;//��¼�յ����
					inf->Col=RightEdge[i];//��¼�յ����
					inf->Flag=1;//����ҵ��ҹյ�
					inf->Number=1;
					i=i-5;
				}
			}
			else if(i<120&&RightEdge[i+1]!=ColumnMax&&RightEdge[i]!=ColumnMax
					&&RightEdge[i-1]==ColumnMax&&RightEdge[i-2]==ColumnMax&&RightEdge[i]<133&&RightEdge[i]-RightEdge[i+1]<=0)
				//&&RightEdge[i+2]!=ColumnMax
			{	//���������� && ��Ե������ͼ���Ե
				//�ҵ��ұ����йյ�
				inf->Row=i;//��¼�յ����
				inf->Col=RightEdge[i];//��¼�յ����
				inf->Flag=1;//����ҵ��ҹյ�
				inf->Number=1;
				i=i-5;
			}
			else if(RightEdge[i]>0&&RightEdge[i+1]>0&&RightEdge[i+2]>0
					&&RightEdge[i]<ColumnMax-1&&RightEdge[i+1]<ColumnMax-1&&RightEdge[i+2]<ColumnMax-1
						&&RightEdge[i-1]==ColumnMax&&RightEdge[i-2]>0&&RightEdge[i-2]<ColumnMax-1)
			{
				if(RightEdge[i]-RightEdge[i+1]<=0&&RightEdge[i]-RightEdge[i-2]<0)
				{//��һ�ж��� ������С ����һ�д�
					inf->Row=i;//��¼�յ����
					inf->Col=RightEdge[i];//��¼�յ����
					inf->Flag=1;//����ҵ��ҹյ�
					inf->Number=1;
					i=i-5;
				}
			}
		}
		
		else if(inf->Flag)
		{
			if(RightEdge[i]>0&&RightEdge[i-1]>0&&RightEdge[i+1]>0&&RightEdge[i+2]>0&&
			   RightEdge[i]<ColumnMax-1&&RightEdge[i-1]<ColumnMax-1&&RightEdge[i+1]<ColumnMax-1&&RightEdge[i+2]<ColumnMax-1)
				/*  &&(RightEdge[i]>10&&RightEdge[i-1]>10&&RightEdge[i+1]>10&&RightEdge[i-2]>10)*///RightEdge[i-2]>0&&&&RightEdge[i-2]<ColumnMax-1
			{//���в�����
				//�������ҹ�
				if(RightEdge[i]-RightEdge[i+1]<=0&&RightEdge[i-1]-RightEdge[i]>0&&(RightEdge[i-2]-RightEdge[i]>0||RightEdge[i-2]==ColumnMax))
				{//��һ��ƫ�� ����һ�в�ƫ�� �������в�����
					inf->Number++;
					i=i-5;
				}
			}
			else if(i<120&&RightEdge[i+1]!=ColumnMax&&RightEdge[i]!=ColumnMax
					&&RightEdge[i-1]==ColumnMax&&RightEdge[i-2]==ColumnMax&&RightEdge[i]<133&&RightEdge[i]-RightEdge[i+1]<=0)
				//&&RightEdge[i+2]!=ColumnMax
			{	//���������� && ��Ե������ͼ���Ե
				inf->Number++;
				i=i-5;
			}
			else if(RightEdge[i]>0&&RightEdge[i+1]>0&&RightEdge[i+2]>0
					&&RightEdge[i]<ColumnMax-1&&RightEdge[i+1]<ColumnMax-1&&RightEdge[i+2]<ColumnMax-1
						&&RightEdge[i-1]==ColumnMax&&RightEdge[i-2]>0&&RightEdge[i-2]<ColumnMax-1)
			{
				if(RightEdge[i]-RightEdge[i+1]<=0&&RightEdge[i]-RightEdge[i-2]<0)
				{//��һ�ж��� ������С ����һ�д�
					
					inf->Number++;
					i=i-5;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ѱ����յ�
//  @param      ��յ�ṹ�� ��ʼ�� ��ֹ��
//  @return     void					
//  @since      v1.0
//  Sample usage:    FindInflectionPointL(&InflectionPointL ,90,60);
//-------------------------------------------------------------------------------------------------------------------
void FindInflectionPointL(InflectionPoint *inf ,unsigned char Start ,unsigned char End )
{
	int i;
	inf->Row=0;
	inf->Col=0;
	inf->Flag=0;
	inf->Number=0;
	for(i=Start;i>End;i--)
	{
		if(inf->Flag==0)
		{
			if(LeftEdge[i]>0&&LeftEdge[i-1]>0&&LeftEdge[i+1]>0&&LeftEdge[i+2]>0
			   &&LeftEdge[i]<ColumnMax-1&&LeftEdge[i-1]<ColumnMax-1&&LeftEdge[i+1]<ColumnMax-1&&LeftEdge[i+2]<ColumnMax-1)
			  //&&LeftEdge[i-2]>0&&LeftEdge[i-2]<ColumnMax-1
			{//���в�����
				if(LeftEdge[i]-LeftEdge[i+1]>=0&&LeftEdge[i-1]-LeftEdge[i]<0&&//&LeftEdge[i]>
				   (LeftEdge[i-2]-LeftEdge[i]<0||LeftEdge[i-2]==0))//�ҵ��յ�//&&(LeftEdge[i-2]-LeftEdge[i-1]<0)&&LeftEdge[i-2]<ColumnMax&&LeftEdge[i-2]>0)
				{ //��һ��ƫ�� ����һ�в�ƫ�� �������в�����
					inf->Row=i;//��¼�ùյ����
					inf->Col=LeftEdge[i];//��¼�ùյ����
					inf->Flag=1;//����ҵ���յ�
					inf->Number=1;
					i=i-4;
				}
			}
			else if(i<125&&LeftEdge[i+2]!=0&&LeftEdge[i+1]!=0&&LeftEdge[i]!=0
					&&LeftEdge[i-1]==0&&LeftEdge[i-2]==0&&LeftEdge[i]>7&&LeftEdge[i]-LeftEdge[i+1]>=0)//<LeftEdge[i+1]
			{ //���������� && ��Ե������ͼ���Ե
				//�ҵ�������йյ�
				inf->Row=i;//��¼�ùյ����
				inf->Col=LeftEdge[i];//��¼�ùյ����
				inf->Flag=1;//����ҵ���յ�
				inf->Number=1;
				i=i-4;
			}
			
			else if(LeftEdge[i]>0&&LeftEdge[i+1]>0&&LeftEdge[i+2]>0
					&&LeftEdge[i]<ColumnMax-1&&LeftEdge[i+1]<ColumnMax-1&&LeftEdge[i+2]<ColumnMax-1
						&&LeftEdge[i-1]==0&&LeftEdge[i-2]>0&&LeftEdge[i-2]<ColumnMax-1)
			{ //
				if(LeftEdge[i]-LeftEdge[i+1]>=0&&LeftEdge[i]-LeftEdge[i-2]>=0)
				{//��һ�ж��� ������С ����һ�д�
					inf->Row=i;//��¼�ùյ����
					inf->Col=LeftEdge[i];//��¼�ùյ����
					inf->Flag=1;//����ҵ���յ�
					inf->Number=1;
					i=i-4;
				}
			}
		}
		
		else if(inf->Flag)
		{
			if(LeftEdge[i]>0&&LeftEdge[i-1]>0&&LeftEdge[i+1]>0&&LeftEdge[i+2]>0
			   &&LeftEdge[i]<ColumnMax-1&&LeftEdge[i-1]<ColumnMax-1&&LeftEdge[i+1]<ColumnMax-1&&LeftEdge[i+2]<ColumnMax-1)
				//&&LeftEdge[i-2]>0&&LeftEdge[i-2]<ColumnMax-1
			{//���в�����
				if(LeftEdge[i]-LeftEdge[i+1]>=0&&LeftEdge[i-1]-LeftEdge[i]<0&&//&LeftEdge[i]>
				   (LeftEdge[i-2]-LeftEdge[i]<0||LeftEdge[i-2]==0))//�ҵ��յ�//&&(LeftEdge[i-2]-LeftEdge[i-1]<0)&&LeftEdge[i-2]<ColumnMax&&LeftEdge[i-2]>0)
				{
					inf->Number++;
					i=i-4;
				}
			}
			else if(i<125&&LeftEdge[i+2]!=0&&LeftEdge[i+1]!=0&&LeftEdge[i]!=0
					&&LeftEdge[i-1]==0&&LeftEdge[i-2]==0&&LeftEdge[i]>7&&LeftEdge[i]-LeftEdge[i+1]>=0)//<LeftEdge[i+1]
			{ 
				inf->Number++;
				i=i-4;
			}
			else if(LeftEdge[i]>0&&LeftEdge[i+1]>0&&LeftEdge[i+2]>0
					&&LeftEdge[i]<ColumnMax-1&&LeftEdge[i+1]<ColumnMax-1&&LeftEdge[i+2]<ColumnMax-1
						&&LeftEdge[i-1]==0&&LeftEdge[i-2]>0&&LeftEdge[i-2]<ColumnMax-1)
			{ //
				if(LeftEdge[i]-LeftEdge[i+1]>=0&&LeftEdge[i]-LeftEdge[i-2]>=0)
				{//��һ�ж��� ������С ����һ�д�
					inf->Number++;
					i=i-4;
				}
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief              ��б��
//  @param              x����������  y����������
//  @return             double					
//  @Manufacturer       �½���
//  Sample usage:       Slope=get_line_slope(x,y,20)x[n],y[n]
//-------------------------------------------------------------------------------------------------------------------
double Get_Line_Slope(int x[], int y[], int n)
{
	
	int i;                           //б��
	double numerator=0.0, denominator=0.0;  //���ӷ�ĸ
	double sum_x=0.0,sum_y=0.0;             //x��y�ĺ�
	double sum_xx=0.0;                      //x��ƽ����
	double sum_xy=0.0;                      //xy���
	double average_x=0.0,average_y=0.0;     //x��y��ƽ����
	
	for(i=0;i<n;i++)
	{
		sum_xx+=x[i] * x[i]; //x��ƽ����
		sum_x+=x[i];         //x���
		sum_y+=y[i];         //y���
		sum_xy+=x[i]*y[i];   //x*y���
	} 
	average_x=sum_x/n;
	average_y=sum_y/n;
	
	for(i=0;i<n;i++)
	{
		numerator+=(y[i]-average_y) * (x[i]-average_x);
		denominator+=(x[i]-average_x) * (x[i]-average_x);
	}
	
	if(denominator==0)
		return 0;
	else
		return (numerator/denominator);  //ָ��б��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief              �����������
//  @param              0�����б�ʣ�1�ұ���б�ʣ�2����б�ʣ���ʼ�У���ֹ��
//  @return             double					
//  @Manufacturer       �½���
//  Sample usage:       Slope=EdgeSlope(0,100,75)
//-------------------------------------------------------------------------------------------------------------------
double EdgeSlope(uint8 LineMode,int16 ScanStart,int16 ScanEnd)
{

	int32 i,j;
	double Slope=0;
	int SlopeX[100];//б��X������
	int SlopeY[100];//б��Y������
	if(LineMode==0)
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			if(LeftEdge[i]!=0)
			{
				SlopeX[j]=LeftEdge[i];
			}
			else
			{
				SlopeX[j]=LeftEdge[i+1];
			}
			SlopeY[j]=i;
		}
		Slope=Get_Line_Slope(SlopeX,SlopeY,ScanStart-ScanEnd);
	}
	else if(LineMode==1)
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			if(RightEdge[i]!=140)
			{
				SlopeX[j]=RightEdge[i];
			}
			else
			{
				SlopeX[j]=RightEdge[i+1];
			}
			SlopeY[j]=i;
		}
		Slope=Get_Line_Slope(SlopeX,SlopeY,ScanStart-ScanEnd);
	}
	else
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			SlopeX[j]=MiddleLine[i];
			SlopeY[j]=i;
		}
		Slope=Get_Line_Slope(SlopeX,SlopeY,ScanStart-ScanEnd);
	}
	return Slope;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief              ��б��/�ؾ�
//  @param              x����������  y����������
//  @return             double					
//  @Manufacturer       �½���
//  Sample usage:       Slope=get_line_slope(x,y,20)x[n],y[n]
//-------------------------------------------------------------------------------------------------------------------
void get_slope_intercept(int x[],int y[],int n, double *slope_line, double *intercept)
{
	long int i;
	double numerator=0.0, denominator=0.0, sum_x=0.0,sum_y=0.0;
	double sum_xx=0.0; //x��ƽ����
	double sum_xy=0.0; // xy���
	double average_x=0.0,average_y=0.0;
	for(i=0;i<n;i++)
	{
		sum_xx+=x[i] * x[i]; //x��ƽ����
		sum_x+=x[i];         //x���
		sum_y+=y[i];         //y���
		sum_xy+=x[i]*y[i];   //x*y���
	} 
	average_x=sum_x/n;
	average_y=sum_y/n;
	for(i=0;i<n;i++)
	{
		numerator+=(y[i]-average_y) * (x[i]-average_x);
		denominator+=(x[i]-average_x) * (x[i]-average_x);
	}
	
	if(denominator==0)
		*slope_line=0;
	else
		*slope_line=numerator/denominator;  //ָ��б��
	
	* intercept=(sum_y-(*slope_line) * sum_x)/n; //ָ��ؾ�
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief              ��б��/�ؾ�
//  @param              x����������  y����������
//  @return             double					
//  @Manufacturer       �½���
//  Sample usage:       EdgeSlopeIntercept(right,start,end,&б��,&�ؾ�)
//-------------------------------------------------------------------------------------------------------------------
void EdgeSlopeIntercept(uint8 LineMode,int16 ScanStart,int16 ScanEnd,double *Slope,double *Intercept)
{
	int32 i,j;
	int SlopeX[100];//б��X������
	int SlopeY[100];//б��Y������
	if(LineMode==0)
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			if(LeftEdge[i]!=0)
			{
				SlopeX[j]=LeftEdge[i];
			}
			else
			{
				SlopeX[j]=LeftEdge[i+1];
			}
			SlopeY[j]=i;
		}
		get_slope_intercept(SlopeX,SlopeY,ScanStart-ScanEnd,Slope,Intercept);//Slope=Get_Line_Slope();
	}
	else if(LineMode==1)
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			if(RightEdge[i]!=140)
			{
				SlopeX[j]=RightEdge[i];
			}
			else
			{
				SlopeX[j]=RightEdge[i+1];
			}
			SlopeY[j]=i;
		}
		get_slope_intercept(SlopeX,SlopeY,ScanStart-ScanEnd,Slope,Intercept);//Get_Line_Slope2(SlopeX,SlopeY,ScanStart-ScanEnd,&Slope,&Intercept);
	}
	else
	{
		for(i=ScanStart,j=0; j<ScanStart-ScanEnd; i--,j++)
		{
			SlopeX[j]=MiddleLine[i];
			SlopeY[j]=i;
		}
		get_slope_intercept(SlopeX,SlopeY,ScanStart-ScanEnd,Slope,Intercept); //Get_Line_Slope2(SlopeX,SlopeY,ScanStart-ScanEnd,&Slope,&Intercept);
	}
	
}

