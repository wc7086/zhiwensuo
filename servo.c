#include "servo.h"
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
void Delay800us()		//@11.0592MHz
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
void Door_Open(u16 ntime)
{
	u16 i;
	servo_pwr=0;
	for(i=0;i<ntime;i++)
	{
		servo=0;
		Delay19200us();
		servo=1;
		Delay800us();
	}
	
	delay_ms(ntime*20);
	
	for(i=0;i<ntime;i++)
	{
		servo=0;
		Delay17800us();
		servo=1;
		Delay2200us();
	}
	servo_pwr=1;			//关闭舵机电源
}

	
	