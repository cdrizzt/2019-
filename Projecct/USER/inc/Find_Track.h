#ifndef _FindTrack_h
#define _FindTrack_h
#include "headfile.h"
#define RowMax		140
#define ColumnMax	140
#define FAR		4
#define CLOSE   7
#define TrackArea_Ratio 0.4



#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )         //��λ��ȡ���
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )         //��λ��ȡ��С

extern int16   MiddleLine[RowMax+1];   //��������
extern uint16   RightEdge[RowMax+1];    //�ұ�������
extern uint16   LeftEdge[RowMax+1];     //���������
extern uint16 	 MidPri;
//���߱�־
extern uint16   LeftLose;                 //��������
extern uint16   RightLose;                //�Ҷ�������
extern uint16   AllLose;                  //ȫ��������
extern uint16   AllLoseClose;             //����ȫ��������
//������ʼ�� �յ���
extern uint16   LeftStart;                //���һ��������
extern uint16   RightStart;               //�ҵ�һ��������
//extern uint16	 LeftInfRow,LeftInfCol;		//��߶��ߵ�
//extern uint16	 RightInfRow,RightInfCol;	//�ұ߶��ߵ�
extern uint16 	 LeftEndRow,RightEndRow;
//��������
extern uint16	Right_Quantity;
extern uint16	Left_Quantity;
//��ֹ��
extern uint16 DropRow;
extern uint16 DropRow_Straight;
extern uint8   Lose_Line;
extern uint8    Width[RowMax+1];

void Search_CenterBlackline();
void Get_EndLine(uint8 middle);
void Calculation_Middleline();
void Search_CenterBlackline_Meeting();
#endif