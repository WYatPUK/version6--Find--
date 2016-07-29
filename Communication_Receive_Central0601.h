#define Receive_Finished 0x00
#define Receive_Port_Not_Exist 0x01
#define Receive_Port_High 0x02
#define Receive_Recognize1_Wrong 0x03
#define Receive_Recognize_Wrong 0x04
#define Receive_Recognize2_Wrong 0x05
#define Receive_Process_TimeOut 0x06
//�������ε�ʵ������
//01 28 29 11 12 12 24 11 11 24 11 11 11 24 24 11 24 12 12 11 23 11 24 11 11 11 11 29 00 
//01 28 29 10 11 12 24 11 11 24 12 12 11 23 24 11 24 12 12 12 12 12 12 11 11 24 24 29 00 
//Ϊ��ִ���ϸ�ı�׼��ʵ������-2������+2Ϊ���յ����䣬�ڴ��������һ����Ϊ�ǲ��Ϸ��豸
//26~31
//9~14
//21~26

//ʧ��

//�ٴ�ʵ��	  //������������������Щ����16������������10���ƣ������Բа���������������
//01 28 29 10 11 12 24 11 11 24 11 12 11 24 23 11 23 11 12 11 24 11 23 11 12 11 11 29 00 00 00 
//01 29 28 12 11 11 24 11 12 24 11 11 12 24 24 12 24 12 11 12 11 11 12 11 11 24 23 29 00 00 00 
//01 29 28 12 11 11 24 12 11 24 11 11 12 24 24 12 24 12 11 12 11 11 12 11 11 24 23 29 00 00 00 
//40��41   38~43
//16��17��18    14~20
//35��36    33~38

//�ɹ�
char ReceiveM_Port(char Port)
{
	unsigned char j, i;
	ReceiveOrder = 0;
	ReceiveData = 0;
	//һ��ʼ��Ҫ��������� 
	if(Port>=Num_Port) return Receive_Port_Not_Exist;
	//���ܽ�����Ǹߵ�ƽ���򷵻�
	if(Receive_Port[Port]() == 1) return Receive_Port_High;
	//��ʼ���룬��Ҫ���ϵĲ���Ҫ����10+������
	//���������0 Ȼ��ʼ�����������ʵ�飬�ȱ������������ͳһ�ô�������������ʵ��ʹ�ã���Ҫ�����жϣ��ж�С��10+�����ڣ���
	//�������1��Ȼ��ʼ������ͬ��
	Send_Port[Port](0);
	for (i=0; i<200 && Receive_Port[Port]() == 0; i++)
	{
		Send_Port[Port](0);//��ʵ����Ϊ������ʱ��ģ���Ȼһ��char����̫�಻�� 
	}
	if (i<18 || i>23) 
	{
		Send_Port[Port](1);//��һ��һ��Ҫ�У���Ȼ�´η��Ͳ��� 
		return Receive_Recognize1_Wrong;
	}
	Send_Port[Port](1);
	for (i=0; i<200 && Receive_Port[Port]() == 1; i++)
	{
		Send_Port[Port](1);
	}
	if (i<18 || i>23) return Receive_Recognize1_Wrong;
	//������� 
	for (i=0; i<16; i++)
	{
		for (j=0; j<70 && Receive_Port[Port]() == 0; ++j);
		if (j == 70) return Receive_Process_TimeOut;
		if (j<12 && j>5) //��Ϊ��0
		{
			ReceiveData<<=1;
			ReceiveData |= 0;
		}
		else if (j<23 && j>14) //��Ϊ��1
		{
			ReceiveData<<=1;
			ReceiveData |= 1;
		}
		else //����
		{
			return Receive_Recognize_Wrong;
		}
		for (j=0; j<70 && Receive_Port[Port]() == 1; ++j);
		if (j == 70) return Receive_Process_TimeOut;
	}
	for (i=0; i<7; i++)
	{
		for (j=0; j<70 && Receive_Port[Port]() == 0; ++j);
		if (j == 70) return Receive_Process_TimeOut;
		if (j<12 && j>5) //��Ϊ��0
		{
			ReceiveOrder<<=1;
			ReceiveOrder |= 0;
		}
		else if (j<23 && j>14) //��Ϊ��1
		{
			ReceiveOrder<<=1;
			ReceiveOrder |= 1;
		}
		else //����
		{
			return Receive_Recognize_Wrong;
		}
		for (j=0; j<70 && Receive_Port[Port]() == 1; ++j);
		if (j == 70) return Receive_Process_TimeOut;
	}
    for (j=0; j<70 && Receive_Port[Port]() == 0; ++j);
	if (j == 70) return Receive_Process_TimeOut;
	if (j<12 && j>5) //��Ϊ��0
	{
		ReceiveOrder<<=1;
		ReceiveOrder |= 0;	
	}
	else if (j<23 && j>14) //��Ϊ��1
	{
		ReceiveOrder<<=1;
		ReceiveOrder |= 1;
	}
	else //����
	{
		return Receive_Recognize_Wrong;
	}
	//�������
	//�ٴζ���
	for (j=0; j<70 && Receive_Port[Port]() == 1; ++j);
	if (j == 70) return Receive_Process_TimeOut;
	Send_Port[Port](0);
	for (i=0; i<200 && Receive_Port[Port]() == 0; i++)
	{
		Send_Port[Port](0);//��ʵ����Ϊ������ʱ��ģ���Ȼһ��char����̫�಻�� 
	} 
	if (i<18 || i>23) 
	{
		Send_Port[Port](1);//��һ��һ��Ҫ�У���Ȼ�´η��Ͳ��� 
		return Receive_Recognize2_Wrong;
	}
	Send_Port[Port](1);
	//�������
	//�ɹ�
	return Receive_Finished;
}
