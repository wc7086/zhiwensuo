#include "stc15.h"
#include "uart.h"
#include "as608.h"
#include "key.h"
#include "servo.h"
#include "pwr.h"
#include "oled.h"
#include "eeprom.h"

/*
ʹ�ñ����룬��ע������
�Ա����̣��ɴ�����
��51��stm32��Ƭ������������
qq��3155913003
*/

u8 key;
u8 weakup_flag=2;
u16 time=0;
/*----------------------------
����ģʽ���ر�ϵͳ��Դ uA����
----------------------------*/
void Pwr_Off()
{
	
	as608_pwr=1;			//�ر�OLED��ָ�Ƶ�Դ
	
	delay_ms(600);
	
	P1M0 = 0xff;
	P1M1 = 0xff;
	P4M0 = 0xff;
	P4M1 = 0xff;
	P5M0 = 0xff;
	P5M1 = 0xff;
	P3M0 = 0xc3;			//1100 0011
	P3M1 = 0xc3;			//1100 0011
	
	IT0 = 0;          //����INT0���ж�����Ϊ�����غ��½���,�����غ��½��ؾ��ɻ���
	EX0 = 1;          //ʹ��INT0�ж�
	
  IT1 = 1;          //����INT1���ж����� (1:���½��� 0:�����غ��½���)
  EX1 = 1;          //ʹ��INT1�ж�
	
	EA=1;
	
	delay_ms(20);
	
	PCON |= 0x02;      //MCU�������ģʽ
	_nop_();          //����ģʽ�����Ѻ�,����ִ�д����,Ȼ���ٽ����жϷ������
	_nop_();
	_nop_();          
	_nop_();
	P1M0 = 0x00;
	P1M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
}
/*----------------------------
��ʾ��ص�ѹ
----------------------------*/
void show_volt()
{
	int volt;
	u8 i;
	volt=31916/Pwr_Get();  //3.16V bandGapֵ101
	volt=volt*2-720;
	if(volt>99)
		volt=99;
	if(volt<0)
		volt=0;
	i=32;
	OLED_ShowCHinese(i,2,12);
	i+=16;
	OLED_ShowCHinese(i,2,13);
	i+=16;
	OLED_ShowChar(i,2,':');
	i+=8;
	OLED_ShowChar(i,2,volt %100/10	+0x30);
	i+=8;
	OLED_ShowChar(i,2,volt	%10		  +0x30);
	i+=8;
	OLED_ShowChar(i,2,'%');
}
/*----------------------------
�����������Ѻ�ִ������
----------------------------*/
void key_weak_up_work()  		
{
	u8 i=0,step=0;
	time=2000;
	uart_rx_sta=0;
	as608_pwr=0;							//��OLED��ָ�Ƶ�Դ
	delay_ms(10);							//ָ�ƹ���10ms�����ٳ�ʼ������
	Uart1_Init();							//���ڳ�ʼ��
	while((uart_rx_sta==0)&&(i<200))
	{
		i++;
		delay_ms(1);
	}
	delay_ms(500);
	finger_id=PS_ValidTempleteNum();
	OLED_Init();							//��ʼ��OLED
	show_volt();
	while(time-->0)
	{		
		Delay1ms();
		key=Key_Scan();   //��������,ʱ��ˢ��
		if(key!=0)
			time=5000;
		if(step==0)
		{
			if(key==1)        //������ָ����key1���
			{
				OLED_ShowString(0,0,"                ");
				OLED_ShowString(0,2,"                ");
				OLED_ShowCHinese(32,0,0);	//����û�
				OLED_ShowCHinese(48,0,1);
				OLED_ShowCHinese(64,0,10);
				OLED_ShowCHinese(80,0,11);
				OLED_ShowCHinese(32,2,14);	//�����ָ
				OLED_ShowCHinese(48,2,15);
				OLED_ShowCHinese(64,2,16);
				OLED_ShowCHinese(80,2,17);
				for(i=0;i<10;i++)
				{
					delay_ms(100);
					if(FPM10A_Add_Fingerprint()==0)
					{
						OLED_ShowString(0,2,"                ");
						OLED_ShowCHinese(64,0,2);//�ɹ�
						OLED_ShowCHinese(80,0,3);
						break;
					}
				}
				if(i==10)
				{
					OLED_ShowString(0,2,"                ");
					OLED_ShowCHinese(64,0,4);//ʧ��
					OLED_ShowCHinese(80,0,5);
				}
				delay_ms(1000);
				step=2;
			}
			if(key==2)				//key2�������
			{
				OLED_ShowCHinese(16,0,6);//ɾ������ָ��
				OLED_ShowCHinese(32,0,7);
				OLED_ShowCHinese(48,0,8);
				OLED_ShowCHinese(64,0,9);
				OLED_ShowCHinese(80,0,10);
				OLED_ShowCHinese(96,0,11);
				OLED_ShowString(0,2," Yes?       No? ");
				step=1;
				
			}
			if(key==3)
				break;
		}

		if(step==1)
		{
			if(key==1)				//ɾ������ָ��
			{
				OLED_ShowString(0,0,"                ");
				OLED_ShowString(0,2,"                ");
				FPM10A_Delete_All_Fingerprint();
				for(i=3;i<13;i++)
				{
					OLED_ShowChar(i*8-8,2,'=');
					OLED_ShowChar(i*8,2,'>');
					delay_ms(200);
				}
				step=2;
				finger_id=0;
			}
			if(key==3)
			{
				step=2;
			}
		}

		if(step==2)
		{	
			step=0;
			OLED_ShowString(0,0,"                ");
			OLED_ShowString(0,2,"                ");
			
			OLED_ShowCHinese(32,0,10);
			OLED_ShowCHinese(48,0,11);
			OLED_ShowChar(64,0,':');
			OLED_ShowChar(72,0,finger_id/100+0x30);
			OLED_ShowChar(80,0,finger_id%100/10+0x30);
			OLED_ShowChar(88,0,finger_id%10+0x30);
			show_volt();
		}
	}
}
/*----------------------------
ָ�ƴ������Ѻ�ִ������
----------------------------*/
void finger_weakup_work()  	
{
	u8 i=0;
	u8 time = 3; //��֤����
	
	as608_pwr=0;							//��OLED��ָ�Ƶ�Դ
	uart_rx_sta=0;
	delay_ms(5);							//ָ�ƹ���10ms�����ٳ�ʼ������
	Uart1_Init();							//���ڳ�ʼ��
	while((uart_rx_sta==0)&&(i<200))
	{
		i++;
		delay_ms(1);
	}
	
	while(time--)
	{
		if(FPM10A_Find_Fingerprint()==0)
		{
			OLED_Init();							//��ʼ��OLED,��Ҫ��ʱ100MS
			show_volt();
			Door_Open(80);
			break;
		}
	}
	
}


