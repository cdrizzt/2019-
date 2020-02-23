#ifndef _Roundabout_h
#define _Roundabout_h

#include "headfile.h"

#define Rightside               1
#define Leftside                0
#define Straight                2

#define Entrance_Initial        7		//��ֵ
#define Entrance_BaseRow        65		//��׼��
#define Entrance_Coefficient    0.3	//�仯��Χ
#define Min_Initial				2		//��Сֵ

typedef struct
{
    uint8  Flag;			//��־λ
	uint8  Direction;		//����
	uint8  State;			//����״̬
	uint8  Last_Time;		//��һ���Ƿ�Ѱ�ҵ�
	uint16 Col;				//���ߵ���
	uint16 Row;				//���ߵ���
	uint16 Wait;			//�ȴ�ʱ��
	int16  Distance;		//����·��
	uint8  Size;			//�� 1 
	uint8  Number;			//��������
	uint8  Mode;			//1��� 0����ͷ
	
}roundabout;       	//�����ṹ��

extern roundabout Roundabout;            //���������ṹ�����
extern uint8 Roundabout_Direction[6];

void Judge_Roundabout(roundabout *prt);			//�����жϳ���
void ModifyEdge_Roundabout(roundabout *prt);	//��������

#endif