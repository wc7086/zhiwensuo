#include "servo.h"

//u16 angle=0; 	//舵机角度
//u8 cnt;				//中断次数
//#define servo_cnt (((angle*10)/45))/10+10   
//////50us一次中断 舵机周期是20ms 400次中断  0.5ms-2.5ms的占空比对应舵机0-180度角度  10-50 180/40=4.5  放大10倍，就不用浮点计算了
void Delay17800us()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 1;
	j = 192;
	k = 121;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void Delay2200us()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 24;
	j = 167;
	do
	{
		while (--j);
	} while (--i);
}
void Delay800us()			//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 9;
	j = 151;
	do
	{
		while (--j);
	} while (--i);
}
void Delay19200us()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	_nop_();
	i = 1;
	j = 207;
	k = 136;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

/*----------------------------
舵机开门，延时时间
----------------------------*/
//void Door_Open(u16 ntime)
//{
//	servo_pwr=0;
//	angle=27;
//	delay_ms(ntime);
//	servo_pwr=1;
//	beep_work(2,100);
//	delay_ms(ntime);
//	servo_pwr=0;
//	angle=153;
//	delay_ms(ntime);
//	servo_pwr=1;
//}
void Door_Open(u16 ntime)
{
	u16 i;
	servo_pwr=0;
	for(i=0;i<(ntime/20);i++)
	{
		servo=0;
		Delay19200us();
		servo=1;
		Delay800us();
	}
	beep_work(2,100);
	delay_ms(ntime);
	
	for(i=0;i<(ntime/20);i++)
	{
		servo=0;
		Delay17800us();
		servo=1;
		Delay2200us();
	}
	servo_pwr=1;			//关闭舵机电源
}

//void Timer0_Init()
//{
//	AUXR |=  (1<<7);    // Timer0 set as 1T mode
//  TMOD &= ~(1<<2);    // Timer0 set as Timer
//  TMOD &= ~0x03;      // Timer0 set as 16 bits Auto Reload
//	TH0 = (65535-600) / 256;  //Timer0 Load
//	TL0 = (65535-600) % 256;	//50us
//	ET0 = 1;        //Timer0 Interrupt Enable
//	TR0 = 1;        //Timer0 Run
//	EA = 1;         //打开总中断
//}

//void tm0() interrupt 1
//{
//	cnt++;
//	if(servo_cnt>cnt)
//		servo=0;
//	else
//		servo=1;
//	if(cnt==400)
//	{
//		cnt=0;
//	}
//}
	