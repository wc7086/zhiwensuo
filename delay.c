#include	"delay.h"
/*----------------------------
º¯Êý£º1msÑÓÊ±
----------------------------*/
void delay_ms(unsigned int ms)
{
	while(ms--)
	{
		Delay1ms();
	}
}

void Delay1ms(void)		//@11.0592MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}