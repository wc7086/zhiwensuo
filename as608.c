#include "as608.h"

unsigned int finger_id = 0;
//FINGERPRINT通信协议定义
unsigned char code FPM10A_Get_Finger_Num[6] ={0x01,0x00,0x03,0x1d,0x00,0x21};												//获得模版总数
//unsigned char code FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};			//口令验证
unsigned char code FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  													//协议包头
unsigned char code FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    													//获得指纹图像
unsigned char code FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; 			//搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索
unsigned char code FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; 									//将图像放入到BUFFER1
unsigned char code FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; 									//将图像放入到BUFFER2
unsigned char code FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; 														//将BUFFER1跟BUFFER2合成特征模版
unsigned char code FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0d,0x00,0x11};											//删除指纹模块里所有的模版
volatile unsigned char FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};				//将BUFFER1中的特征码存放到指定的位置
//volatile:系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据
void Uart_Send_Byte(u8 dat)
{
	SendData(dat);
}
/*------------------ FINGERPRINT命令字 --------------------------*/
 //发送包头
void FPM10A_Cmd_Send_Pack_Head(void)
{
	int i;	
	for(i=0;i<6;i++) //包头
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   	
}
////发送指令
//void FPM10A_Cmd_Check(void)
//{
//	int i=0;
//	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
//	for(i=0;i<10;i++)	
//		Uart_Send_Byte(FPM10A_Get_Device[i]);
//}
//接收反馈数据缓冲
void FPM10A_Receive_Data()
{
	unsigned int i=1000;
	FPM10A_RECEICE_BUFFER[9]=1;
	uart_rx_sta=0;
	while(i--)
	{
		Delay1ms();
		if(uart_rx_sta>=10)
		{
			delay_ms(10);
			break;
		}
	}
	uart_rx_sta=0;
}

