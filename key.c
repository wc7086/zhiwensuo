#include "key.h"

u8 Key_Scan()
{
	static u8 key_up,key_num;		//ËÉÊÖ¼ì²â
	key_num=0;
	if(((key1==0)||(key2==0)||(key3==0))&&(key_up==1) )
	{
		key_up=0;
		delay_ms(10);		//Èí¼şÏû¶¶
		if(key1==0)
			key_num = 1;
		else if(key2==0)
			key_num = 2;
		else if(key3==0)
			key_num = 3;
	}
	else if((key1==1)&&(key2==1)&&(key3==1))
	{
		key_up=1;
	}
	return key_num;
}

