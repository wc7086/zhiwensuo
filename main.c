#include "stc15.h"
#include "uart.h"
#include "as608.h"
#include "key.h"
#include "servo.h"
#include "pwr.h"
#include "oled.h"
#include "eeprom.h"

/*
使用本代码，请注明出处
淘宝店铺：可待电子
接51、stm32单片机程序仿真设计
qq：3155913003
*/

u8 key;
u8 weakup_flag=2;
u16 time=0;
/*----------------------------
掉电模式，关闭系统电源 uA待机
----------------------------*/
void Pwr_Off()
{
	
	as608_pwr=1;			//关闭OLED和指纹电源
	
	delay_ms(600);
	
	P1M0 = 0xff;
	P1M1 = 0xff;
	P4M0 = 0xff;
	P4M1 = 0xff;
	P5M0 = 0xff;
	P5M1 = 0xff;
	P3M0 = 0xc3;			//1100 0011
	P3M1 = 0xc3;			//1100 0011
	
	IT0 = 0;          //设置INT0的中断类型为上升沿和下降沿,上升沿和下降沿均可唤醒
	EX0 = 1;          //使能INT0中断
	
  IT1 = 1;          //设置INT1的中断类型 (1:仅下降沿 0:上升沿和下降沿)
  EX1 = 1;          //使能INT1中断
	
	EA=1;
	
	delay_ms(20);
	
	PCON |= 0x02;      //MCU进入掉电模式
	_nop_();          //掉电模式被唤醒后,首先执行此语句,然后再进入中断服务程序
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
显示电池电压
----------------------------*/
void show_volt()
{
	int volt;
	u8 i;
	volt=31916/Pwr_Get();  //3.16V bandGap值101
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
按键触发唤醒后执行任务
----------------------------*/
void key_weak_up_work()  		
{
	u8 i=0,step=0;
	time=2000;
	uart_rx_sta=0;
	as608_pwr=0;							//打开OLED和指纹电源
	delay_ms(10);							//指纹供电10ms以上再初始化串口
	Uart1_Init();							//串口初始化
	while((uart_rx_sta==0)&&(i<200))
	{
		i++;
		delay_ms(1);
	}
	delay_ms(500);
	finger_id=PS_ValidTempleteNum();
	OLED_Init();							//初始化OLED
	show_volt();
	while(time-->0)
	{		
		Delay1ms();
		key=Key_Scan();   //按键按下,时间刷新
		if(key!=0)
			time=5000;
		if(step==0)
		{
			if(key==1)        //放上手指，按key1添加
			{
				OLED_ShowString(0,0,"                ");
				OLED_ShowString(0,2,"                ");
				OLED_ShowCHinese(32,0,0);	//添加用户
				OLED_ShowCHinese(48,0,1);
				OLED_ShowCHinese(64,0,10);
				OLED_ShowCHinese(80,0,11);
				OLED_ShowCHinese(32,2,14);	//请放手指
				OLED_ShowCHinese(48,2,15);
				OLED_ShowCHinese(64,2,16);
				OLED_ShowCHinese(80,2,17);
				for(i=0;i<10;i++)
				{
					delay_ms(100);
					if(FPM10A_Add_Fingerprint()==0)
					{
						OLED_ShowString(0,2,"                ");
						OLED_ShowCHinese(64,0,2);//成功
						OLED_ShowCHinese(80,0,3);
						break;
					}
				}
				if(i==10)
				{
					OLED_ShowString(0,2,"                ");
					OLED_ShowCHinese(64,0,4);//失败
					OLED_ShowCHinese(80,0,5);
				}
				delay_ms(1000);
				step=2;
			}
			if(key==2)				//key2清除所有
			{
				OLED_ShowCHinese(16,0,6);//删除所有指纹
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
			if(key==1)				//删除所有指纹
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
指纹触摸唤醒后执行任务
----------------------------*/
void finger_weakup_work()  	
{
	u8 i=0;
	u8 time = 3; //验证三次
	
	as608_pwr=0;							//打开OLED和指纹电源
	uart_rx_sta=0;
	delay_ms(5);							//指纹供电10ms以上再初始化串口
	Uart1_Init();							//串口初始化
	while((uart_rx_sta==0)&&(i<200))
	{
		i++;
		delay_ms(1);
	}
	
	while(time--)
	{
		if(FPM10A_Find_Fingerprint()==0)
		{
			OLED_Init();							//初始化OLED,需要延时100MS
			show_volt();
			Door_Open(80);
			break;
		}
	}
	
}


/*----------------------------
主函数
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
//	as608_pwr=0;							//打开OLED和指纹电源
//	delay_ms(10);							//指纹供电10ms以上再初始化串口
//	Uart1_Init();							//串口初始化
//	while((uart_rx_sta==0)&&(i<200))
//	{
//		i++;
//		delay_ms(1);
//	}
//	delay_ms(500);
//	
//	finger_id=PS_ValidTempleteNum();
//	OLED_Init();							//初始化OLED
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
		Pwr_Off();						//进入掉电模式，省电，程序不运行
		if(weakup_flag==1)		//key3按键唤醒
			key_weak_up_work();
		if(weakup_flag==0)		//指纹掉电模式唤醒后处理
			finger_weakup_work();
	}
}
/*----------------------------
INT1中断入口，按键key3唤醒
----------------------------*/
void exint1() interrupt 2
{
	weakup_flag=1;		//key3按键唤醒
}
/*----------------------------
INT0中断入口,指纹触摸
----------------------------*/
void exint0() interrupt 0
{
	weakup_flag=0;		//Tch指纹触摸高电平唤醒
}
/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC15F4K60S4 系列 定时器2用作串口1的波特率发生器举例------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966-------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.GXWMCU.com --------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/* 如果要在文章中应用此代码,请在文章中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/