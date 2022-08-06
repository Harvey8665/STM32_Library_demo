#ifndef __CCD_H
#define __CCD_H	 
#include "sys.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
/*
硬件资源:开发板     线性CCD
         5V  —— VDD
         GND   —— GND
         PA1   —— AO1
         PA2   —— CLK
         PA7   —— SI
*/
////////////////////////////////////////////////////////////////////////////////// 
#define TSL_SI    PAout(7)   //SI  7
#define TSL_CLK   PAout(2)   //CLK 2
#define PI 3.14159265  // 定义常数pi
#define ZHONGZHI 0
// 声明已经在其他地方声明过的变量
extern u8 ccd_adc[128];  // 储存每个像素点对应的值，此处应为模拟量，使用时需要配合阈值使用
extern int TIME_us;  //曝光时间
extern u8 CCD_Zhongzhi,CCD_Yuzhi; // 定义CCD检测的中值（左下降沿和右上升沿的坐标之和的一半）、CCD的阈值（小于阈值的为检测目标的位置，大于阈值的为空白）

void Ccd_Init(void);  // 初始化CCD摄像头
void Dly_us(int a);
void RD_TSL(void);  // CCD数据采集
void sendToPc(void);  // 发送到上位机，需要配合 WHEELTEC_CCD串口助手
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
void Find_CCD_Zhongzhi(void); // 线性CCD取中值

		 				    
#endif
