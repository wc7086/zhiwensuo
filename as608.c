#include "as608.h"

unsigned int finger_id = 0;
//FINGERPRINTͨ��Э�鶨��
unsigned char code FPM10A_Get_Finger_Num[6] ={0x01,0x00,0x03,0x1d,0x00,0x21};												//���ģ������
//unsigned char code FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};			//������֤
unsigned char code FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  													//Э���ͷ
unsigned char code FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    													//���ָ��ͼ��
unsigned char code FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; 			//����ָ��������Χ0 - 999,ʹ��BUFFER1�е�����������
unsigned char code FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; 									//��ͼ����뵽BUFFER1
unsigned char code FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 									//��ͼ����뵽BUFFER2
unsigned char code FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; 														//��BUFFER1��BUFFER2�ϳ�����ģ��
unsigned char code FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};											//ɾ��ָ��ģ�������е�ģ��
volatile unsigned char FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};				//��BUFFER1�е��������ŵ�ָ����λ��
//volatile:ϵͳ�������´������ڵ��ڴ��ȡ���ݣ���ʹ��ǰ���ָ��ոմӸô���ȡ������
void Uart_Send_Byte(u8 dat)
{
	SendData(dat);
}
/*------------------ FINGERPRINT������ --------------------------*/
 //���Ͱ�ͷ
void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //��ͷ
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   	
}
////����ָ��
//void FPM10A_Cmd_Check(void)
//{
//	int i=0;
//	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
//	for(i=0;i<10;i++)	
//		Uart_Send_Byte(FPM10A_Get_Device[i]);
//}
//���շ������ݻ���
void FPM10A_Receive_Data()
{
	unsigned int i=1000;
	FPM10A_RECEICE_BUFFER[9]=1;
	uart_rx_sta=0;
	while(i--)
	{
		Delay1ms();
		if(uart_rx_sta>=10)
		{
			delay_ms(10);
			break;
		}
	}
	uart_rx_sta=0;
}

//FINGERPRINT_���ָ��ͼ������
void FPM10A_Cmd_Get_Img(void)
{
  unsigned char i;
  FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<6;i++) //�������� 0x1d
		Uart_Send_Byte(FPM10A_Get_Img[i]);
}
//��ͼ��ת��������������Buffer1��
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ      
  for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
		Uart_Send_Byte(FPM10A_Img_To_Buffer1[i]);
}
//��ͼ��ת��������������Buffer2��
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
  unsigned char i;
	for(i=0;i<6;i++)    //���Ͱ�ͷ
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
  for(i=0;i<7;i++)   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
		Uart_Send_Byte(FPM10A_Img_To_Buffer2[i]);
}
//����ȫ���û�
void FPM10A_Cmd_Search_Finger(void)
{
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<11;i++)
	{
		Uart_Send_Byte(FPM10A_Search[i]);   
	}
}
//ת����������
void FPM10A_Cmd_Reg_Model(void)
{
	unsigned char i;	   
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<6;i++)
		Uart_Send_Byte(FPM10A_Reg_Model[i]);
}
//ɾ��ָ��ģ���������ָ��ģ��
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
	unsigned char i;    
	for(i=0;i<6;i++) //��ͷ
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
	for(i=0;i<6;i++) //����ϲ�ָ��ģ��
		Uart_Send_Byte(FPM10A_Delete_All_Model[i]);   
}
//����ָ��
void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
       unsigned long temp = 0;
		   unsigned char i;
       FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
       FPM10A_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //����У���
		   	   temp = temp + FPM10A_Save_Finger[i]; 
		   FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //���У������
		   FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
       FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ	
       for(i=0;i<9;i++)  
      		Uart_Send_Byte(FPM10A_Save_Finger[i]);      //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
}
//���ָ��
u8 FPM10A_Add_Fingerprint()
{
	FPM10A_Cmd_Get_Img(); 	//���ָ��ͼ��
	FPM10A_Receive_Data();	//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
	if(FPM10A_RECEICE_BUFFER[9]==0)
	{
		delay_ms(200);
		FINGERPRINT_Cmd_Img_To_Buffer1(); //ͼ��ת��������������Buffer1��
		FPM10A_Receive_Data();
		if(FPM10A_RECEICE_BUFFER[9]==0)
		{
			FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
			FPM10A_Receive_Data();//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
			if(FPM10A_RECEICE_BUFFER[9]==0)
			{
				delay_ms(200);
				FINGERPRINT_Cmd_Img_To_Buffer2();
				FPM10A_Receive_Data();
				FPM10A_Cmd_Reg_Model();//ת����������
				FPM10A_Receive_Data(); 
				FPM10A_Cmd_Save_Finger(finger_id);                		         
				FPM10A_Receive_Data();
				finger_id=finger_id+1;
				return 0; 
			}
			else
				return 1; 
		}
		else
			return 2;
	}
	else
		return FPM10A_RECEICE_BUFFER[9]; 
}

