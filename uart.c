#include "uart.h"

/*
Uart���
*/
#define FOSC 11059200L          //ϵͳƵ��
#define BAUD 115200             	//���ڲ�����

#define PARITYBIT 0   //����У��λ

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

bit busy;

u8 uart_rx_sta;         				//����״̬���	
unsigned char FPM10A_RECEICE_BUFFER[16];
/*----------------------------
���ڳ�ʼ��
----------------------------*/
void Uart1_Init()
{
//	ACC = P_SW1;
//	ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//	P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)

  ACC = P_SW1;
  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	SCON = 0x50;               	 			//8λ�ɱ䲨����
	T2L = (65536 - (FOSC/4/BAUD));   	//���ò�������װֵ
	T2H = (65536 - (FOSC/4/BAUD))>>8;
	AUXR = 0x14;               				//T2Ϊ1Tģʽ, ��������ʱ��2
	AUXR |= 0x01;               			//ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
	ES = 1;                     			//ʹ�ܴ���1�ж�
	EA = 1;
}
/*----------------------------
UART �жϷ������
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //���RIλ
        FPM10A_RECEICE_BUFFER[uart_rx_sta++] = SBUF;              //P0��ʾ��������
    }
    if (TI)
    {
        TI = 0;                 //���TIλ
        busy = 0;               //��æ��־
    }
}

/*----------------------------
���ʹ�������
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //�ȴ�ǰ������ݷ������
    busy = 1;
    SBUF = dat;                 //д���ݵ�UART���ݼĴ���
}

/*----------------------------
�����ַ���
----------------------------*/
//void SendString(char *s)
//{
//    while (*s)                  //����ַ���������־
//    {
//        SendData(*s++);         //���͵�ǰ�ַ�
//    }
//}
