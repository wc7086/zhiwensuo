#ifndef __OLED_H
#define __OLED_H	
#include "stc15.h"		 
#include "delay.h"	
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define OLED_MODE 0

sbit OLED_SCL=P1^1;//Ê±ÖÓ D0£¨SCLK£
sbit OLED_SDIN=P1^0;//D1£¨MOSI£© Êý¾Ý

#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1

//OLEDÄ£Ê½ÉèÖÃ
//0:4Ïß´®ÐÐÄ£Ê½
//1:²¢ÐÐ8080Ä£Ê½

#define Max_Column	128					  
//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

//OLED¿ØÖÆÓÃº¯Êý
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_Set_Pos(unsigned char x, unsigned char y);

void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
#endif  
	 



