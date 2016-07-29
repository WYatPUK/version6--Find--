#include <reg51.h>

void SendCharHex(char x)
{
	unsigned char low;
	unsigned char high;
	low = x&0x0f;
	high = (x>>4)&0x0f;
	if (low<10) low = '0'+low;
	else low = 'A'+low-10;
	if (high<10) high = '0'+high;
	else high = 'A'+high-10;
	SBUF=high;
	while(!TI);
	TI=0;
	SBUF=low;
	while(!TI);
	TI=0;	
}


#include "GeneralAnswer0624.h"
#define This_UUID 0x1114

#define Password 0x3463

unsigned int R;//10λ����ֵ��6λ������
unsigned int order;//8λ����
unsigned int x;
int nearistp;
#include "Find_Struct.h"
#include "link.h"

#define Num_Port 1
#define Port0_Sendp P2^0
#define Port0_Receivep  P0^7
#include "Communication_Profile0601.h"
#include "Communication_Receive_Central0601.h"
#include "Communication_Send_Central0601.h"

char Str[30];
int Strlen;

void DelaySelfCheck()
{
	int i, j;
	for (i=0; i<200; i++)
	{
		for (j=0 ; j<200; j++);
	}
}
void UsartConfiguration();

void main()
{
	char mstate;
	UsartConfiguration();
	//P0 = 0;
	//P1 = 0;
	//P2 = 0;
	//P3 = 0;
	Initialize_Communication();
	while (1)
	{
	SendCharHex(0);
	}
	while (1)
	{
		 while (Receive_Port[0]() == 1);
		 mstate = ReceiveM_Port(0);
		 if (mstate != Receive_Finished)
		 {
		 	 continue;
		 }
		 if (ReceiveOrder == R_Cmd_GoStart) 
		 {
		 	 continue; //�ص�������
		 }
		 SendData = ReceiveData;
		 SendOrder = ReceiveOrder;
		 SendM_Port(0);
		 	 									   	    									  
		 if (ReceiveOrder == R_Cmd_Ask)//�ظ��豸�ͺ�
		 {
		     SendData = This_UUID;//���
			 SendOrder = Unit_Resistance;//˵���ǵ����͵�
			 SendM_Port(0);
		 }
		 else if (ReceiveOrder == R_Cmd_Check)//�����Լ����
		 {
		 	 int i, p;
			 if(ReceiveData == Password)
			 {
			 	 SendOrder = T_Cmd_PasswordAccepted;
				 SendData = 0;
				 SendM_Port(0);
			 }
			 else
			 {
			 	 SendOrder = T_Cmd_PasswordWrong;
				 SendData = 0;
				 SendM_Port(0);
				 continue;
			 }
			 P0 = 1<<7;
			 P1 = 0;
			 P2 = 1;
			 P3 = 3;
			 p=1;
			 for (i=0; i<7; i++)
			 {
			 	P0 |= p;
				DelaySelfCheck();
				p<<=1;
				P0 = 1<<7;
				DelaySelfCheck();
			 }
			 p=1;
			 for (i=0; i<8; i++)
			 {
			 	P1 |= p;
				DelaySelfCheck();
				p<<=1;
				P1 = 0;
				DelaySelfCheck();
			 }
			 p=0x04;
			 for (i=0; i<6; i++)
			 {
			 	P3 |= p;
				DelaySelfCheck();
				p<<=1;
				P3 = 3;
				DelaySelfCheck();
			 }
			 p=2;
			 for (i=0; i<7; i++)
			 {
			 	P2 |= p;
				DelaySelfCheck();
				p<<=1;
				P2 = 1;
				DelaySelfCheck();
			 }
		 }
		 else if (ReceiveOrder == R_Cmd_Link) //����8���ŵ�·
		 {
		 	 char a, b;
             ClearLink();
			 a = ReceiveData & 0x0f;	//�������ӷ�ʽ
			 b = (ReceiveData>>4) & 0x0f;
			 if ((a==1 && b==1) || (a==2 && b==2))
			 {
			 	  SendOrder = T_Cmd_LinkWrong;
				  SendData = 0;
				  SendM_Port(0);
				  continue;
			 }
			 Link(a,b);
			 SendOrder = T_Cmd_LinkFinished;
		 	 SendData = 0;
			 SendM_Port(0);
		 }
		 else if (ReceiveOrder == Unit_Resistance)	//�ǵ���ĵ�λ
		 {
			 x = ReceiveData;
			 Recognize_Resistance(x);
			 nearistp = Find_Nearist(x);
			 SendData = SListR[nearistp];
			 SendOrder = Unit_Resistance;
			 SendM_Port(0);
			 while (Receive_Port[0]() == 1);  //�ȴ����޳�ʱ�䣬Ϊ��֤��ȫ���ڽ����²���ǰ���пػ���Ҫ����GoStartָ��
			 if (ReceiveM_Port(0) != Receive_Finished) continue;
			 if (ReceiveOrder == R_Cmd_GoStart) continue; //�ص������� 
			 if (ReceiveOrder == R_Cmd_FindConfirm)	//ȷ��
			 Excute(nearistp);
		 }
		 else //δ֪����
		 {
		 	 SendOrder = T_Cmd_UnkownCmd;
			 SendData = 0;
			 SendM_Port(0);
		 } 
	}	
}
unsigned char CharToHex(char high, char low)
{
	unsigned char x; 
	if (high >= '0' && high <= '9')
	{
		x = (high - '0')<<4;
	}
	else if (high >= 'a' && high <= 'f')
	{
		x = (high - 'a' + 10)<<4;
	}
	else if (high >= 'A' && high <= 'F')
	{
		x = (high - 'A' + 10)<<4;
	}
	else return 0;
	if (low >= '0' && low <= '9')
	{
		x |= (low - '0');
	}
	else if (low >= 'a' && low <= 'f')
	{
		x |= (low - 'a' + 10);
	}
	else if (low >= 'A' && low <= 'F')
	{
		x |= (low - 'A' + 10);
	}
	else return 0;
	return x;	
}

