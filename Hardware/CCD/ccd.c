#include "ccd.h"
#include "adc.h"	
#include "usart.h"
#include "string.h"
#include "oled.h"
/*
硬件资源:开发板     线性CCD
         5V  —— VDD
         GND   —— GND
         PA1   —— AO1
         PA2   —— CLK    如需修改本引脚，则在下方修改Ccd_Init函数，再到 ccd.h 里修改 TSL_CLK 的定义即可
         PA7   —— SI     如需修改本引脚，则在下方修改Ccd_Init函数，再到 ccd.h 里修改 TSL_SI 的定义即可
*/
#define dynamic_threshold 1    // 此处设置是否启用动态阈值算法，值为1时启用

u8 ccd_adc[128]={0};
u8 SciBuf[200];  //存储上传到上位机的信息
int TIME_us=10; //曝光时间
u8 Flag_Target;
u8 CCD_Zhongzhi,CCD_Yuzhi;   //线性CCD  相关

void Ccd_Init(void)
{ 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 // 声明CLK的引脚，注意，该引脚在RD_TSL疑似手动模拟PWM 2022-8-13
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //PA.2 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.2
 GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PA.2 输出高

 // 声明SI的引脚，可以在此修改引脚的配置
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //PA.7 端口配置, 推挽输出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_7); 						 //PA.7	输出高

}


/******************************************************************************
***
* FUNCTION NAME: void Dly_us(int a) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 延时函数，控制曝光时间 *
* MODIFY DATE : NONE *
* INPUT : int *
* OUTPUT : NONE *
* RETURN : NONE *
*******************************************************************************
**/
void Dly_us(int a)
{
   int ii;    
   for(ii=0;ii<a;ii++);      
}

/******************************************************************************
***
* FUNCTION NAME: RD_TSL(void) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 按照时序依次读取CCD输出的模拟电压值 *
* 函数功能：CCD数据采集 *
* MODIFY DATE : NONE *
* INPUT : void *
* OUTPUT : NONE *
* RETURN : NONE *
  
*******************************************************************************
**/
  void RD_TSL(void) 
	{
		u8 i=0,tslp=0;
		TSL_CLK=1;     //CLK引脚设为高电平          
		TSL_SI=0; 
		Dly_us(TIME_us);   //延时曝光时间
				
		TSL_SI=1; 
		TSL_CLK=0;
		Dly_us(TIME_us);
				
		TSL_CLK=1;
		TSL_SI=0;
		Dly_us(TIME_us); 
		for(i=0;i<128;i++)
		{ 
			TSL_CLK=0; 
			Dly_us(TIME_us);  //调节曝光时间
			ccd_adc[tslp]=(u8)((float)Get_Adc(ADC_Channel_1)/4096*255);  //将读取到的电压值存入数组中
			++tslp;
			TSL_CLK=1;
			Dly_us(TIME_us);
		}  
	}

/**************************************************************************
函数功能：线性CCD取中值
入口参数：无
返回  值：无
**************************************************************************/
void  Find_CCD_Zhongzhi(void)
{ 
	 static u16 i,j,Left,Right;
	#if dynamic_threshold
	 static u16 value1_max,value1_min;
	
	   value1_max=ccd_adc[0];  //动态阈值算法，读取最大和最小值
     for(i=5;i<123;i++)   //两边各去掉5个点
     {
        if(value1_max<=ccd_adc[i])
        value1_max=ccd_adc[i];
     }
	   value1_min=ccd_adc[0];  //最小值
     for(i=5;i<123;i++) 
     {
        if(value1_min>=ccd_adc[i])
        value1_min=ccd_adc[i];
     }
   CCD_Yuzhi=(value1_max+value1_min)/2;	  //计算出本次中线提取的阈值
	#else
	 CCD_Yuzhi=25;  // 设置固定的阈值
	#endif
	 printf("阈值：%d  ",CCD_Yuzhi);
	 for(i = 5;i<118; i++)   //寻找左边跳变沿
	{
		if(ccd_adc[i]>CCD_Yuzhi&&ccd_adc[i+1]>CCD_Yuzhi&&ccd_adc[i+2]>CCD_Yuzhi&&ccd_adc[i+3]<CCD_Yuzhi&&ccd_adc[i+4]<CCD_Yuzhi&&ccd_adc[i+5]<CCD_Yuzhi)
		{	
			Left=i;
			break;	
		}
	}
	 for(j = 118;j>5; j--)//寻找右边跳变沿
  {
		if(ccd_adc[j]<CCD_Yuzhi&&ccd_adc[j+1]<CCD_Yuzhi&&ccd_adc[j+2]<CCD_Yuzhi&&ccd_adc[j+3]>CCD_Yuzhi&&ccd_adc[j+4]>CCD_Yuzhi&&ccd_adc[j+5]>CCD_Yuzhi)
		{	
		  Right=j;
		  break;	
		}
  }
	CCD_Zhongzhi=(Right+Left)/2;//计算中线位置
	printf("中值：%d\r\n",CCD_Zhongzhi);
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>90)   //计算中线的偏差，如果太大
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //则取上一次的值
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //保存上一次的偏差
}

