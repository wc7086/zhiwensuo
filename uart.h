#ifndef __UART_H__
#define __UART_H__

#include "stc15.h"
sbit RX=P3^6;
sbit TX=P3^7;

extern u8 uart_rx_sta;         		//接收状态标记	
extern unsigned char FPM10A_RECEICE_BUFFER[16];
void Uart1_Init(void);
void SendData(u8 dat);
//void SendString(char *s);
unsigned char ReceiveData(void);//UART Receive a byteg
#endif