#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
  		   
// IO方向设置
// 更多详细的设置教程，可参考 https://github.com/Harvey8665/STM32_Library_demo/blob/main/IO_set.md 或 https://gitee.com/bulijie233/STM32_Library_demo/blob/main/IO_set.md
// 寄存器是16位的，CRH中H表示高8位，那个F从左往右分别是15 14 13 12 11 10 9 8，用GPIO_Pin_n就将第n位置0，其他位为F
// u32是4个字节的，如果是GPIO_Pin_8则最后一位为0，如果是其他引脚，则是 (n-8)*4
// GPIOB->CRH|=(u32)8<<4，设置为输出模式为3，设置为输入模式则为8
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<4;}

// IO操作函数
// 此处已修改为PB8、PB9
#define IIC_SCL    PBout(8)                                                   //SCL
#define IIC_SDA    PBout(9)                                                   //SDA	 
#define READ_SDA   PBin(9)                                                    //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                                                           //初始化IIC的IO口				 
void IIC_Start(void);				                                           //发送IIC开始信号
void IIC_Stop(void);	  			                                           //发送IIC停止信号
void IIC_Send_Byte(u8 txd);			                                           //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);                                           //IIC读取一个字节
u8 IIC_Wait_Ack(void); 				                                           //IIC等待ACK信号
void IIC_Ack(void);					                                           //IIC发送ACK信号
void IIC_NAck(void);				                                           //IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 iicDevReadByte(u8 devaddr,u8 addr);	                                       /*读一字节*/
void iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*写一字节*/
void iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*连续读取多个字节*/
void iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*连续写入多个字节*/

#endif
