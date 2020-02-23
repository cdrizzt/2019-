#ifndef _Crossroads_h
#define _Crossroads_h
#include "headfile.h"

typedef struct
{
	char Start_Flag;		   //��ʼѰ��ʮ�ֱ�־λ
	char Flag;                //��־λ
	char Direction;           //ʮ�ַ��� Leftside 0��б Rightside 1��б  2ֱ��
	char State;               //ʮ��״̬

}crossroad;       //ʮ�ֽṹ��

extern uint16 newedge[140];
extern crossroad Crossroad;

void Judge_Crossroad(crossroad *prt);
void ModifyEdge_Crossroad(crossroad *prt);

#endif