//FINGERPRINT_获得指纹图像命令
void FPM10A_Cmd_Get_Img(void)
{
  unsigned char i;
  FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<6;i++) //发送命令 0x1d
		Uart_Send_Byte(FPM10A_Get_Img[i]);
}
//讲图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void)
{
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头      
  for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
		Uart_Send_Byte(FPM10A_Img_To_Buffer1[i]);
}
//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void)
{
  unsigned char i;
	for(i=0;i<6;i++)    //发送包头
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
  for(i=0;i<7;i++)   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
		Uart_Send_Byte(FPM10A_Img_To_Buffer2[i]);
}
//搜索全部用户
void FPM10A_Cmd_Search_Finger(void)
{
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<11;i++)
	{
		Uart_Send_Byte(FPM10A_Search[i]);   
	}
}
//转换成特征码
void FPM10A_Cmd_Reg_Model(void)
{
	unsigned char i;	   
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<6;i++)
		Uart_Send_Byte(FPM10A_Reg_Model[i]);
}
//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void)
{
	unsigned char i;    
	for(i=0;i<6;i++) //包头
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
	for(i=0;i<6;i++) //命令合并指纹模版
		Uart_Send_Byte(FPM10A_Delete_All_Model[i]);   
}
//保存指纹
void FPM10A_Cmd_Save_Finger( unsigned int storeID )
{
       unsigned long temp = 0;
		   unsigned char i;
       FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
       FPM10A_Save_Finger[6] = (storeID&0x00FF);
		   for(i=0;i<7;i++)   //计算校验和
		   	   temp = temp + FPM10A_Save_Finger[i]; 
		   FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
		   FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
       FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头	
       for(i=0;i<9;i++)  
      		Uart_Send_Byte(FPM10A_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
}
//添加指纹
u8 FPM10A_Add_Fingerprint()
{
	FPM10A_Cmd_Get_Img(); 	//获得指纹图像
	FPM10A_Receive_Data();	//判断接收到的确认码,等于0指纹获取成功
	if(FPM10A_RECEICE_BUFFER[9]==0)
	{
		delay_ms(200);
		FINGERPRINT_Cmd_Img_To_Buffer1(); //图像转换成特征码存放在Buffer1中
		FPM10A_Receive_Data();
		if(FPM10A_RECEICE_BUFFER[9]==0)
		{
			FPM10A_Cmd_Get_Img(); //获得指纹图像
			FPM10A_Receive_Data();//判断接收到的确认码,等于0指纹获取成功
			if(FPM10A_RECEICE_BUFFER[9]==0)
			{
				delay_ms(200);
				FINGERPRINT_Cmd_Img_To_Buffer2();
				FPM10A_Receive_Data();
				FPM10A_Cmd_Reg_Model();//转换成特征码
				FPM10A_Receive_Data(); 
				FPM10A_Cmd_Save_Finger(finger_id);                		         
				FPM10A_Receive_Data();
				finger_id=finger_id+1;
				return 0; 
			}
			else
				return 1; 
		}
		else
			return 2;
	}
	else
		return FPM10A_RECEICE_BUFFER[9]; 
}

//搜索指纹
u8 FPM10A_Find_Fingerprint()
{
	unsigned int find_fingerid = 0;
	unsigned char id_show[]={0,0,0};
	FPM10A_Cmd_Get_Img(); //获得指纹图像
	FPM10A_Receive_Data();		
	
	if(FPM10A_RECEICE_BUFFER[9]==0)//判断接收到的确认码,等于0指纹获取成功
	{		
		delay_ms(100);
		FINGERPRINT_Cmd_Img_To_Buffer1();
		FPM10A_Receive_Data();		
		FPM10A_Cmd_Search_Finger();
		FPM10A_Receive_Data();			
		if(FPM10A_RECEICE_BUFFER[9] == 0) //搜索到  
		{
			find_fingerid = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];//拼接指纹ID数
			return 0;
		}
		else //没有找到
		{
			return 1;
		}
	}
	else
		return 2;
}
//删除所有存贮的指纹库
u8 FPM10A_Delete_All_Fingerprint()
{
	unsigned char i=0;
	FINGERPRINT_Cmd_Delete_All_Model();
	FPM10A_Receive_Data(); 
	if(FPM10A_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
	{
		return 0;
	}
	else
		return 1;
}
//删除 flash 数据库中指定 ID 号开始的 N 个指纹模板
//u8 FPM10A_Delete_N_Fingerprint(unsigned int storeID,unsigned int N)
//{
//	unsigned char i=0;
//	unsigned long temp = 0;
//	unsigned char FPM10A_Delete_Finger[10];
//	FPM10A_Delete_Finger[0]=0x01;
//	FPM10A_Delete_Finger[1]=0x00;
//	FPM10A_Delete_Finger[2]=0x07;
//	FPM10A_Delete_Finger[3]=0x0c;
//	FPM10A_Delete_Finger[4]=(storeID&0xFF00)>>8;
//	FPM10A_Delete_Finger[5]=(storeID&0x00FF);
//	FPM10A_Delete_Finger[6]=(N&0x00FF)>>8;
//	FPM10A_Delete_Finger[7]=(N&0x00FF);
//	for(i=0;i<8;i++)   //计算校验和
//		 temp = temp + FPM10A_Delete_Finger[i]; 
//	FPM10A_Delete_Finger[8]=(temp & 0x00FF00) >> 8; //存放校验数据
//	FPM10A_Delete_Finger[9]= temp & 0x0000FF;	
//	
//	FPM10A_Cmd_Send_Pack_Head();
//	for(i=0;i<10;i++)
//		Uart_Send_Byte(FPM10A_Delete_Finger[i]);      //发送命令
//	
//	FPM10A_Receive_Data(); 
//	
//	if(FPM10A_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
//	{
//			return 0;
//	}
//	else
//		return 1;
//}
//读取指纹个数
u16 PS_ValidTempleteNum()
{
	unsigned char i;   
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头	
	for(i=0;i<6;i++)
		Uart_Send_Byte(FPM10A_Get_Finger_Num[i]);      //发送命令 
	FPM10A_Receive_Data();
	if(FPM10A_RECEICE_BUFFER[9] == 0)					//判断数据低第9位是否接收到0
	{
		return (FPM10A_RECEICE_BUFFER[10]*256)+FPM10A_RECEICE_BUFFER[11];  //
	}
	else
		return 65535;
}
////指纹模块握手
//u8 Device_Check(void)
//{
//	unsigned int i=200;				
//	FPM10A_Cmd_Check();										//单片机向指纹模块发送校对命令
//	FPM10A_RECEICE_BUFFER[9]=1;
//	uart_rx_sta=0;
//	while(i--)
//	{
//		Delay1ms();
//		if(uart_rx_sta>10)
//		{
//			break;
//		}
//	}
//	uart_rx_sta=0;
//	if(FPM10A_RECEICE_BUFFER[9] == 0)			//判断数据低第9位是否接收到0
//	{
//		return 0;
//	}
//	else
//		return 1;
//}
//指纹模块灯光控制
//void FPM10A_AuraLedConfig(u8 ctrl,u8 speed,u8 color,u8 freqency) 
//{
//	u16 check;
//	FPM10A_Cmd_Send_Pack_Head();	//包头
//	Uart_Send_Byte(0X01);				//包标识  
//	Uart_Send_Byte(0X00);				//包长度高位
//	Uart_Send_Byte(0X07);				//包长度低位
//	Uart_Send_Byte(0x3C);				//指令码
//	Uart_Send_Byte(ctrl);				//状态
//	Uart_Send_Byte(speed);			//速度
//	Uart_Send_Byte(color);			//颜色
//	Uart_Send_Byte(freqency);		//次数
//	check=0x01+0x07+0x3C+ctrl+speed+color+freqency;
//	Uart_Send_Byte(check>>8);
//	Uart_Send_Byte(check);
//}






















