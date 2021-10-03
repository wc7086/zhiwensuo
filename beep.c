
#include "beep.h"

sbit beep = P5^4;
/*-------------------------------
蜂鸣器驱动
参数：次数，蜂鸣时间
--------------------------------*/
void beep_work(u8 cnt,u16 time)
{
	u8 i;
	for(i=0;i<cnt;i++)
	{
		beep=0;
		delay_ms(time);
		beep=1;
		delay_ms(time);
	}
}