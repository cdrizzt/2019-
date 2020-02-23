#ifndef all_struct_h
#define all_struct_h




typedef struct
{
	uint8	Direction;		//����
	uint8	Flag;			//�ᳵ����־
	uint8	Start_Flag;		//��ʼ�жϻᳵ��
	uint8	Mode;			//0 ֱ���ᳵ����ͣ����  1����ᳵ��ͣ����
}meeting_area;

typedef struct
{
	uint8	Flag;
	uint8	State;			//��Ϊ6�� 0 1 3 5 6 Ϊ�ᳵ  2 4������  3-�м�ᳵ 7-����
	uint8   Track_Mode;		//0����ͷѲ��  1���ѭ��
	uint8	Electric;		//0����ͷѭ��	1���ѭ��
	uint8	End;			//�����ᳵ��־λ
	uint16 	Wait_Time;		//�ȴ�ʱ��
	uint16  Distance;		//��������
	uint8	Find_Track;		//����Ѳ��ƫ��
	uint8	Stop_Flag;		//ͣ����־
	uint8	Brake;			//ɲ����־ 1����ɲ�� 0ɲ������x
	uint8	Oder;			//����˳��
	uint8	Send_Flag;  //ͨѶ��־λ
}meeting;

extern uint8 Mode;
//void Running(void);

#endif