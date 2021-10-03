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
读取从ADC引脚读取3.3V电压数值
用于计算电源电压 256/VCC=ADC3V3_Get()/3.3
避免浮点运算，会占用大量内存
----------------------------*/
u8 ADC3V3_Get()  //
{
	P1ASF = 0xff;                   //设置P1口为AD口
  ADC_RES = 0;                    //清除结果寄存器
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
  Delay(2);                       //ADC上电并延时
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 6 | ADC_START;
	_nop_();                        //等待4个NOP
	_nop_();
	_nop_();
	_nop_();
	while (!(ADC_CONTR & ADC_FLAG));//等待ADC转换完成
	ADC_CONTR &= ~ADC_FLAG;         //Close ADC
	P1ASF = 0x00;                   //不设置P1口为模拟口,则ch0测量第9通道，内部基准电压
	return ADC_RES;                 //返回ADC结果
}