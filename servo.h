#ifndef __SERVO_H__
#define __SERVO_H__
#include "stc15.h"
#include "as608.h"
#include "beep.h"
sbit servo     = P1^3;
sbit servo_pwr = P1^4;

//void Angle_Set(u8 a);
//void Timer0_Init(void);
void Door_Open(u16 ntime);

#endif
