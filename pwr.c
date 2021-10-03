#include "pwr.h"
/*
ADC���
*/
#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_SPEEDLL 0x00            //540��ʱ��
#define ADC_FLAG    0x10            //ADC��ɱ�־
#define ADC_START   0x08            //ADC��ʼ����λ
/*----------------------------
�����ʱ
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
��ȡ�����Դ��ѹ
----------------------------*/
u8 Pwr_Get()  //8λADCģʽ
{
	P1ASF = 0x00;                   //������P1��Ϊģ���,��ch0������9ͨ�����ڲ���׼��ѹ
  ADC_RES = 0;                    //�������Ĵ���
  ADC_CONTR = ADC_POWER | ADC_SPEEDLL;
  Delay(2);                       //ADC�ϵ粢��ʱ
	ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 0 | ADC_START;
	_nop_();                        //�ȴ�4��NOP
	_nop_();
	_nop_();
	_nop_();
	while (!(ADC_CONTR & ADC_FLAG));//�ȴ�ADCת�����
	ADC_CONTR &= ~ADC_FLAG;         //Close ADC  //ADC_FLAG=1;
	
	
	ADC_CONTR &= 0X7F;  						//ADC_POWER=0���ر�ADC��Դ���͹���
	
	return ADC_RES;                 //����ADC���
}
