#ifndef __SERVO_H__
#define __SERVO_H__
#include "stc15.h"
#include "as608.h"
sbit servo     = P1^3;
sbit servo_pwr = P1^4;

extern u8 H1,L1,H2,L2; // 定时器模拟PWM两组装载值

void Angle_Set(u8 a);
void Timer0_Init(void);
void Door_Open(u16 ntime);

#endif
