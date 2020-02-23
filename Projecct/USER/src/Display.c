#include "Display.h"

/*******************************************
TFTҺ��
�ֱ���              128*160    
*********************************************/

Site_t site={80,0};
Size_t size={80,80};
uint8 flag=0;
uint8 mode=0;

/*	
#if display
	for(int i=140;i>0;i--)
		for(int j=0;j<140;j++)
			image_canny[i][j]=0;
	
	for(int i=140;i>DropRow;i--)
		for(int j=0;j<138;j++)
		{
			if(j<LeftEdge[i]||j>RightEdge[i])
			{
				image_canny[i][j]=0;
			}
			else
			{
				image_canny[i][j]=255;
			}
		}
	Display_image(image_canny[0],site,size);    //ͼ����ʾ����
#else
	Display_image(image[0],site,size);    //ͼ����ʾ����
#endif
	*/

void Display0()
{
	int i;
	int32 a;
	Site_t site_point={0,0};
	site.x=80;	site.y=0;  //��ʾͼ�����Ͻ�λ��    {x,y}
	size.W=80;	size.H=80;	//��ʾ����ͼ���С      {W,H}
	
	
	Display_image(image[0],site,size);    //ͼ����ʾ����
	for(i=ROW;i>=50;i--)                 //���߳���  ��ͼ������
	{
		site_point.x=site.x-i*size.H/ROW;
		//����
		
		site_point.y=LeftEdge[i]*size.W/COL+2+site.y;    //����X������  ͼ������*��ͼ����ʾ��С/ͼ��ԭ��С��
		LCD_point(site_point.x,site_point.y,GREEN);
		
		//����
		site_point.y=RightEdge[i]*size.W/COL-2+site.y;
		LCD_point(site_point.x,site_point.y,YELLOW);
		
		//����
		site_point.y=MiddleLine[i]*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,RED);
		
		//ƫ��
		site_point.y=(int)(70-Error)*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,BLACK);
		
	}
	for(i=50;i>=40;i--)
	{
		site_point.y=70*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,RED);
	}
	//��ʾ�յ�
	if(InflectionPointR.Flag==1||InflectionPointL.Flag==1||Roundabout.State==2)
	{
		if(InflectionPointR.Flag==1)
		{
			site_point.y=InflectionPointR.Col*size.W/COL-2+site.y;
			site_point.x=site.x-InflectionPointR.Row*size.H/ROW;
			Display_cross(&site_point,3,BLUE);
		}
		
		if(InflectionPointL.Flag==1)
		{
			site_point.y=InflectionPointL.Col*size.W/COL-2+site.y;
			site_point.x=site.x-InflectionPointL.Row*size.H/ROW;
			Display_cross(&site_point,3,BLUE);
		}
		
		if(Roundabout.State==2)
		{
			site_point.y=Roundabout.Col*size.W/COL-2+site.y;
			site_point.x=site.x-Roundabout.Row*size.H/ROW;
			Display_cross(&site_point,4,WHITE);
		}
	}
	//
	if(Crossroad.State==2)
	{
		site_point.y=RightEdge[RightStart]*size.W/COL-2+site.y;
		site_point.x=site.x-RightStart*size.H/ROW;
		Display_cross(&site_point,3,GREEN);
		
		site_point.y=LeftEdge[LeftStart]*size.W/COL-2+site.y;
		site_point.x=site.x-LeftStart*size.H/ROW;
		Display_cross(&site_point,3,YELLOW);
	}
	
	site_point.x=128;//������ʾ�е�ַ
	site_point.y=0;
	lcd_showstr(site_point.x,site_point.y,"Le:",YELLOW);
	
	//��ʾ����
	{
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=25;
		a=DropRow;
		lcd_showint8(site_point.x,site_point.y,a,YELLOW);
		
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=0;
		lcd_showstr(site_point.x,site_point.y,"Ls:",YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=25;
		a=(int)DropRow_Straight;
		lcd_showint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=0;
		lcd_showstr(site_point.x,site_point.y,"Er:",YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=25;
		a=(int)Error;
		lcd_showint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=68;
		lcd_showstr(site_point.x,site_point.y,"A:",YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=75;
		a=AllLose;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=68;
		lcd_showstr(site_point.x,site_point.y,"R:",YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=75;
		a=RightLose;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=68;
		lcd_showstr(site_point.x,site_point.y,"L:",YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=75;
		a=LeftLose;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=112;
		lcd_showstr(site_point.x,site_point.y,"AC:",YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=128;
		a=AllLoseClose;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=112;
		lcd_showstr(site_point.x,site_point.y,"RS:",YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=128;
		a=Right_Quantity;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=112;
		lcd_showstr(site_point.x,site_point.y,"LS:",YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=128;
		a=Left_Quantity;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=80;//������ʾ�е�ַ
		site_point.y=80;
		lcd_showstr(site_point.x,site_point.y,"R:",YELLOW);
		
		site_point.x=80;//������ʾ�е�ַ
		site_point.y=97;
		if(InflectionPointR.Flag)
			a=InflectionPointR.Row;
		else
			a=0;
		lcd_showuint8_3(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=80;//������ʾ�е�ַ
		site_point.y=129;
		if(InflectionPointR.Flag)
			a=InflectionPointR.Col;
		else
			a=0;
		lcd_showuint8_3(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=64;//������ʾ�е�ַ
		site_point.y=80;
		lcd_showstr(site_point.x,site_point.y,"L:",YELLOW);
		
		site_point.x=64;//������ʾ�е�ַ
		site_point.y=97;
		if(InflectionPointL.Flag)
			a=InflectionPointL.Row;
		else
			a=0;
		lcd_showuint8_3(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=64;//������ʾ�е�ַ
		site_point.y=129;
		if(InflectionPointL.Flag)
			a=InflectionPointL.Col;
		else
			a=0;
		lcd_showuint8_3(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=48;//������ʾ�е�ַ
		site_point.y=80;
		lcd_showstr(site_point.x,site_point.y,"S:",YELLOW);
		
		site_point.x=48;//������ʾ�е�ַ
		site_point.y=97;
		a=Right_AD+Left_AD;
		lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
//		site_point.x=48;//������ʾ�е�ַ
//		site_point.y=105;
//		if(InflectionPointR.Flag)
//			a=InflectionPointR.Number;
//		else
//			a=0;
//		lcd_showchar(site_point.x,site_point.y,'0' + a,YELLOW);
//		
//		site_point.x=48;//������ʾ�е�ַ
//		site_point.y=120;
//		lcd_showstr(site_point.x,site_point.y,"LN:",YELLOW);
//		
//		site_point.x=48;//������ʾ�е�ַ
//		site_point.y=145;
////		if(InflectionPointL.Flag)
////			a=InflectionPointL.Number;
////		else
////			a=0;
//		a=Left_AD+Right_AD;
//		lcd_showchar(site_point.x,site_point.y,'0' + a,YELLOW);
		
		site_point.x=32;//������ʾ�е�ַ
		site_point.y=80;
		lcd_showstr(site_point.x,site_point.y,"L:",YELLOW);
		
		site_point.x=32;//������ʾ�е�ַ
		site_point.y=97;
		a=(uint16)(Left_AD);
		lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=16;//������ʾ�е�ַ
		site_point.y=80;
		lcd_showstr(site_point.x,site_point.y,"R:",YELLOW);
		
		site_point.x=16;//������ʾ�е�ַ
		site_point.y=97;
		a=Right_AD;
		lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
		
		site_point.y=129;
		
	}
}

void Display1()
{
	int i;
	int32 a;
	Site_t site_point={0,0};
	site.x=111;	site.y=0;  //��ʾͼ�����Ͻ�λ��    {x,y}
	size.W=111;	size.H=111;	//��ʾ����ͼ���С��      {W,H}
	
	Display_image(image[0],site,size);    //ͼ����ʾ����
	
	for(i=ROW;i>=50;i--)                 //���߳���  ��ͼ������
	{
		site_point.x=site.x-i*size.H/ROW;
		//����
		
		site_point.y=LeftEdge[i]*size.W/COL+2+site.y;    //����X������  ͼ������*��ͼ����ʾ��С/ͼ��ԭ��С��
		LCD_point(site_point.x,site_point.y,GREEN);
		
		//����
		site_point.y=RightEdge[i]*size.W/COL-2+site.y;
		LCD_point(site_point.x,site_point.y,YELLOW);
		
		//����
		site_point.y=MiddleLine[i]*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,RED);
		
		//ƫ��
		site_point.y=(int)(70-Error)*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,BLACK);
		
	}
	for(i=50;i>=40;i--)
	{
		site_point.y=70*size.W/COL+site.y;
		LCD_point(site_point.x,site_point.y,RED);
	}
	//��ʾ�յ�
	if(InflectionPointR.Flag==1||InflectionPointL.Flag==1||Roundabout.State==2)
	{
		if(InflectionPointR.Flag==1)
		{
			site_point.y=InflectionPointR.Col*size.W/COL-2+site.y;
			site_point.x=site.x-InflectionPointR.Row*size.H/ROW;
			Display_cross(&site_point,3,BLUE);
		}
		
		if(InflectionPointL.Flag==1)
		{
			site_point.y=InflectionPointL.Col*size.W/COL-2+site.y;
			site_point.x=site.x-InflectionPointL.Row*size.H/ROW;
			Display_cross(&site_point,3,BLUE);
		}
		
		if(Roundabout.State==2)
		{
			site_point.y=Roundabout.Col*size.W/COL-2+site.y;
			site_point.x=site.x-Roundabout.Row*size.H/ROW;
			Display_cross(&site_point,4,WHITE);
		}
	}
	//
	if(Crossroad.State==2)
	{
		site_point.y=RightEdge[RightStart]*size.W/COL-2+site.y;
		site_point.x=site.x-RightStart*size.H/ROW;
		Display_cross(&site_point,3,GREEN);
		
		site_point.y=LeftEdge[LeftStart]*size.W/COL-2+site.y;
		site_point.x=site.x-LeftStart*size.H/ROW;
		Display_cross(&site_point,3,YELLOW);
	}
	
	{
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=0;
		lcd_showstr(site_point.x,site_point.y,"Le:",YELLOW);
		
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=25;
		a=DropRow;
		lcd_showint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"A:",YELLOW);
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=120;
		a=AllLose;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"oo:",YELLOW);
		
		site_point.x=113;//������ʾ�е�ַ
		site_point.y=120;
		a=Meeting_Other.Oder;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"mo",YELLOW);
		
		site_point.x=98;//������ʾ�е�ַ
		site_point.y=120;
		a=Meeting_My.Oder;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=83;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"of:",YELLOW);
		
		site_point.x=83;//������ʾ�е�ַ
		site_point.y=130;
		a=Meeting_Other.Flag;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=68;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"os:",YELLOW);
		
		site_point.x=68;//������ʾ�е�ַ
		site_point.y=130;
		a=Meeting_Other.State;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=53;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"Mf:",YELLOW);
		
		site_point.x=53;//������ʾ�е�ַ
		site_point.y=130;
		a=Meeting_My.Flag;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=38;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"ms:",YELLOW);
		
		site_point.x=38;//������ʾ�е�ַ
		site_point.y=130;//site_point.y=123;
		a=Meeting_My.State;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
		
		site_point.x=23;//������ʾ�е�ַ
		site_point.y=113;
		lcd_showstr(site_point.x,site_point.y,"me",YELLOW);
		
		site_point.x=23;//������ʾ�е�ַ
		site_point.y=130;//site_point.y=123;
		a=Meeting_My.End;
		lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
	}
	
}

void Display2()
{
	int32 a;
	flag=1;
	Site_t site_point={0,0};
/**************************************************/
	site_point.x=128;//������ʾ�е�ַ
	site_point.y=0;
	lcd_showstr(site_point.x,site_point.y,"R:",YELLOW);
	
	
	site_point.x=128;//������ʾ�е�ַ
	site_point.y=17;
	a=Right_AD;
	lcd_showint8(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=113;//������ʾ�е�ַ
	site_point.y=0;
	lcd_showstr(site_point.x,site_point.y,"L:",YELLOW);
		
	site_point.x=113;//������ʾ�е�ַ
	site_point.y=17;
	a=Left_AD;
	lcd_showint8(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=98;//������ʾ�е�ַ
	site_point.y=0;
	lcd_showstr(site_point.x,site_point.y,"S:",YELLOW);
	
	
	site_point.x=98;//������ʾ�е�ַ
	site_point.y=17;
	a=(int16)Left_AD+Right_AD;
	lcd_showint8(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=128;//������ʾ�е�ַ
	site_point.y=70;
	lcd_showstr(site_point.x,site_point.y,"TR:",YELLOW);
	
	
	site_point.x=128;//������ʾ�е�ַ
	site_point.y=95;
	a=AD_right;
	lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=113;//������ʾ�е�ַ
	site_point.y=70;
	lcd_showstr(site_point.x,site_point.y,"TL:",YELLOW);
	
	
	site_point.x=113;//������ʾ�е�ַ
	site_point.y=95;
	a=AD_left;
	lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=93;//������ʾ�е�ַ
	site_point.y=70;
	lcd_showstr(site_point.x,site_point.y,"ML:",YELLOW);
	
	
	site_point.x=93;//������ʾ�е�ַ
	site_point.y=95;
	a=ad_max_left;
	lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
	
	site_point.x=78;//������ʾ�е�ַ
	site_point.y=70;
	lcd_showstr(site_point.x,site_point.y,"MR:",YELLOW);
	
	
	site_point.x=78;//������ʾ�е�ַ
	site_point.y=95;
	a=ad_max_right;
	lcd_showuint16(site_point.x,site_point.y,a,YELLOW);
/****************************************************/
	
	if(Button_2==0)
	{
		while(Button_2==0);	//��������
		systick_delay_ms(5);
		AD_Normalization_Flag=1;
		AD_Normalization();		//��һ��
		flag=0;					//�˳�ģʽ
		mode=0;
		dsp_single_colour(RED);	//�л�ģʽʱ������
		
	}
	if(Button_1==0)			
	{
		while(Button_1==0);	//��������
		systick_delay_ms(5);
		flag=0;					//�˳�ģʽ
		mode=0;
		dsp_single_colour(RED);	//�л�ģʽʱ������
	}
}

void Display3()
{
	
	int i;
	int32 a;
	Site_t site_point={0,0};
	
	//��ʾ��ͼ��
	{
		
		site.x=111;	site.y=0;  //��ʾͼ�����Ͻ�λ��    {x,y}
		size.W=111;	size.H=111;	//��ʾ����ͼ���С��      {W,H}
		
		Display_image(image[0],site,size);    //ͼ����ʾ����
		
		for(i=ROW;i>=50;i--)                 //���߳���  ��ͼ������
		{
			site_point.x=site.x-i*size.H/ROW;
			//����
			
			site_point.y=LeftEdge[i]*size.W/COL+2+site.y;    //����X������  ͼ������*��ͼ����ʾ��С/ͼ��ԭ��С��
			LCD_point(site_point.x,site_point.y,GREEN);
			
			//����
			site_point.y=RightEdge[i]*size.W/COL-2+site.y;
			LCD_point(site_point.x,site_point.y,YELLOW);
			
			//����
			site_point.y=MiddleLine[i]*size.W/COL+site.y;
			LCD_point(site_point.x,site_point.y,RED);
			
			
			site_point.y=(int)(70-Error)*size.W/COL+site.y;
			LCD_point(site_point.x,site_point.y,BLACK);
		}
		
		if(InflectionPointR.Flag==1||InflectionPointL.Flag==1||Roundabout.State==2)
		{
			if(InflectionPointR.Flag==1)
			{
				site_point.y=InflectionPointR.Col*size.W/COL-2+site.y;
				site_point.x=site.x-InflectionPointR.Row*size.H/ROW;
				Display_cross(&site_point,3,BLUE);
			}
			
			if(InflectionPointL.Flag==1)
			{
				site_point.y=InflectionPointL.Col*size.W/COL-2+site.y;
				site_point.x=site.x-InflectionPointL.Row*size.H/ROW;
				Display_cross(&site_point,3,BLUE);
			}
			
			if(Roundabout.State==2)
			{
				site_point.y=Roundabout.Col*size.W/COL-2+site.y;
				site_point.x=site.x-Roundabout.Row*size.H/ROW;
				Display_cross(&site_point,4,WHITE);
			}
		}
		
		if(Crossroad.State==2)
		{
			site_point.y=RightEdge[RightStart]*size.W/COL-2+site.y;
			site_point.x=site.x-RightStart*size.H/ROW;
			Display_cross(&site_point,3,GREEN);
			
			site_point.y=LeftEdge[LeftStart]*size.W/COL-2+site.y;
			site_point.x=site.x-LeftStart*size.H/ROW;
			Display_cross(&site_point,3,YELLOW);
		}
	}
	
	//��ʾ����
	{
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=0;
		lcd_showstr(site_point.x,site_point.y,"exp_time:",YELLOW);
		
		
		site_point.x=128;//������ʾ�е�ַ
		site_point.y=25;
		a=EXP_Time;;
		lcd_showuint8_3(site_point.x,site_point.y,a,YELLOW);
	}
	//�޸Ĳ���
	{
		if(Button_3==0)
		{
			while(Button_3==0);
			systick_delay_ms(5);
			EXP_Time=EXP_Time+5;
			EXP_Time=EXP_Time>209?209:EXP_Time;
			EXP_Time=EXP_Time<10?10:EXP_Time;
			while(EXP_Time!=set_exposure_time(EXP_Time));
		}
		if(Button_4==0)
		{
			while(Button_4==0);
			systick_delay_ms(5);
			EXP_Time=EXP_Time-7;
			EXP_Time=EXP_Time>209?209:EXP_Time;
			EXP_Time=EXP_Time<10?10:EXP_Time;
			while(EXP_Time!=set_exposure_time(EXP_Time));
		}
		
		if(Button_2==0)
		{
			systick_delay_ms(10);
			while(Button_2==0);
			Beep_ON;	
			systick_delay_ms(10);
			
			while(FLASH_EraseSector(252));	//��������
			systick_delay_ms(10);
			FLASH_WriteSector(252,(const uint8 *)&EXP_Time,16,0);//ȷ������flash	255 254�������ڵ�� 253������������ͷ
			systick_delay_ms(5);
			flag=0;
			Beep_OFF;
			mode=0;
			dsp_single_colour(RED);	//�л�ģʽʱ������
		}
		if(Button_1==0)
		{
			flag=0;
			mode=0;
			dsp_single_colour(RED);	//�л�ģʽʱ������
		}
	
	}
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      LCD��ʾ���� //���Ժ���
//  @param      NULL
//  @return     void					
//  @since      v1.0
//  Sample usage:		��ʾͼ�� ���� ��ֵ
//	@Mode:		 0 ������ʾ��ȡ���� 1 �Ŵ���ʾ���ᳵ������ ȷ�ϣ� 2��У�- //���ɼ�ͼ�� 3 ���ع⣨+��
//	@Mode:		flag(��λ)//2 3��λ
//-------------------------------------------------------------------------------------------------------------------

void Display()
{
	
	//��ʾͼ��ʱ��������ʱ��100ms �ڴ�ʱ�ر�ͼ��ɼ�
	
	if(flag==0)//����λ�ǿ��������л�ģʽ
	{
		if(Button_1==0)
		{
			while(Button_1==0);	//��������
			systick_delay_ms(5);
			dsp_single_colour(RED);	//�л�ģʽʱ������
			mode=0;					//ģʽ0
			
		}
		if(Button_2==0)
		{
			while(Button_2==0);	//��������
			systick_delay_ms(5);
			dsp_single_colour(RED);	//�л�ģʽʱ������
			mode=1;					//ģʽ0
		}
		if(Button_3==0)
		{
			while(Button_3==0);	//��������
			systick_delay_ms(5);
			dsp_single_colour(RED);	//�л�ģʽʱ������
			mode=2;	
			flag=1;					//��λ
		}
		if(Button_4==0)
		{
			while(Button_4==0);	//��������
			systick_delay_ms(5);
			dsp_single_colour(RED);	//�л�ģʽʱ������
			mode=3;		
			flag=1;					//��λ
		}
	}
	
	
	//mode=0;
	switch(mode)
	{
	case 0:	Display0();	 break;//������ʾģʽ 
	case 1:	Display1();	 break;//�Ŵ�ͼ�� �ᳵ����ʾ
	case 2:	Display2();	 break;//���ֵ��ʾ �����ɼ�ͼ��
	case 3:	Display3();	 break;//�����ع�
	}
	
	
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ͼ�����ź���
//  @param      image[0] siteλ�� size��С
//  @return     void					
//  @since      v1.0
//  Sample usage:		���ӣ�displayimage(displayimage(image[0],siteͼ��λ��,sizeͼ���С);
//-------------------------------------------------------------------------------------------------------------------
void Display_image(uint8 *prt,Site_t site, Size_t size)
{
	int i,j;
	uint16 color = 0;
	uint16 temp = 0;
	
	//	for(i=site.x;i<size.W+site.x;i++)//��ֱ��ʾ
	//	{
	//		Lcd_SetRegion(i,site.y,i,160-1);		//��������
	//		for(j=0;j<size.H;j++)
	//		{	
	//			temp = *(prt + j*ROW/size.H*COL + (i-site.x)*(COL-1)/(size.W-1));//��ȡ���ص�
	//			
	//			color=(0x001f&((temp)>>3))<<11;
	//			color=color|(((0x003f)&((temp)>>2))<<5);
	//			color=color|(0x001f&((temp)>>3));
	//			LCD_WriteData_16Bit(color);
	//		}
	//	}
	
	for(i=site.x;i>=site.x-size.H;i--)//������ʾ
	{
		Lcd_SetRegion(i,site.y,i,160-1);		//��������
		for(j=site.y;j<site.y+size.W;j++)
		{
			temp = *(prt + (site.x-i)*ROW/size.H*ROW + (j-site.y)*(COL-1)/(size.W-1));//��ȡ���ص�
			
			color=(0x001f&((temp)>>3))<<11;
			color=color|(((0x003f)&((temp)>>2))<<5);
			color=color|(0x001f&((temp)>>3));
			LCD_WriteData_16Bit(color);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �������
//  @param      ���� ��С ��ɫ
//  @return     void					
//  @since      v1.0
//  Sample usage:		���ӣ�Display_cross(Site_t *prt,uint8 size,uint16 color)
//-------------------------------------------------------------------------------------------------------------------
void Display_cross(Site_t *prt,uint8 size,uint16 color)
{
	int i;
	for(i=size;i>=-size;i--)
	{
		LCD_point(prt->x-i,prt->y-i,color);
		LCD_point(prt->x-i,prt->y+i,color);
	}
}
void LCD_point(uint16 x,uint16 y, uint16 rgb565)
{
	Lcd_SetRegion(x,y,x+1,y+1);
	LCD_WriteData_16Bit(rgb565);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����������ʾ����
//  @param      
//  @return     void					
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void Display_Information(uint8 i,uint8 Number,uint8 Long)
{
	uint8 a=0;
	Site_t site_point={0,0};
	site_point.x=128-i*15;//������ʾ�е�ַ
	site_point.y=0;
	if(i==0)
	{
		lcd_showstr(site_point.x,site_point.y,"BumpNo:",YELLOW);
		while(1);
	}
	if(i==1)
		lcd_showstr(site_point.x,site_point.y,"BumpLg:",YELLOW);
	if(i==2)
		lcd_showstr(site_point.x,site_point.y,"RoundNo:",YELLOW);
	if(i==3)
		lcd_showstr(site_point.x,site_point.y,"Le:",YELLOW);
	if(i==4)
		lcd_showstr(site_point.x,site_point.y,"Le:",YELLOW);
	
	
	
	site_point.y=50;
	a=Number;
	lcd_showuint8(site_point.x,site_point.y,a,YELLOW);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      �ַ���ʾ����
//  @param      
//  @return     void					
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void lcd_showchar(uint16 x,uint16 y,uint8 dat,int color)
{
	uint8 i,j;
	uint8 temp;
	for(i=0; i<16; i++)
	{
		Lcd_SetRegion(x-i,y,x-i,y+7);
		temp = tft_ascii[dat-32][i];//��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
		for(j=0; j<8; j++)
		{
			if(temp&0x01)	LCD_WriteData_16Bit(color);
			else			LCD_WriteData_16Bit(BGCOLOR);
			temp>>=1;
		}
	}
}
void lcd_showstr(uint16 x,uint16 y,uint8 dat[],int color)
{
	uint16 j;
	
	j = 0;
	while(dat[j] != '\0')
	{
		lcd_showchar(x,y+8*j,dat[j],color);
		j++;
	}
}

void lcd_showint8(uint16 x,uint16 y,int8 dat,int color)
{
	uint8 a[3];
	uint8 i;
	if(dat<0)
	{
		lcd_showchar(x,y,'-',color);
		dat = -dat;
	}
	else	lcd_showchar(x,y,' ',color);
	
	a[0] = dat/100;
	a[1] = dat/10%10;
	a[2] = dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x,y+(8*(i+1)),'0' + a[i],color);
		i++;
	}
	
}

void lcd_showuint8_3(uint16 x,uint16 y,uint16 dat,int color)
{
	uint16 a[3];
	uint8 i;
	a[0] = (uint16)dat/100;
	a[1] = (uint16)dat/10%10;
	a[2] = (uint16)dat%10;
	i = 0;
	while(i<3)
	{
		lcd_showchar(x,y+(8*(i)),'0' + a[i],color);
		i++;
	}
	
}

void lcd_showuint8(uint16 x,uint16 y,uint8 dat,int color)
{
	uint8 a[2];
	uint8 i;
	
	a[0] = dat/10%10;
	a[1] = dat%10;
	
	i = 0;
	while(i<2)
	{
		lcd_showchar(x,y+(8*(i+1)),'0' + a[i],color);
		i++;
	}
	
}

void lcd_showint16(uint16 x,uint16 y,int16 dat,int color)
{
	uint8 a[5];
	uint8 i;
	if(dat<0)
	{
		lcd_showchar(x,y,'-',color);
		dat = -dat;
	}
	else	lcd_showchar(x,y,' ',color);
	
	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	i = 0;
	
	while(i<5)
	{
		lcd_showchar(x,y+(8*(i+1)),'0' + a[i],color);
		i++;
	}
}

void lcd_showuint16(uint16 x,uint16 y,uint16 dat,int color)
{
	uint8 a[5];
	uint8 i;
	
	a[0] = dat/10000;
	a[1] = dat/1000%10;
	a[2] = dat/100%10;
	a[3] = dat/10%10;
	a[4] = dat%10;
	
	i = 0;
	while(i<5)
	{
		lcd_showchar(x,y+(8*(i+1)),'0' + a[i],color);
		i++;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      Һ����������
//  @param      color     		��ɫ����
//  @return     void
//  @since      v1.0
//  Sample usage:               dsp_single_colour(YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void dsp_single_colour(int color)
{
	uint8 i,j;
	Lcd_SetRegion(0,0,TFT_X_MAX-1,TFT_Y_MAX-1);
	for (i=0;i<TFT_Y_MAX;i++)
		for (j=0;j<TFT_X_MAX;j++)
			LCD_WriteData_16Bit(color);
}