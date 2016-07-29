#define Send_Finished 0x00
#define Send_Port_Not_Exist 0x01
#define Send_Port_Low 0x02 
#define Send_Recognize1_Wrong 0x03 
#define Send_Recognize2_Wrong 0x04
char SendM_Port(char Port)
{
	unsigned char S[24], j, i, x;
	//һ��ʼ��Ҫ��������� 
	if(Port>=Num_Port) return Send_Port_Not_Exist;
	//���ܽ�����ǵ͵�ƽ���򷵻�
	if(Receive_Port[Port]() == 0) return Send_Port_Low; 
	//��ʼ���д�������
	j = 0; 
	for (i=15; i<=15; --i)
	{
		S[j] = ((SendData>>i)&1) * 10 + 10;
		j++;
	}
	for (i=7; i<=7; --i)
	{
		S[j] = ((SendOrder>>i)&1) * 10 + 10;
		j++;
	}
	//�����������
	//��ʼ���� �����0���Է�����10+���ڻظ�0 + 75λ�ĵ͵�ƽ����֪�Է�����Wy�豸 + �����1���Է�����10+���ڻظ�1 + ���75λ�ĸߵ�ƽ ������� 
	for (i=0; i<10 && Receive_Port[Port]() == 1; ++i)
	{
		Send_Port[Port](0);
	} 
	if (i == 10) 
	{
		Send_Port[Port](1);//һ��Ҫ�У���Ȼ�Է��ͻ�һֱ��Ϊ����Ϣ�ڴ���
		return Send_Recognize1_Wrong; //���Ե�ʱ��ȥ��
	} 
	for (i=0; i<37; ++i)
	{
		Send_Port[Port](0);
	} 
	for (i=0; i<10 && Receive_Port[Port]() == 0; ++i)
	{
		Send_Port[Port](1);
	}
	if (i == 10) return Send_Recognize1_Wrong; 
	for (i=0; i<37; ++i)
	{
		Send_Port[Port](1);
	}
	//�������� 
	//��ʼ���� 
	for (i=0; i<24; ++i)
	{
		x = S[i];
		for (;x;x--)
		{
			Send_Port[Port](0);
		}
		for (x=0;x<10; x++)
		{
			Send_Port[Port](1);
		}
	}
	//��������
	//�ٴζ��룬���ϴ�һ�� 
	for (i=0; i<10 && Receive_Port[Port]() == 1; ++i)
	{
		Send_Port[Port](0);
	} 
	if (i == 10)
	{
		Send_Port[Port](1);
		return Send_Recognize2_Wrong; //���Ե�ʱ��ȥ��
	} 
	for (i=0; i<37; ++i)
	{
		Send_Port[Port](0);
	} 
	for (i=0; i<10 && Receive_Port[Port]() == 0; ++i)
	{
		Send_Port[Port](1);
	}
	if (i == 10) return Send_Recognize2_Wrong; 
	Send_Port[Port](1);
	//��������
	//�������
	return Send_Finished;
} 
