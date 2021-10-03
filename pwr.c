#include "pwr.h"
/*
ADC相关
*/
#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_SPEEDLL 0x00            //540个时钟
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位
/*----------------------------
软件延时
----------------------------*/
void Delay(u8 n)
{
    u8 x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}
/*----------------------------
读取输入电源电压
----------------------------*/
u8 Pwr_Get()  //8位ADC模式
{
	P1ASF = 0x00;                   //不设置P1口为模拟口,则ch0测量第9通道，内部基准电压
  ADC_RES = 0;                    //清除结果寄存器
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
  Delay(2);                       //ADC上电并延时
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 0 | ADC_START;
	_nop_();                        //等待4个NOP
	_nop_();
	_nop_();
	_nop_();
	while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
	ADC_CONTR &= ~ADC_FLAG;         //Close ADC  //ADC_FLAG=1;
	
	
	ADC_CONTR &= 0X7F;  						//ADC_POWER=0，关闭ADC电源降低功耗
	
	return ADC_RES;                 //返回ADC结果
}