void DoString() 
{
	if (Strlen == 6){
		SendCharHex(0);
		ReceiveOrder = CharToHex(Str[4],Str[5]);
		ReceiveData = CharToHex(Str[0],Str[1])<<4 + CharToHex(Str[2],Str[3]); 
		x = ReceiveData;
		//Recognize_Resistance(x);
		nearistp = Find_Nearist(x);
		SendData = SListR[nearistp];
		SendOrder = Unit_Resistance;
		SendCharHex(SendData>>4 & 0xff);
		SendCharHex(SendData & 0xff);
		SendCharHex(SendOrder & 0xff);
		SendCharHex(0);
	}
}
void DoUSART(unsigned char receiveData)
{
	switch (receiveData)
	{							 
		case '$': //ˢ��Str �� ����Str����
			if (Strlen != 0)
			{
				DoString();
				Strlen = 0;
			}
			Strlen = 0;
			break;
		default:
			Str[Strlen] = receiveData;
			Strlen ++;
			break;
	}
}
void UsartConfiguration()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XFA;
	//TH1=0XF3;		    //��������ʼֵ���ã�ע�Ⲩ������4800��
	//TL1=0XF3;
	TL1=0XFA;			 //������9600������
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					    //�򿪼�����
	ET1=0;
}
void Usart() interrupt 4
{
	unsigned char receiveData;
	receiveData=SBUF; //��ȥ���յ�������
	RI = 0;           //��������жϱ�־λ
	DoUSART(receiveData);
	//SBUF=receiveData; //�����յ������ݷ��뵽���ͼĴ���
	//while(!TI);		  //�ȴ������������
	//TI=0;			  //���������ɱ�־λ
}