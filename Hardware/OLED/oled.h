//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源（V1.2底板接的是3.3V）
//              SCL   接PC8（SCL）（V1.2底板已接好）
//              SDA   接PC9（SDA）（V1.2底板已接好）            
//              ----------------------------------------------------------------
//修改说明：修改引脚需要修改本文件的第28、29、31、32行，还有 oled.c 中的 OLED_Init()函数
//////////////////////////////////////////////////////////////////////////////////
#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    	
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_8)//SCL     换引脚需要修改此处
#define OLED_SCLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_9)//SDA     换引脚需要修改此处
#define OLED_SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_9)

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//SCL     换引脚需要修改此处
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SDA     换引脚需要修改此处
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据


void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(unsigned char dat);
void OLED_Init(void);
void OLED_Show(void);


/////////0x7A///////改过地址的OLED屏幕使用以下函数
void OLED_ClearPoint1(unsigned char x,unsigned char y);
void OLED_DrawLine1(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void OLED_DrawCircle1(unsigned char x,unsigned char y,unsigned char r);
void OLED_ShowChar1(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1);
void OLED_ShowString1(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1);
void OLED_ShowNum1(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1);
void OLED_ShowChinese1(unsigned char x,unsigned char y,unsigned char num,unsigned char size1);
void OLED_DrawPoint1(unsigned char x,unsigned char y);
void OLED_ColorTurn1(unsigned char i);
void OLED_DisplayTurn1(unsigned char i);
void OLED_WR_Byte1(unsigned char dat,unsigned char mode);
void OLED_DisPlay_On1(void);
void OLED_DisPlay_Off1(void);
void OLED_Refresh1(void);
void OLED_Clear1(void);
void OLED_WR_BP1(unsigned char x,unsigned char y);
void OLED_ShowPicture1(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ScrollDisplay1(unsigned char num,unsigned char space);


/////////0x78///////没改过地址的OLED屏幕使用以下函数
void OLED_ColorTurn(unsigned char i);
void OLED_DisplayTurn(unsigned char i);
void OLED_WR_Byte(unsigned char dat,unsigned char mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_WR_BP(unsigned char x,unsigned char y);
void OLED_ShowPicture(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_ScrollDisplay(unsigned char num,unsigned char space);
void OLED_DrawPoint(unsigned char x,unsigned char y);
void OLED_ClearPoint(unsigned char x,unsigned char y);
void OLED_DrawLine(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void OLED_DrawCircle(unsigned char x,unsigned char y,unsigned char r);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1);
void OLED_ShowString(unsigned char x,unsigned char y,unsigned char *chr,unsigned char size1);
void OLED_ShowNum(unsigned char x,unsigned char y,unsigned int num,unsigned char len,unsigned char size1);
void OLED_ShowChinese(unsigned char x,unsigned char y,unsigned char num,unsigned char size1);



#endif  
	 



