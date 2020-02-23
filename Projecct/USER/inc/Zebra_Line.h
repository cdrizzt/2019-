#ifndef _Zebra_Line_h
#define _Zebra_Line_h

#include "headfile.h"

typedef struct
{
    uint8  Flag;      //��־λ
	uint8  Start_Flag;//��ʼѰ�ұ�־λ
    uint16 StartRow;  //��ʼ��
    uint16 EndRow;    //������
	uint16 Second_Flag;
	
}zebracrossing;       //������
extern zebracrossing ZebraCrossing;
void Judge_ZebraCrossing(zebracrossing *prt);
void Judge_MiddleZebraCrossing(zebracrossing *prt);
void ModifyEdge_ZebraCrossing(zebracrossing *prt);
#endif
