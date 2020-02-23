#ifndef _Tracktype_h
#define _Tracktype_h

#include "headfile.h"

typedef struct
{
    uint16 Row;//�յ��������
    uint16 Col;//�յ��������
    uint8  Flag;//�յ�ı�־λ
    uint8  Number;//�յ�ĸ���
} InflectionPoint; //�����յ�ṹ��

extern InflectionPoint  InflectionPointL;//������߹յ�ṹ�����
extern InflectionPoint  InflectionPointR;//������߹յ�ṹ�����

void FindInflectionPointR(InflectionPoint *inf ,unsigned char Start ,unsigned char End );
void FindInflectionPointL(InflectionPoint *inf ,unsigned char Start ,unsigned char End );
double EdgeSlope(uint8 LineMode,int16 ScanStart,int16 ScanEnd);
void EdgeSlopeIntercept(uint8 LineMode,int16 ScanStart,int16 ScanEnd,double *Slope,double *Intercept);
#endif