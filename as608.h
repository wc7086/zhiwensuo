#ifndef __AS608_H
#define __AS608_H

#include <stdio.h>
#include <string.h>
#include "stc15.h" 
#include "delay.h" 
#include "uart.h" 

extern unsigned int finger_id;
#define PS_Led_red 			0x01
#define PS_Led_green 		0x02
#define PS_Led_yelleo 	0x03
#define PS_Led_blue 		0x04
#define PS_Led_pink 		0x05
#define PS_Led_wirte 		0x07
#define PS_Led_color3 	0x20
#define PS_Led_color7 	0x30

#define PS_Led_breath 	0x01
#define PS_Led_twinkle 	0x02
#define PS_Led_on 	0x03
#define PS_Led_off 	0x04
void FPM10A_Receive_Data();
u8 FPM10A_Delete_All_Fingerprint();
u8 FPM10A_Find_Fingerprint();
u8 FPM10A_Add_Fingerprint();
//void FPM10A_AuraLedConfig(u8 ctrl,u8 speed,u8 color,u8 freqency);  		//光环控制
u8 FPM10A_Delete_N_Fingerprint(unsigned int storeID,unsigned int N);	//删除 flash 数据库中指定 ID 号开始的 N 个指纹模板
u16 PS_ValidTempleteNum();//读取指纹数
#endif



