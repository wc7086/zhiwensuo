#include "uart.h"

/*
Uart相关
*/
#define FOSC 11059200L          //系统频率
#define BAUD 115200             	//串口波特率

#define PARITYBIT 0   //定义校验位

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

bit busy;

u8 uart_rx_sta;         				//接收状态标记	
unsigned char FPM10A_RECEICE_BUFFER[16];
/*----------------------------
串口初始化
----------------------------*/
void Uart1_Init()
{
//	ACC = P_SW1;
//	ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//	P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)

  ACC = P_SW1;
  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
  P_SW1 = ACC;  
//  
//  ACC = P_SW1;
//  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=1
//  ACC |= S1_S1;               //(P1.6/RxD_3, P1.7/TxD_3)
//  P_SW1 = ACC;  

	SCON = 0x50;               	 			//8位可变波特率
	T2L = (65536 - (FOSC/4/BAUD));   	//设置波特率重装值
	T2H = (65536 - (FOSC/4/BAUD))>>8;
	AUXR = 0x14;               				//T2为1T模式, 并启动定时器2
	AUXR |= 0x01;               			//选择定时器2为串口1的波特率发生器
	ES = 1;                     			//使能串口1中断
	EA = 1;
}
/*----------------------------
UART 中断服务程序
-----------------------------*/
void Uart() interrupt 4
{
    if (RI)
    {
        RI = 0;                 //清除RI位
        FPM10A_RECEICE_BUFFER[uart_rx_sta++] = SBUF;              //P0显示串口数据
    }
    if (TI)
    {
        TI = 0;                 //清除TI位
        busy = 0;               //清忙标志
    }
}

/*----------------------------
发送串口数据
----------------------------*/
void SendData(u8 dat)
{
    while (busy);               //等待前面的数据发送完成
    busy = 1;
    SBUF = dat;                 //写数据到UART数据寄存器
}

/*----------------------------
发送字符串
----------------------------*/
//void SendString(char *s)
//{
//    while (*s)                  //检测字符串结束标志
//    {
//        SendData(*s++);         //发送当前字符
//    }
//}
