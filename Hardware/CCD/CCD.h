#ifndef __CCD_H
#define __CCD_H	 
#include "sys.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
/*
Ӳ����Դ:������     ����CCD
         5V  ���� VDD
         GND   ���� GND
         PA1   ���� AO1
         PA2   ���� CLK
         PA7   ���� SI
*/
////////////////////////////////////////////////////////////////////////////////// 
#define TSL_SI    PAout(7)   //SI  7
#define TSL_CLK   PAout(2)   //CLK 2
#define PI 3.14159265  // ���峣��pi
#define ZHONGZHI 0
// �����Ѿ��������ط��������ı���
extern u8 ccd_adc[128];  // ����ÿ�����ص��Ӧ��ֵ���˴�ӦΪģ������ʹ��ʱ��Ҫ�����ֵʹ��
extern int TIME_us;  //�ع�ʱ��
extern u8 CCD_Zhongzhi,CCD_Yuzhi; // ����CCD������ֵ�����½��غ��������ص�����֮�͵�һ�룩��CCD����ֵ��С����ֵ��Ϊ���Ŀ���λ�ã�������ֵ��Ϊ�հף�

void Ccd_Init(void);  // ��ʼ��CCD����ͷ
void Dly_us(int a);
void RD_TSL(void);  // CCD���ݲɼ�
void sendToPc(void);  // ���͵���λ������Ҫ��� WHEELTEC_CCD��������
char binToHex_high(u8 num);
char binToHex_low(u8 num);
void slove_data(void);
void Kinematic_Analysis(float velocity,float angle);
void TIM2_IRQHandler(void) ;
void Set_Pwm(int motor_a,int motor_b,int servo);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off( int voltage);
void Get_Angle(u8 way);
int myabs(int a);
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
void Get_RC(void);
void Find_CCD_Zhongzhi(void); // ����CCDȡ��ֵ

		 				    
#endif
