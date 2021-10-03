#ifndef __KEY_H__
#define __KEY_H__
#include "stc15.h"

sbit key1 = P1^5;
sbit key2 = P5^5;
sbit key3 = P3^3;


u8 Key_Scan();
#endif