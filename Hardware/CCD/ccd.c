#include "ccd.h"
#include "adc.h"	
#include "usart.h"
#include "string.h"
#include "oled.h"
/*
Ӳ����Դ:������     ����CCD
         5V  ���� VDD
         GND   ���� GND
         PA1   ���� AO1
         PA2   ���� CLK    �����޸ı����ţ������·��޸�Ccd_Init�������ٵ� ccd.h ���޸� TSL_CLK �Ķ��弴��
         PA7   ���� SI     �����޸ı����ţ������·��޸�Ccd_Init�������ٵ� ccd.h ���޸� TSL_SI �Ķ��弴��
*/
#define dynamic_threshold 1    // �˴������Ƿ����ö�̬��ֵ�㷨��ֵΪ1ʱ����

u8 ccd_adc[128]={0};
u8 SciBuf[200];  //�洢�ϴ�����λ������Ϣ
int TIME_us=10; //�ع�ʱ��
u8 Flag_Target;
u8 CCD_Zhongzhi,CCD_Yuzhi;   //����CCD  ���

void Ccd_Init(void)
{ 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PA�˿�ʱ��
	
 // ����CLK�����ţ�ע�⣬��������RD_TSL�����ֶ�ģ��PWM 2022-8-13
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 //PA.2 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.2
 GPIO_SetBits(GPIOA,GPIO_Pin_2);						 //PA.2 �����

 // ����SI�����ţ������ڴ��޸����ŵ�����
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //PA.7 �˿�����, �������
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_7); 						 //PA.7	�����

}


/******************************************************************************
***
* FUNCTION NAME: void Dly_us(int a) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : ��ʱ�����������ع�ʱ�� *
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
* FUNCTION : ����ʱ�����ζ�ȡCCD�����ģ���ѹֵ *
* �������ܣ�CCD���ݲɼ� *
* MODIFY DATE : NONE *
* INPUT : void *
* OUTPUT : NONE *
* RETURN : NONE *
  
*******************************************************************************
**/
  void RD_TSL(void) 
	{
		u8 i=0,tslp=0;
		TSL_CLK=1;     //CLK������Ϊ�ߵ�ƽ          
		TSL_SI=0; 
		Dly_us(TIME_us);   //��ʱ�ع�ʱ��
				
		TSL_SI=1; 
		TSL_CLK=0;
		Dly_us(TIME_us);
				
		TSL_CLK=1;
		TSL_SI=0;
		Dly_us(TIME_us); 
		for(i=0;i<128;i++)
		{ 
			TSL_CLK=0; 
			Dly_us(TIME_us);  //�����ع�ʱ��
			ccd_adc[tslp]=(u8)((float)Get_Adc(ADC_Channel_1)/4096*255);  //����ȡ���ĵ�ѹֵ����������
			++tslp;
			TSL_CLK=1;
			Dly_us(TIME_us);
		}  
	}

/**************************************************************************
�������ܣ�����CCDȡ��ֵ
��ڲ�������
����  ֵ����
**************************************************************************/
void  Find_CCD_Zhongzhi(void)
{ 
	 static u16 i,j,Left,Right;
	#if dynamic_threshold
	 static u16 value1_max,value1_min;
	
	   value1_max=ccd_adc[0];  //��̬��ֵ�㷨����ȡ������Сֵ
     for(i=5;i<123;i++)   //���߸�ȥ��5����
     {
        if(value1_max<=ccd_adc[i])
        value1_max=ccd_adc[i];
     }
	   value1_min=ccd_adc[0];  //��Сֵ
     for(i=5;i<123;i++) 
     {
        if(value1_min>=ccd_adc[i])
        value1_min=ccd_adc[i];
     }
   CCD_Yuzhi=(value1_max+value1_min)/2;	  //���������������ȡ����ֵ
	#else
	 CCD_Yuzhi=25;  // ���ù̶�����ֵ
	#endif
	 printf("��ֵ��%d  ",CCD_Yuzhi);
	 for(i = 5;i<118; i++)   //Ѱ�����������
	{
		if(ccd_adc[i]>CCD_Yuzhi&&ccd_adc[i+1]>CCD_Yuzhi&&ccd_adc[i+2]>CCD_Yuzhi&&ccd_adc[i+3]<CCD_Yuzhi&&ccd_adc[i+4]<CCD_Yuzhi&&ccd_adc[i+5]<CCD_Yuzhi)
		{	
			Left=i;
			break;	
		}
	}
	 for(j = 118;j>5; j--)//Ѱ���ұ�������
  {
		if(ccd_adc[j]<CCD_Yuzhi&&ccd_adc[j+1]<CCD_Yuzhi&&ccd_adc[j+2]<CCD_Yuzhi&&ccd_adc[j+3]>CCD_Yuzhi&&ccd_adc[j+4]>CCD_Yuzhi&&ccd_adc[j+5]>CCD_Yuzhi)
		{	
		  Right=j;
		  break;	
		}
  }
	CCD_Zhongzhi=(Right+Left)/2;//��������λ��
	printf("��ֵ��%d\r\n",CCD_Zhongzhi);
//	if(myabs(CCD_Zhongzhi-Last_CCD_Zhongzhi)>90)   //�������ߵ�ƫ����̫��
//	CCD_Zhongzhi=Last_CCD_Zhongzhi;    //��ȡ��һ�ε�ֵ
//	Last_CCD_Zhongzhi=CCD_Zhongzhi;  //������һ�ε�ƫ��
}

/*

**************************���º�����Ҫ�����λ��ʹ��*****************************

*/

/******************************************************************************
***
* FUNCTION NAME: void slove_data(void) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : ����CCD�ж�ȡ������Ϣ������λ��Ҫ���ͨ��Э����뵽�������Դ����� *
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
* FUNCTION : �������͵���Ϣͨ�����ڷ�������λ��*
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
				printf("%c",SciBuf[i]); //���ַ���ʽ���͸�4λ��Ӧ��16����
				//printf("%c",binToHex_low(SciBuf[i]));  //���ַ���ʽ���͵�4λ��Ӧ��16����
		 }
		 printf("00");   //ͨ��Э��Ҫ��
		 printf("#");    //ͨ��Э��Ҫ��
	 }
	 
/******************************************************************************
***
* FUNCTION NAME: binToHex_low(u8 num) *
* CREATE DATE : 20170707 *
* CREATED BY : XJU *
* FUNCTION : �������Ƶ�8λת��16����*
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
* FUNCTION : �������Ƹ�8λת��16����*
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



