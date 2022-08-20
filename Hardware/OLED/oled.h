//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//              ˵��: 
//              ----------------------------------------------------------------
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ��V1.2�װ�ӵ���3.3V��
//              SCL   ��PC8��SCL����V1.2�װ��ѽӺã�
//              SDA   ��PC9��SDA����V1.2�װ��ѽӺã�            
//              ----------------------------------------------------------------
//�޸�˵�����޸�������Ҫ�޸ı��ļ��ĵ�28��29��31��32�У����� oled.c �е� OLED_Init()����
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
//-----------------OLED IIC�˿ڶ���----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_8)//SCL     ��������Ҫ�޸Ĵ˴�
#define OLED_SCLK_Set() GPIO_SetBits(GPIOC,GPIO_Pin_8)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOC,GPIO_Pin_9)//SDA     ��������Ҫ�޸Ĵ˴�
#define OLED_SDIN_Set() GPIO_SetBits(GPIOC,GPIO_Pin_9)

//#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//SCL     ��������Ҫ�޸Ĵ˴�
//#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_4)

//#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SDA     ��������Ҫ�޸Ĵ˴�
//#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

 		     
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(unsigned char dat);
void OLED_Init(void);
void OLED_Show(void);


/////////0x7A///////�Ĺ���ַ��OLED��Ļʹ�����º���
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


/////////0x78///////û�Ĺ���ַ��OLED��Ļʹ�����º���
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
	 



