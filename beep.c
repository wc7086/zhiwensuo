
#include "beep.h"

sbit beep = P5^4;
/*-------------------------------
����������
����������������ʱ��
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