//����ָ��
u8 FPM10A_Find_Fingerprint()
{
	unsigned int find_fingerid = 0;
	unsigned char id_show[]={0,0,0};
	FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
	FPM10A_Receive_Data();		
	
	if(FPM10A_RECEICE_BUFFER[9]==0)//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
	{		
		delay_ms(100);
		FINGERPRINT_Cmd_Img_To_Buffer1();
		FPM10A_Receive_Data();		
		FPM10A_Cmd_Search_Finger();
		FPM10A_Receive_Data();			
		if(FPM10A_RECEICE_BUFFER[9] == 0) //������  
		{
			find_fingerid = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];//ƴ��ָ��ID��
			return 0;
		}
		else //û���ҵ�
		{
			return 1;
		}
	}
	else
		return 2;
}
//ɾ�����д�����ָ�ƿ�
u8 FPM10A_Delete_All_Fingerprint()
{
	unsigned char i=0;
	FINGERPRINT_Cmd_Delete_All_Model();
	FPM10A_Receive_Data(); 
	if(FPM10A_RECEICE_BUFFER[9] == 0)					//�ж����ݵ͵�9λ�Ƿ���յ�0
	{
		return 0;
	}
	else
		return 1;
}
//ɾ�� flash ���ݿ���ָ�� ID �ſ�ʼ�� N ��ָ��ģ��
//u8 FPM10A_Delete_N_Fingerprint(unsigned int storeID,unsigned int N)
//{
//	unsigned char i=0;
//	unsigned long temp = 0;
//	unsigned char FPM10A_Delete_Finger[10];
//	FPM10A_Delete_Finger[0]=0x01;
//	FPM10A_Delete_Finger[1]=0x00;
//	FPM10A_Delete_Finger[2]=0x07;
//	FPM10A_Delete_Finger[3]=0x0c;
//	FPM10A_Delete_Finger[4]=(storeID&0xFF00)>>8;
//	FPM10A_Delete_Finger[5]=(storeID&0x00FF);
//	FPM10A_Delete_Finger[6]=(N&0x00FF)>>8;
//	FPM10A_Delete_Finger[7]=(N&0x00FF);
//	for(i=0;i<8;i++)   //����У���
//		 temp = temp + FPM10A_Delete_Finger[i]; 
//	FPM10A_Delete_Finger[8]=(temp & 0x00FF00) >> 8; //���У������
//	FPM10A_Delete_Finger[9]= temp & 0x0000FF;	
//	
//	FPM10A_Cmd_Send_Pack_Head();
//	for(i=0;i<10;i++)
//		Uart_Send_Byte(FPM10A_Delete_Finger[i]);      //��������
//	
//	FPM10A_Receive_Data(); 
//	
//	if(FPM10A_RECEICE_BUFFER[9] == 0)					//�ж����ݵ͵�9λ�Ƿ���յ�0
//	{
//			return 0;
//	}
//	else
//		return 1;
//}
//��ȡָ�Ƹ���
u16 PS_ValidTempleteNum()
{
	unsigned char i;   
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ	
	for(i=0;i<6;i++)
		Uart_Send_Byte(FPM10A_Get_Finger_Num[i]);      //�������� 
	FPM10A_Receive_Data();
	if(FPM10A_RECEICE_BUFFER[9] == 0)					//�ж����ݵ͵�9λ�Ƿ���յ�0
	{
		return (FPM10A_RECEICE_BUFFER[10]*256)+FPM10A_RECEICE_BUFFER[11];  //
	}
	else
		return 65535;
}
////ָ��ģ������
//u8 Device_Check(void)
//{
//	unsigned int i=200;				
//	FPM10A_Cmd_Check();										//��Ƭ����ָ��ģ�鷢��У������
//	FPM10A_RECEICE_BUFFER[9]=1;
//	uart_rx_sta=0;
//	while(i--)
//	{
//		Delay1ms();
//		if(uart_rx_sta>10)
//		{
//			break;
//		}
//	}
//	uart_rx_sta=0;
//	if(FPM10A_RECEICE_BUFFER[9] == 0)			//�ж����ݵ͵�9λ�Ƿ���յ�0
//	{
//		return 0;
//	}
//	else
//		return 1;
//}
//ָ��ģ��ƹ����
//void FPM10A_AuraLedConfig(u8 ctrl,u8 speed,u8 color,u8 freqency) 
//{
//	u16 check;
//	FPM10A_Cmd_Send_Pack_Head();	//��ͷ
//	Uart_Send_Byte(0X01);				//����ʶ  
//	Uart_Send_Byte(0X00);				//�����ȸ�λ
//	Uart_Send_Byte(0X07);				//�����ȵ�λ
//	Uart_Send_Byte(0x3C);				//ָ����
//	Uart_Send_Byte(ctrl);				//״̬
//	Uart_Send_Byte(speed);			//�ٶ�
//	Uart_Send_Byte(color);			//��ɫ
//	Uart_Send_Byte(freqency);		//����
//	check=0x01+0x07+0x3C+ctrl+speed+color+freqency;
//	Uart_Send_Byte(check>>8);
//	Uart_Send_Byte(check);
//}






















