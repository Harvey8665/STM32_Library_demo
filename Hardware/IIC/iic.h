#ifndef __IIC_H
#define __IIC_H
#include "sys.h"
  		   
// IO��������
// ������ϸ�����ý̳̣��ɲο� https://github.com/Harvey8665/STM32_Library_demo/blob/main/IO_set.md �� https://gitee.com/bulijie233/STM32_Library_demo/blob/main/IO_set.md
// �Ĵ�����16λ�ģ�CRH��H��ʾ��8λ���Ǹ�F�������ҷֱ���15 14 13 12 11 10 9 8����GPIO_Pin_n�ͽ���nλ��0������λΪF
// u32��4���ֽڵģ������GPIO_Pin_8�����һλΪ0��������������ţ����� (n-8)*4
// GPIOB->CRH|=(u32)8<<4������Ϊ���ģʽΪ3������Ϊ����ģʽ��Ϊ8
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<4;}

// IO��������
// �˴����޸�ΪPB8��PB9
#define IIC_SCL    PBout(8)                                                   //SCL
#define IIC_SDA    PBout(9)                                                   //SDA	 
#define READ_SDA   PBin(9)                                                    //����SDA 

//IIC���в�������
void IIC_Init(void);                                                           //��ʼ��IIC��IO��				 
void IIC_Start(void);				                                           //����IIC��ʼ�ź�
void IIC_Stop(void);	  			                                           //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			                                           //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);                                           //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				                                           //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					                                           //IIC����ACK�ź�
void IIC_NAck(void);				                                           //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8 iicDevReadByte(u8 devaddr,u8 addr);	                                       /*��һ�ֽ�*/
void iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*дһ�ֽ�*/
void iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*������ȡ����ֽ�*/
void iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*����д�����ֽ�*/

#endif
