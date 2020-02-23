#include "Find_Track.h"

//��������
int16   MiddleLine[RowMax+1];   //��������
uint16   RightEdge[RowMax+1];    //�ұ�������
uint16   LeftEdge[RowMax+1];     //���������
uint16 	 MidPri    = 70;
//���߱�־
uint16   LeftLose;                 //��������
uint16   RightLose;                //�Ҷ�������
uint16   AllLose;                  //ȫ��������
uint16   AllLoseClose;             //����ȫ��������
//������ʼ�� �յ���
uint16   LeftStart;                //���һ��������
uint16   RightStart;               //�ҵ�һ��������
//uint16	 LeftInfRow,LeftInfCol;		//��߶��ߵ�
//uint16	 RightInfRow,RightInfCol;	//�ұ߶��ߵ�
uint16 	 LeftEndRow,RightEndRow;
//��������
uint16	Right_Quantity;
uint16	Left_Quantity;
//uint16	Last_Start;

uint8   Lose_Line;
//��ֹ��
uint16 DropRow;
uint16 LastDropRow;
uint16 DropRow_Straight;
//·��
uint8  Width[RowMax+1]=
{
	0,0,0,0,0,0,0,0,0,0,/*0-9*/0,0,0,0,0,0,0,0,0,0,/*10-19*/
	0,0,0,0,0,0,0,0,0,0,/*20-29*/0,0,0,0,0,0,0,0,0,0,/*30-39*/
	0,0,0,0,0,0,0,0,0,0,/*40-49*/12,28,30,32,34,36,38,40,42,44,/*50-59*/
	46,52,54,56,58,60,60,62,64,66,/*60-69*/
	68,70,72,74,76,78,80,82,84,86,/*70-79*/
	88,90,90,90,90,92,94,94,94,94,/*80-99*/
	95,95,96,96,98,100,102,104,106,108,/*90-99*/
	110,112,114,116,118,120,122,124,126,128,/*100-109*/
	130,132,134,136,138,140,142,142,142,146,/*110-119*/
	152,154,156,158,160,162,164,166,168,170,/*120-129*/
	172,174,176,178,180,182,184,186,188,190/*130-139*/
};
uint8  Width_Straight[RowMax+1]=
{
	0,0,0,0,0,0,0,0,0,0,/*0-9*/0,0,0,0,0,0,0,0,0,0,/*10-19*/
	0,0,0,0,0,0,0,0,0,0,/*20-29*/0,0,0,0,0,0,0,0,0,0,/*30-39*/
	0,0,0,0,0,0,0,0,0,0,/*40-49*/0,2,4,6,8,10,12,14,16,18,/*50-59*/
	20,22,24,25,27,30,33,35,37,39,/*60-69*/
	41,43,45,47,49,51,53,55,57,60,/*70-79*/
	62,64,66,68,70,72,74,76,78,80,/*80-99*/
	83,85,87,89,91,93,95,97,99,101,/*90-99*/
	104,106,108,110,112,114,116,118,120,121,/*100-109*/
	123,125,127,129,131,133,135,137,139,141,/*110-119*/
	144,146,148,150,152,154,156,158,160,162,/*120-129*/
	165,167,169,171,173,175,177,179,181,183/*130-139*/
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ���߳�ʼ������
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		��ʼ���߽�����  ��ͼƬ����һ��Ϳ��
//-------------------------------------------------------------------------------------------------------------------
void Edge_Init() 
{
	//��¼���߱���
	LeftLose		= 0;
	RightLose		= 0; 
	AllLose			= 0;
	AllLoseClose	= 0;
	//��ʼ
	LeftStart     	= 0;
	RightStart    	= 0;
	
	LeftEndRow		= 0;
	RightEndRow		= 0;
	//��������
	Right_Quantity	= 0;
	Left_Quantity	= 0;
	//��ֹ�г�ʼ�� ������40��
	DropRow			= 40;
	LastDropRow=40;
	for(int i=0;i<=140;i++)
	{
		RightEdge[i]  = ColumnMax;
		LeftEdge[i]   = 0;
		MiddleLine[i] = ColumnMax/2;
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      Ѱ�ұ߽纯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		ǰ5��ȫ��ɨ��  ����ֱ��ѯ��ʼ�� ���ر߸���   
//-------------------------------------------------------------------------------------------------------------------
void Search_CenterBlackline()
{
	Edge_Init();
	
	int16 Middle = MidPri;
	int i,j,jj;//i->��  j,jj->��
	if(Roundabout.State==2||Roundabout.State==1)//
	{
		for(i=RowMax-2;i>RowMax-8;i--)
		{
			for(j=Middle;j>1;j--)
			{
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Left_Quantity++;
					LeftEdge[i]=j;                                        //��¼��ʱ��������
					break;                                      //����ѭ��
				}
			}
			//Ѱ���ұ�
			for(j=Middle;j<ColumnMax-2;j++)
			{	
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Right_Quantity++;
					RightEdge[i]=j;                                        //��¼��ʱ��������
					break;   
				}
			}
			if(LeftStart==0&&LeftEdge[i+1]!=0&&LeftEdge[i+2]!=0)  
			{
				LeftStart  = i+2;
			}
			//�������в�����
			if(RightStart==0&&RightEdge[i+1]!=ColumnMax&&RightEdge[i+2]!=ColumnMax)
			{
				RightStart  = i+2;
			}
			/***************************************/
			if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)                           //��߅���]�ЁG��
			{
				Middle= (LeftEdge[i]+RightEdge[i])/2;                       //���ߵ������ұ߽��/2   
			}
			else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)                      //����߶���
			{ 
				if(LeftStart||RightStart)                                     //��¼������
				{
					LeftLose++;
				}
				if(Width[i]<=140)//����=�ұ߽�-·��
					Middle=RightEdge[i]-Width[i]/2;    
				else
					Middle=RightEdge[i]-70;
			}
			else if(LeftEdge[i]!=0 &&RightEdge[i]==ColumnMax)                       //���G���Ҿ�   
			{
				if(LeftStart||RightStart)                                     //��¼������
				{
					RightLose++;
				}
				if(Width[i]<=140)
					Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
				else
					Middle=LeftEdge[i]+70;
			}
			else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)                      //��߅�G��  
			{         
				if(LeftStart||RightStart)                                     //��¼����
				{
					AllLose++;
				}              
				AllLoseClose++;                   //ʮ��ǰ��ȫ����¼
			}
			Middle = Middle>ColumnMax-2?ColumnMax-2:Middle;
			Middle = Middle<3?3:Middle;
			if(i==RowMax-5)
			{
				if(ABS(Middle-MidPri)<60)
					MidPri=Middle;
			}
		}
		if(RightStart==0)
		{
			for(i=135;i>50;i--)
			{
				if(Solve_Prewitt(i,ColumnMax-4))   
				{
					RightStart=i;
					break;
				}
			}
		}
		if(LeftStart==0)
		{
			for(i=135;i>50;i--)
			{
				if(Solve_Prewitt(i,3))   
				{
					LeftStart=i;
					break;
				}
			}
		}
		for(i=RowMax-8;i>DropRow-10;i--)
		{
			if(i==DropRow-9)
			{
				if(Roundabout.Direction==Leftside&&RightEndRow==0)
					RightEndRow=i+4;
				if(Roundabout.Direction==Rightside&&LeftEndRow==0)
					LeftEndRow=i+4;
			}
			if(LeftEndRow&&RightEndRow)
			{
				break;
			}
			if(i<=RightStart&&i<=LeftStart)
			{
				if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=ColumnMax)       //��һ�Ѓ�߅���]�ЁG�� 
				{	//��Ѱ�����
					if(LeftEndRow==0)
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
					if(RightEndRow==0)
					{
						//Ѱ���ұ�
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
				else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=ColumnMax)	//�����
				{
					//Ѱ���ұ�
					if(RightEndRow==0)
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
					if(LeftEndRow==0)
					{
						for(j=Middle;j>1;j--)
						{
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
								//									break;
								Left_Quantity++;
								LeftEdge[i]=j;                                        //��¼��ʱ��������
								break;                                      //����ѭ��
							}
						}
					}
				}
				else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==ColumnMax)	//���ұ�
				{
					//��Ѱ�����
					if(LeftEndRow==0)
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
					if(RightEndRow==0)
					{
						for(j=Middle;j<ColumnMax-2;j++)
						{	
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
								//									break;
								Right_Quantity++;
								RightEdge[i]=j;                                        //��¼��ʱ��������
								break;   
							}
						}
					}
				}
				else if(LeftEdge[i+1]==0 && RightEdge[i+1]==ColumnMax)
				{
					if(RightEndRow==0)
					{
						for(j=Middle;j<ColumnMax-2;j++)
						{	
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
								//									break;
								Right_Quantity++;
								RightEdge[i]=j;                                        //��¼��ʱ��������
								break;   
							}
						}
					}
					if(LeftEndRow==0)
					{
						for(j=Middle;j>1;j--)
						{
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
								//									break;
								Left_Quantity++;
								LeftEdge[i]=j;                                        //��¼��ʱ��������
								break;                                      //����ѭ��
							}
						}
					}
				}
			}
			else if(i<=RightStart&&i>LeftStart&&RightEndRow==0)
			{
				if(RightEdge[i+1]!=ColumnMax)
				{
					//Ѱ���ұ�
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
				else
				{
					for(j=Middle;j<ColumnMax-2;j++)
					{	
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							//							if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
							//								break;
							Right_Quantity++;
							RightEdge[i]=j;                                        //��¼��ʱ��������
							break;   
						}
					}
				}
			}
			else if(i>RightStart&&i<=LeftStart&&LeftEndRow==0)
			{
				if(LeftEdge[i+1]!=0)
				{
					//��Ѱ�����
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
				else
				{
					for(j=Middle;j>1;j--)
					{
						if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
						{
							//							if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
							//								break;
							Left_Quantity++;
							LeftEdge[i]=j;                                        //��¼��ʱ��������
							break;                                      //����ѭ��
						}
					}
				}
			}
			
			if(i<=RightStart||i<=LeftStart)
			{	//��������ͻ��
				
				if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)   //�]�ЁG������r��
				{
					Middle=(LeftEdge[i] + RightEdge[i])/2;
				} 
				//�G���Ҿ�
				else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)   
				{    
					RightLose++;
					if(Width[i]<=140)
						Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
					else
						Middle=LeftEdge[i]+70;
				}
				//�G����
				else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)     
				{      
					LeftLose++;
					if(Width[i]<=140)//����=�ұ߽�-·��
						Middle=RightEdge[i]-Width[i]/2;    
					else
						Middle=RightEdge[i]-70;
				}
				//��߅�G��
				else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)  
				{         
					AllLose++;
					AllLoseClose++;
				}
				
			}
			else
			{
				AllLoseClose++;
			}
			
			Middle = Middle>ColumnMax-2?ColumnMax-2:Middle;
			Middle = Middle<2?2:Middle;
			if((LeftEdge[i]>=110||RightEdge[i]<=30||RightEdge[i]-LeftEdge[i]<25||Solve_Prewitt(i-3,Middle))
			   &&DropRow==40&&i<MAX(RightStart,LeftStart))
			{
				DropRow=i;
			}
			
			if(Roundabout.Direction==Rightside)
			{
				if((((RightEdge[i]==ColumnMax&&RightEdge[i+1]!=ColumnMax)&&i<RightStart-2)||RightStart==0||i<63)&&RightEndRow==0)
				{
					RightEndRow=i+2;
				}
				if((Solve_Prewitt(i,LeftEdge[i]+6)||Solve_Prewitt(i-1,LeftEdge[i]+6)||Solve_Prewitt(i-2,LeftEdge[i]+8)||(LeftEdge[i]==0&&LeftEdge[i+1]==0))&&i<=LeftStart-5&&LeftEndRow==0)
				{
					LeftEndRow=i;
				}
				
			}
			else if(Roundabout.Direction==Leftside)
			{
				//if((LeftEdge[i]>=5&&LeftEdge[i+1]<5&&i<LeftStart-5)||(LeftEdge[i]-LeftEdge[i+1]>10))
				if((((LeftEdge[i]==0&&LeftEdge[i+1]!=0)&&i<LeftStart-2)||LeftStart==0||i<63)&&LeftEndRow==0)
				{
					LeftEndRow=i+2;
				}
				if((Solve_Prewitt(i,RightEdge[i]-6)||Solve_Prewitt(i-2,RightEdge[i]-6)||Solve_Prewitt(i-1,RightEdge[i]-8)||(RightEdge[i]==ColumnMax&&RightEdge[i+1]==ColumnMax))&&i<=RightStart-5&&RightEndRow==0)
				{
					RightEndRow=i;
				}
			}
		}
	}
	else 
	{
		for(i=RowMax-2;i>RowMax-8;i--)
		{	//Ѱ�����
			for(j=Middle;j>1;j--)
			{
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Left_Quantity++;
					LeftEdge[i]=j;                                        //��¼��ʱ��������
					break;                                      //����ѭ��
				}
			}
			//Ѱ���ұ�
			for(j=Middle;j<ColumnMax-2;j++)
			{	
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Right_Quantity++;
					RightEdge[i]=j;                                        //��¼��ʱ��������
					break;   
				}
			}
			/***************************************/
			if(LeftStart==0&&LeftEdge[i+1]!=0&&LeftEdge[i+2]!=0)  
			{
				LeftStart  = i+2;
			}
			//�������в�����
			if(RightStart==0&&RightEdge[i+1]!=ColumnMax&&RightEdge[i+2]!=ColumnMax)
			{
				RightStart  = i+2;
			}
			/***************************************/
			if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)                           //��߅���]�ЁG��
			{
				Middle= (LeftEdge[i]+RightEdge[i])/2;                       //���ߵ������ұ߽��/2   
			}
			else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)                      //����߶���
			{ 
				if(LeftStart||RightStart)                                     //��¼������
				{
					LeftLose++;
				}
				if(Width[i]<=140)//����=�ұ߽�-·��
					Middle=RightEdge[i]-Width[i]/2;    
				else
					Middle=RightEdge[i]-70;
			}
			else if(LeftEdge[i]!=0 &&RightEdge[i]==ColumnMax)                       //���G���Ҿ�   
			{
				if(LeftStart||RightStart)                                     //��¼������
				{
					RightLose++;
				}
				if(Width[i]<=140)
					Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
				else
					Middle=LeftEdge[i]+70;
			}
			else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)                      //��߅�G��  
			{         
				if(LeftStart||RightStart)                                     //��¼����
				{
					AllLose++;
				}              
				AllLoseClose++;      
				Middle=70;
			}

			Middle = Middle>ColumnMax-2?ColumnMax-2:Middle;
			Middle = Middle<3?3:Middle;
			if(i==RowMax-5)
			{
				if(ABS(Middle-MidPri)<60)
					MidPri=Middle;
			}
		}
		
		{
			/***************************************/
			if(RightStart==0)
			{
				for(i=135;i>50;i--)
				{
					if(Solve_Prewitt(i,ColumnMax-4))   
					{
						RightStart=i;
						break;
					}
				}
			}
			if(LeftStart==0)
			{
				for(i=135;i>50;i--)
				{
					if(Solve_Prewitt(i,3))   
					{
						LeftStart=i;
						break;
					}
				}
			}
			for(i=RowMax-8;i>DropRow;i--)
			{
				if(i<=RightStart&&i<=LeftStart)
				{
					if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=ColumnMax)       //��һ�Ѓ�߅���]�ЁG�� 
					{	//��Ѱ�����
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
						
						//Ѱ���ұ�
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
					else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=ColumnMax)	//�����
					{
						//Ѱ���ұ�
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
						
						for(j=Middle;j>1;j--)
						{
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
								//									break;
								Left_Quantity++;
								LeftEdge[i]=j;                                        //��¼��ʱ��������
								break;                                      //����ѭ��
							}
						}
					}
					else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==ColumnMax)	//���ұ�
					{
						//��Ѱ�����
						j = ((LeftEdge[i+1]+CLOSE) >= ColumnMax-2)?ColumnMax-2:(LeftEdge[i+1]+CLOSE);  
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
						
						for(j=Middle;j<ColumnMax-2;j++)
						{	
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
								//									break;
								Right_Quantity++;
								RightEdge[i]=j;                                        //��¼��ʱ��������
								break;   
							}
						}
						
					}
					else if(LeftEdge[i+1]==0 && RightEdge[i+1]==ColumnMax)
					{
						for(j=Middle;j<ColumnMax-2;j++)
						{	
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
								//									break;
								Right_Quantity++;
								RightEdge[i]=j;                                        //��¼��ʱ��������
								break;   
							}
						}
						for(j=Middle;j>1;j--)
						{
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
								//									break;
								Left_Quantity++;
								LeftEdge[i]=j;                                        //��¼��ʱ��������
								break;                                      //����ѭ��
							}
						}
					}
				}
				else if(i<=RightStart&&i>LeftStart)
				{
					if(RightEdge[i+1]!=ColumnMax)
					{
						//Ѱ���ұ�
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
					else
					{
						for(j=Middle;j<ColumnMax-2;j++)
						{	
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
								//									break;
								Right_Quantity++;
								RightEdge[i]=j;                                        //��¼��ʱ��������
								break;   
							}
						}
					}
				}
				else if(i>RightStart&&i<=LeftStart)
				{
					if(LeftEdge[i+1]!=0)
					{
						//��Ѱ�����
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
					else
					{
						for(j=Middle;j>1;j--)
						{
							if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
							{
								//								if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
								//									break;
								Left_Quantity++;
								LeftEdge[i]=j;                                        //��¼��ʱ��������
								break;                                      //����ѭ��
							}
						}
					}
				}
				
				if(i<=RightStart||i<=LeftStart)
				{	//��������ͻ��
					
					if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)   //�]�ЁG������r��
					{
						Middle=(LeftEdge[i] + RightEdge[i])/2;
					} 
					//�G���Ҿ�
					else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)   
					{    
						RightLose++;
						if(Width[i]<=140)
							Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
						else
							Middle=LeftEdge[i]+70;
					}
					//�G����
					else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)     
					{      
						LeftLose++;
						if(Width[i]<=140)//����=�ұ߽�-·��
							Middle=RightEdge[i]-Width[i]/2;    
						else
							Middle=RightEdge[i]-70;
					}
					//��߅�G��
					else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)  
					{         
						AllLose++;
						AllLoseClose++;
					}
					//					if(LeftEdge[i+1]-LeftEdge[i]>3&&LeftInfRow==0&&Middle<110)
					//					{
					//						LeftInfCol=LeftEdge[i+1];                //���ж���
					//						LeftInfRow=i+1; 
					//					}
					//					if(RightEdge[i]-RightEdge[i+1]>3&&RightInfRow==0&&Middle>30)
					//					{
					//						RightInfCol=RightEdge[i+1];            //���ж���
					//						RightInfRow=i+1;      
					//					}
				}
				else
				{
					AllLoseClose++;
				}
				
				Middle = Middle>ColumnMax-2?ColumnMax-2:Middle;
				Middle = Middle<2?2:Middle;
				if((LeftEdge[i]>=105||RightEdge[i]<=35||RightEdge[i]-LeftEdge[i]<25||
					Solve_Prewitt(i-2,Middle))&&i<120)
				{
					DropRow=i;
					break;
				}
			}
			if(Crossroad.State!=2)
			{
				LeftStart=LeftStart-DropRow>5?LeftStart:0;
				RightStart=RightStart-DropRow>5?RightStart:0;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ᳵ��Ѱ�ұ߽纯��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		Ѱ�ҵ���
//-------------------------------------------------------------------------------------------------------------------
void Search_CenterBlackline_Meeting()
{
	
	Edge_Init();
	int16 Middle = MidPri;
	DropRow=77;
	
	int i,j,jj;
	int16 Left=1;//,Right=ColumnMax-2;
	if(Meeting_My.State>2&&Meeting_My.State<6)
	{
		for(i=RowMax-2;i>RowMax-15;i--)
		{	//Ѱ�����
			for(j=Middle;j>1;j--)
			{
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Left_Quantity++;
					LeftEdge[i]=j;                                        //��¼��ʱ��������
					break;                                      //����ѭ��
				}
			}
			//Ѱ���ұ�
			for(j=Middle;j<ColumnMax-2;j++)
			{	
				if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
				{
					Right_Quantity++;
					RightEdge[i]=j;                                        //��¼��ʱ��������
					break;   
				}
			}
			/***************************************/
			if(LeftStart==0&&LeftEdge[i+1]!=0&&LeftEdge[i+2]!=0)  
			{
				LeftStart  = i+2;
			}
			//�������в�����
			if(RightStart==0&&RightEdge[i+1]!=ColumnMax&&RightEdge[i+2]!=ColumnMax)
			{
				RightStart  = i+2;
			}
			/***************************************/
			if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)                           //��߅���]�ЁG��
			{
				Middle= (LeftEdge[i]+RightEdge[i])/2;                       //���ߵ������ұ߽��/2   
			}
			else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)                      //����߶���
			{ 
				if(LeftStart||RightStart)                                     //��¼������
				{
					LeftLose++;
				}
				if(Width[i]<=140)//����=�ұ߽�-·��
					Middle=RightEdge[i]-Width[i]/2;    
				else
					Middle=RightEdge[i]-70;
			}
			else if(LeftEdge[i]!=0 &&RightEdge[i]==ColumnMax)                       //���G���Ҿ�   
			{
				if(LeftStart||RightStart)                                     //��¼������
				{
					RightLose++;
				}
				if(Width[i]<=140)
					Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
				else
					Middle=LeftEdge[i]+70;
			}
			else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)                      //��߅�G��  
			{         
				if(LeftStart||RightStart)                                     //��¼����
				{
					AllLose++;
				}              
				AllLoseClose++;      
				Middle=70;//ʮ��ǰ��ȫ����¼
			}
			
			Middle = Middle>ColumnMax-5?ColumnMax-5:Middle;
			Middle = Middle<5?5:Middle;
			if(i==RowMax-5)
			{
				if(ABS(Middle-MidPri)<50)
					MidPri=Middle;
			}
		}
		for(i=RowMax-15;i>DropRow;i--)
		{
			if(LeftEdge[i+1]!=0 && RightEdge[i+1]!=ColumnMax)       //��һ�Ѓ�߅���]�ЁG�� 
			{	//��Ѱ�����
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
				
				//Ѱ���ұ�
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
			else if(LeftEdge[i+1]==0 && RightEdge[i+1]!=ColumnMax)	//�����
			{
				//Ѱ���ұ�
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
				
				for(j=Middle;j>1;j--)
				{
					if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
					{
						//						if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
						//							break;
						Left_Quantity++;
						LeftEdge[i]=j;                                        //��¼��ʱ��������
						break;                                      //����ѭ��
					}
				}
			}
			else if(LeftEdge[i+1]!=0 && RightEdge[i+1]==ColumnMax)	//���ұ�
			{
				//��Ѱ�����
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
				
				for(j=Middle;j<ColumnMax-2;j++)
				{	
					if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
					{
						//						if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
						//							break;
						Right_Quantity++;
						RightEdge[i]=j;                                        //��¼��ʱ��������
						break;   
					}
				}
				
			}
			else if(LeftEdge[i+1]==0 && RightEdge[i+1]==ColumnMax)
			{
				for(j=Middle;j<ColumnMax-2;j++)
				{	
					if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
					{
						//						if(RightInfRow!=0&&j>RightInfCol-TrackArea_Ratio*(RightInfRow-i))
						//							break;
						Right_Quantity++;
						RightEdge[i]=j;                                        //��¼��ʱ��������
						break;   
					}
				}
				for(j=Middle;j>1;j--)
				{
					if(Solve_Prewitt(i,j))                                    //���ĵ�Ϊ��Ե�����
					{
						//						if(LeftInfRow&&j<LeftInfCol+TrackArea_Ratio*(LeftInfRow-i))
						//							break;
						Left_Quantity++;
						LeftEdge[i]=j;                                        //��¼��ʱ��������
						break;                                      //����ѭ��
					}
				}
			}
			
			if(LeftStart==0&&LeftEdge[i+1]!=0&&LeftEdge[i+2]!=0)  
			{
				LeftStart  = i+2;
			}
			//�������в�����
			if(RightStart==0&&RightEdge[i+1]!=ColumnMax&&RightEdge[i+2]!=ColumnMax)
			{
				RightStart  = i+2;
			}
			if(LeftEdge[i]!=0 && RightEdge[i]!=ColumnMax)   //�]�ЁG������r��
			{
				Middle=(LeftEdge[i] + RightEdge[i])/2;
			} 
			//�G���Ҿ�
			else if(LeftEdge[i]!=0 && RightEdge[i]==ColumnMax)   
			{    
				RightLose++;
				if(Width[i]<=140)
					Middle=LeftEdge[i]+Width[i]/2;                               //����=��߽�+·��
				else
					Middle=LeftEdge[i]+70;
			}
			//�G����
			else if(LeftEdge[i]==0 && RightEdge[i]!=ColumnMax)     
			{      
				LeftLose++;
				if(Width[i]<=140)//����=�ұ߽�-·��
					Middle=RightEdge[i]-Width[i]/2;    
				else
					Middle=RightEdge[i]-70;
			}
			//��߅�G��
			else if(LeftEdge[i]==0 && RightEdge[i]==ColumnMax)  
			{         
				AllLose++;
				AllLoseClose++;
			}
			Middle = Middle>ColumnMax-2?ColumnMax-2:Middle;
			Middle = Middle<2?2:Middle;
			if(LeftEdge[i]>=110||RightEdge[i]<=30||RightEdge[i]-LeftEdge[i]<25||
			   Solve_Prewitt(i-3,Middle)&&i<120)
			{
				DropRow=i;
				break;
			}
		}

		if(Meeting_My.Electric==0)
		{
			Lose_Line=0;
			if(Meeting_Area.Direction==Leftside||Meeting_My.State==5)
			{
				int a=0;
				for(i=40;i<100;i++)
					for(j=125;j<135;j++)
					{
						if(image[j][i]<Track_White)
						{
							a++;
						}
						if(a>350)
						{
							Lose_Line=1;
							MidPri=125;
							if(Meeting_My.Track_Mode==1)
							{
								Meeting_My.Electric=1;
							}
							break;
						}
					}
			}
			else if(Meeting_Area.Direction==Rightside&&Meeting_My.State!=5)
			{
				int a=0;
				//a=0;
				for(i=40;i<100;i++)
					for(j=125;j<135;j++)
					{
						if(image[j][i]<Track_White)
						{
							a++;
						}
						if(a>350)
						{
							Lose_Line=1;
							MidPri=15;
							if(Meeting_My.Track_Mode==1)
							{
								Meeting_My.Electric=1;
							}
							break;
						}
					}
			}
		}
		if(Meeting_My.Electric)
		{
			if(Meeting_Area.Direction==Leftside||Meeting_My.State==5)
			{
				int a=0;
				for(i=40;i<100;i++)
					for(j=125;j<135;j++)
					{
						if(image[j][i]>Track_White)
						{
							a++;
						}
						if(a>400)
						{
							Lose_Line=0;
							MidPri=125;
							if(Meeting_My.Track_Mode==1)
							{
								Meeting_My.Electric=0;
							}
							break;
						}
					}
			}
			else if(Meeting_Area.Direction==Rightside&&Meeting_My.State!=5)
			{
				int a=0;
				//a=0;
				for(i=40;i<100;i++)
					for(j=125;j<135;j++)
					{
						if(image[j][i]>Track_White)
						{
							a++;
						}
						if(a>400)
						{
							Lose_Line=0;
							MidPri=15;
							if(Meeting_My.Track_Mode==1)
							{
								Meeting_My.Electric=0;
							}
							break;
						}
					}
			}
			
		}
	}
	
	else
	{
		for(i=RowMax-2;i>DropRow;i--)
		{
			if(LeftEdge[i+1]==0)
			{
				for(j=Middle;j>Left;j--)
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
				j = ((LeftEdge[i+1]+10) >= ColumnMax-2)? ColumnMax-2:(LeftEdge[i+1]+10);  
				jj = ((LeftEdge[i+1]-10) <= 1)? 1:(LeftEdge[i+1]-10);  
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
			/*��������*/
			{
				if(LeftEdge[i]!=0)
				{
					Middle=LeftEdge[i]+Width[i]/4>120?120:LeftEdge[i]+Width[i]/4;
					Left=LeftEdge[i]-10<1?1:LeftEdge[i]-10;
				}
				if(LeftStart==0&&LeftEdge[i+1]!=0&&LeftEdge[i+2]!=0)  
				{
					LeftStart  = i+2;
				}
				if(i==RowMax-5)
				{
					if(ABS(Middle-MidPri)<50)
						MidPri=Middle;
				}
				if((LeftEdge[i]>=110||Solve_Prewitt(i-3,Middle)||Middle-LeftEdge[i]<10)&&i<110)
				{
					DropRow=i;
					break;
				}
			}
		}
		int a=0;
		{
			Lose_Line=0;
			if(Meeting_My.State!=6&&Meeting_My.State!=1)
			{
				for(i=72;i<82;i++)
					for(j=125;j<135;j++)
					{
						if(image[j][i]<Track_White)
						{
							a++;
						}
						if(a>65)
						{
							Lose_Line=1;
							MidPri=120;
						}
					}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡ��ֹ��
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		�м���������ɨ
//-------------------------------------------------------------------------------------------------------------------
void Get_EndLine(uint8 middle)
{
	
	int i;
	uint8 MLEndFlag     =  1;
	uint8 MREndFlag     =  1;
	uint8 MEndFlag1     =  1;//��
	uint8 MEndFlag     =  1;//��
	uint8 BlackEnd[7]  = {0};
	uint8 BlackEndMax  =  0;
	DropRow_Straight            =  0;//����Д�
	
	int  mright;
	int  mleft;
	int  mid;
	int  ml=70 ;
	
	mid       =  middle;
	mright    =  middle+6>ColumnMax-2?ColumnMax-2:middle+6;
	mleft     =  middle-6<1?1:middle-6;
	//	uint8 LEndFlag      =  1;
	//	uint8 REndFlag      =  1;
	//	uint8 LLEndFlag     =  1;
	//	uint8 RREndFlag     =  1;
	//	
	//	uint8 lleft  = 20;
	//	uint8 left   = 25;
	//	uint8 right  = 120;
	//	uint8 rright = 115;
	
	
	for(i=RowMax-3;i>=50;i--)
	{   
		
		if(Solve_Prewitt(i,mleft)==0&&LeftEdge[i]<mleft&&RightEdge[i]>mleft&&MLEndFlag)//(image[i][rmid]>40&&MREndFlag)
			BlackEnd[0]++;
		else
			MLEndFlag=0;
		
		if(Solve_Prewitt(i,mid)==0&&LeftEdge[i]<mid&&RightEdge[i]>mid&&MEndFlag)
			BlackEnd[1]++;
		else
			MEndFlag=0;
		
		if(Solve_Prewitt(i,mright)==0&&LeftEdge[i]<mright&&RightEdge[i]>mright&&MREndFlag)//(image[i][rright]>40&&RREndFlag)
			BlackEnd[2]++;
		else
			MREndFlag=0;
		
		if(Solve_Prewitt(i,ml)==0&&LeftEdge[i]<ml&&RightEdge[i]>ml&&MEndFlag1)
			BlackEnd[3]++;
		else
			MEndFlag1=0;
		
		//		if(Solve_Prewitt(i,right)==0&&REndFlag)//(image[i][rright]>40&&RREndFlag)
		//			BlackEnd[3]++;
		//		else
		//			REndFlag=0;
		//		
		//		if(Solve_Prewitt(i,rright)==0&&RREndFlag)//(image[i][rright]>40&&RREndFlag)
		//			BlackEnd[4]++;
		//		else
		//			RREndFlag=0;
		//		
		//		if(Solve_Prewitt(i,left)==0&&LEndFlag)//(image[i][rright]>40&&RREndFlag)
		//			BlackEnd[5]++;
		//		else
		//			LEndFlag=0;
		//		
		//		if(Solve_Prewitt(i,lleft)==0&&LLEndFlag)//(image[i][rright]>40&&RREndFlag)
		//			BlackEnd[6]++;
		//		else
		//			LLEndFlag=0;
		
		
		if(MREndFlag==0&&MLEndFlag==0&&MEndFlag==0&&MEndFlag1==0)//&&LLEndFlag==0&&LEndFlag==0&&RREndFlag==0&&RREndFlag==0
			break;
		
	}
	
	
	
	BlackEndMax =MAX(BlackEnd[1],BlackEnd[0]);//ȡ��ֵ
	BlackEndMax =MAX(BlackEndMax,BlackEnd[2]);
	if(Right_Quantity>=30||Left_Quantity>=30||Roundabout.State==2)
		BlackEndMax=BlackEnd[3];
	
	//	DropRow_Right=140-(MAX(BlackEnd[3],BlackEnd[4]));
	//	DropRow_Left =140-(MAX(BlackEnd[5],BlackEnd[6]));
	
	DropRow_Straight=140-BlackEndMax;//�ⶥ������
	
	if(DropRow_Straight>125)
		DropRow_Straight=125;
	if(DropRow_Straight<=45)
		DropRow_Straight=45;
	
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������������
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		��ʼ���߽�����  ��ͼƬ����һ��Ϳ��
//-------------------------------------------------------------------------------------------------------------------
void  Calculation_Middleline()
{
	int i;
	if(Meeting_My.Flag&&Meeting_My.Find_Track||Meeting_My.State==0)//||Meeting_My.State==5)&&Meeting_My.Brake==0
	{
		if(Meeting_My.State==0)
		{
			for(i=RowMax-3;i>DropRow;i--)
			{
				if(LeftEdge[i]==0)
				{
					MiddleLine[i]=MiddleLine[i+1];
				}
				else
				{
					MiddleLine[i]=LeftEdge[i]<1?1:LeftEdge[i]+5;
				} 
			}
		}
		else if(Meeting_My.State==1||Meeting_My.State==6)
		{
			if(Meeting_My.State==6&&Meeting_My.End==1)
			{
				for(i=RowMax-2;i>DropRow;i--)
				{
					if(i<ZebraCrossing.StartRow+1&&i>ZebraCrossing.EndRow-1)//�յ����ڽ���ʱ
					{
						continue;
					}
					if(i==RowMax-2)
					{
						if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
						}
						if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
						}
						if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=MiddleLine[i+1];
						}
					}
					else
					{
						if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
						{
							if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
								MiddleLine[i]=MiddleLine[i+1];
							else
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
						}
						if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
						}
						if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=MiddleLine[i+1];
						}
					}
				}
			}
			else
			{
				for(i=RowMax-3;i>DropRow;i--)
				{
					if(i<ZebraCrossing.StartRow+1&&i>ZebraCrossing.EndRow-1)//�յ����ڽ���ʱ
					{
						continue;
					}
					if(LeftEdge[i]==0)
					{
						MiddleLine[i]=MiddleLine[i+1];
					}
					else
					{
						MiddleLine[i]=LeftEdge[i]<1?1:LeftEdge[i]+10;
					} 
				}
			}
			
		}
		else if(Meeting_My.State==3)
		{
			if(Meeting_My.Oder==1)
			{
				if(Meeting_Area.Direction==Rightside)
				{
					for(i=RowMax-2;i>DropRow;i--)
					{
						if(RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=RightEdge[i]>ColumnMax-1?ColumnMax-1:RightEdge[i]-5;
						}
						else 
						{
							if(LeftEdge[i]!=0)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]>135?135:LeftEdge[i]+Width[i]);
							}
							else
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
				else
				{
					for(i=RowMax-2;i>DropRow;i--)
					{
						if(LeftEdge[i]!=0)
						{
							MiddleLine[i]=LeftEdge[i]<1?1:LeftEdge[i]+5;
						}
						else 
						{
							if(RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]<5?5:RightEdge[i]-Width[i]);
							}
							else
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
			}
			else if(Meeting_My.Oder==2)
			{
				if(Meeting_Area.Direction==Rightside)
				{
					for(i=RowMax-2;i>DropRow;i--)
					{
						if(RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=RightEdge[i]>ColumnMax-1?ColumnMax-1:RightEdge[i]-Width[i]/4;
						}
						else 
						{
							if(LeftEdge[i]!=0)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]*3/4>135?135:LeftEdge[i]+Width[i]*3/4);
							}
							else
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
				else
				{
					for(i=RowMax-2;i>DropRow;i--)
					{
						if(LeftEdge[i]!=0)
						{
							MiddleLine[i]=LeftEdge[i]<1?1:LeftEdge[i]+Width[i]/4;
						}
						else 
						{
							if(RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]*3/4<5?5:RightEdge[i]-Width[i]*3/4);
							}
							else
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
				
			}
		}
		else if(Meeting_My.State==5)
		{
			for(i=RowMax-2;i>DropRow;i--)
			{
				if(LeftEdge[i]!=0)
				{
					MiddleLine[i]=LeftEdge[i]<1?1:LeftEdge[i]+Width[i]/6;
				}
				else 
				{
					if(RightEdge[i]!=ColumnMax)
					{
						MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]*5/6<5?5:RightEdge[i]-Width[i]*5/6);
					}
					else
					{
						MiddleLine[i]=MiddleLine[i+1];
					}
				}
			}
		}
	}
	else
	{
		/*****************����������������**************************/
		if(!Roundabout.Flag&&!Crossroad.Flag)
		{
			for(i=RowMax-2;i>DropRow;i--)
			{
				if(i==RowMax-2)
				{
					if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
					{
						MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
					}
					if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
					{
						MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
					}
					if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
					{
						MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
					}
					if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
					{
						MiddleLine[i]=MiddleLine[i+1];
					}
				}
				else
				{
					if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
					{
						if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
							MiddleLine[i]=MiddleLine[i+1];
						else
							MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
					}
					if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
					{
						MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
					}
					if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
					{
						MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
					}
					if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
					{
						MiddleLine[i]=MiddleLine[i+1];
					}
				}
			}
		}
		else if(Roundabout.Flag)
		{
			for(i=RowMax-2;i>DropRow;i--)
			{
				if(Roundabout.State==5||Roundabout.State==4)//�ڵ��ĵ���׶�ʱ������
				{
					if(Roundabout.Direction==Rightside)
					{
						if(LeftEdge[i]==0)
						{
							if(i<73)
							{
								DropRow=i;
								break;
							}
							MiddleLine[i]=MiddleLine[i+1];
						}
						else
							MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
					}
					else if(Roundabout.Direction==Leftside)
					{
						if(RightEdge[i]==ColumnMax)
						{
							if(i<73)
							{
								DropRow=i;
								break;
							}
							MiddleLine[i]=MiddleLine[i+1];
						}
						else
							MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
					}
				}
				else if(Roundabout.State==6)
				{
					if(Roundabout.Direction==Rightside)
					{
						MiddleLine[i]=(uint16)(LeftEdge[i]+0.8*Width[i]/2>135?135:LeftEdge[i]+0.8*Width[i]/2);
					}
					else
					{
						MiddleLine[i]=(uint16)(RightEdge[i]-0.8*Width[i]/2<5?5:RightEdge[i]-0.8*Width[i]/2);
					}
				}
				
				else//������������
				{
					if(i==RowMax-2)
					{
						if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
						}
						if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
						}
						if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=MiddleLine[i+1];
						}
					}
					else
					{
						if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
						{
							if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
								MiddleLine[i]=MiddleLine[i+1];
							else
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=(uint16)(RightEdge[i]-Width[i]/2<5?5:RightEdge[i]-Width[i]/2);
						}
						if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=(uint16)(LeftEdge[i]+Width[i]/2>135?135:LeftEdge[i]+Width[i]/2);
						}
						if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
						{
							if(i<73)
							{
								DropRow=i;
								break;
							}
							MiddleLine[i]=MiddleLine[i+1];
						}
					}
				}
			}
		}
		else if(Crossroad.Flag)
		{
			if(Crossroad.State==1)
			{ 
				if(Crossroad.Direction==Straight)
				{	
					int end=MAX(InflectionPointR.Row,InflectionPointL.Row);
					for(i=MAX(RightStart,LeftStart);i>end;i--)
					{
						if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
						{
							if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
								MiddleLine[i]=MiddleLine[i+1];
							else
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
						{
							MiddleLine[i]=RightEdge[i]-Width_Straight[i]/2;
						}
						if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=LeftEdge[i]+Width_Straight[i]/2;
						}
						if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
						{
							MiddleLine[i]=MiddleLine[i+1];
						}
					}
				}
				else if(Crossroad.Direction==Leftside)
				{
					int end=InflectionPointL.Row;
					for(i=LeftStart;i>end;i--)
					{
						MiddleLine[i]=LeftEdge[i]+Width_Straight[i]/2;
					}
				}
				else if(Crossroad.Direction==Rightside)
				{
					int end=InflectionPointR.Row;
					for(i=RightStart;i>end;i--)
					{
						MiddleLine[i]=RightEdge[i]-Width_Straight[i]/2;
					}
				}
				
			}
			else if(Crossroad.State==2)
			{
				if(Crossroad.Direction==Straight)
				{
					for(i=RowMax-2;i>DropRow;i--)
					{
						if(i==RowMax-2)
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
						else
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
									MiddleLine[i]=MiddleLine[i+1];
								else
									MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
				else if(Crossroad.Direction==Leftside)
				{
					for(i=RowMax-2;i>=DropRow;i--)   
					{
						if(i==RowMax-2)
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
						else
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
									MiddleLine[i]=MiddleLine[i+1];
								else
									MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
						
					}
				}
				else if(Crossroad.Direction==Rightside)
				{
					for(i=RowMax-2;i>=DropRow;i--)   
					{
						if(i==RowMax-2)
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
						else
						{
							if(LeftEdge[i]!=0&&RightEdge[i]!=ColumnMax)
							{
								if((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))
									MiddleLine[i]=MiddleLine[i+1];
								else
									MiddleLine[i]=(LeftEdge[i]+RightEdge[i])/2;
							}
							if(LeftEdge[i]==0&&RightEdge[i]!=ColumnMax)
							{
								MiddleLine[i]=(uint16)(RightEdge[i]-Width_Straight[i]/2<5?5:RightEdge[i]-Width_Straight[i]/2);
							}
							if(LeftEdge[i]!=0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=(uint16)(LeftEdge[i]+Width_Straight[i]/2>135?135:LeftEdge[i]+Width_Straight[i]/2);
							}
							if(LeftEdge[i]==0&&RightEdge[i]==ColumnMax)
							{
								MiddleLine[i]=MiddleLine[i+1];
							}
						}
					}
				}
			}
		}
	}
}