/*----------------------------
������
----------------------------*/
void main()
{ 		
//	u8 i,str[]={0xef,0x01,0xff,0xff,0xff,0xff,0x01,0x00,0x03,0xaa,0x00,0xae};
	P1M0 = 0x00;
	P1M1 = 0x00;
	P3M0 = 0x00;
	P3M1 = 0x00;
	P4M0 = 0x00;
	P4M1 = 0x00;
	P5M0 = 0x00;
	P5M1 = 0x00;
//	uart_rx_sta=0;
//	as608_pwr=0;							//��OLED��ָ�Ƶ�Դ
//	delay_ms(10);							//ָ�ƹ���10ms�����ٳ�ʼ������
//	Uart1_Init();							//���ڳ�ʼ��
//	while((uart_rx_sta==0)&&(i<200))
//	{
//		i++;
//		delay_ms(1);
//	}
//	delay_ms(500);
//	
//	finger_id=PS_ValidTempleteNum();
//	OLED_Init();							//��ʼ��OLED
//	
//	FPM10A_RECEICE_BUFFER[9]=1;
//	for(i=0;i<12;i++)
//		SendData(str[i]);
//	FPM10A_Receive_Data();
//	delay_ms(500);
//	if(FPM10A_RECEICE_BUFFER[9]==0)
//		show_volt();
//		while(1);
	while(1)
	{
		Pwr_Off();						//�������ģʽ��ʡ�磬��������
		if(weakup_flag==1)		//key3��������
			key_weak_up_work();
		if(weakup_flag==0)		//ָ�Ƶ���ģʽ���Ѻ���
			finger_weakup_work();
	}
}
/*----------------------------
INT1�ж���ڣ�����key3����
----------------------------*/
void exint1() interrupt 2
{
	weakup_flag=1;		//key3��������
}
/*----------------------------
INT0�ж����,ָ�ƴ���
----------------------------*/
void exint0() interrupt 0
{
	weakup_flag=0;		//Tchָ�ƴ����ߵ�ƽ����
}
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 ϵ�� ��ʱ��2��������1�Ĳ����ʷ���������------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* ���Ҫ�ڳ�����ʹ�ô˴���,���ڳ�����ע��ʹ����STC�����ϼ�����        */
/* ���Ҫ��������Ӧ�ô˴���,����������ע��ʹ����STC�����ϼ�����        */
/*---------------------------------------------------------------------*/