/*

**************************以下函数需要配合上位机使用*****************************

*/

/******************************************************************************
***
* FUNCTION NAME: void slove_data(void) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 将从CCD中读取到的信息按照上位机要求的通信协议存入到数组中以待发送 *
* MODIFY DATE : NONE *
* INPUT : void *
* OUTPUT : NONE *
* RETURN : NONE *
*******************************************************************************
**/ 
	 void slove_data(void)
	 {
		int i;
		RD_TSL();
    SciBuf[0] = 0; 
	  SciBuf[1] = 132;
    SciBuf[2] = 0; 
    SciBuf[3] = 0;
	  SciBuf[4] = 0;
    SciBuf[5] = 0; 
		for(i=0;i<128;i++)
			SciBuf[6+i] = ccd_adc[i];
	 }
/******************************************************************************
***
* FUNCTION NAME: void sendToPc(void) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 将待发送的信息通过串口发送至上位机*
* MODIFY DATE : NONE *
* INPUT : void *
* OUTPUT : NONE *
* RETURN : NONE *
*******************************************************************************
**/ 	 	 
	 void sendToPc(void)
	 { 
		 int i;
		 slove_data();
		 printf("*");
		 printf("LD");
		 for(i=2;i<134;i++)
		 { 
				printf("%c",SciBuf[i]); //以字符形式发送高4位对应的16进制
				//printf("%c",binToHex_low(SciBuf[i]));  //以字符形式发送低4位对应的16进制
		 }
		 printf("00");   //通信协议要求
		 printf("#");    //通信协议要求
	 }
	 
/******************************************************************************
***
* FUNCTION NAME: binToHex_low(u8 num) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 将二进制低8位转换16进制*
* MODIFY DATE : NONE *
* INPUT : u8 *
* OUTPUT : NONE *
* RETURN : char *
*******************************************************************************
**/ 	 	 
 char binToHex_low(u8 num)
 {u8 low_four;
	 low_four=num&0x0f;
	 if(low_four==0)
		 return('0');
	 else if(low_four==1)
		  return('1');
	 else if(low_four==2)
		  return('2');
	 else if(low_four==3)
		  return('3');
	 else if(low_four==4)
		  return('4');
	 else if(low_four==5)
		  return('5');
	 else if(low_four==6)
		  return('6');
	 else if(low_four==7)
		  return('7');
	 else if(low_four==8)
		  return('8');
	 else if(low_four==9)
		  return('9');
	 else if(low_four==10)
		  return('A');
	 else if(low_four==11)
		  return('B');
	 else if(low_four==12)
		  return('C');
	 else if(low_four==13)
		  return('D');
	 else if(low_four==14)
		  return('E');
	 else if(low_four==15)
		  return('F');
	 else
		 return(' ');
 }
 
/******************************************************************************
***
* FUNCTION NAME: binToHex_low(u8 num) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : 将二进制高8位转换16进制*
* MODIFY DATE : NONE *
* INPUT : u8 *
* OUTPUT : NONE *
* RETURN : char *
*******************************************************************************
**/ 						 
 char binToHex_high(u8 num)
{
		u8 high_four;
		high_four=(num>>4)&0x0f;
		if(high_four==0)
			return('0');
				else if(high_four==1)
					return('1');
					else if(high_four==2)
							return('2');
							else if(high_four==3)
								return('3');
								else if(high_four==4)
								return('4');
									else if(high_four==5)
									return('5');
										else if(high_four==6)
											return('6');
											else if(high_four==7)
											return('7');
												else if(high_four==8)
												return('8');
													else if(high_four==9)
														return('9');
														else if(high_four==10)
															return('A');
															else if(high_four==11)
																return('B');
																else if(high_four==12)
																	return('C');
																	else if(high_four==13)
																		return('D');
																		else if(high_four==14)
																			return('E');
																			else if(high_four==15)
																				return('F');
																			  else
		                                      return(' ');
}



