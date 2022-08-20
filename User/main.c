#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "bmp.h"
#include "adc.h"
#include "ccd.h"
#include "Tim.h"
#include "Motor.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"

 int main(void)
 {
//	 int a=25;
//	 int b=125;
//	 int a=0;
//	 u8 t;
//	 u8 len;	
	 
   delay_init();	    	 //延时函数初始化	  
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 OLED_Init();			//初始化OLED
	 //Adc_Init();  //ADC初始化
	 //Ccd_Init();   //CCD初始化
	 //TIM2_PWM_Init(999,1439); //=====定时中断初始化
//	 TIM3_PWM_Init(999,1439);
	 IIC_Init();  // 初始化MPU6050的IIC接口
	 Motor_init();
	 Motor_PWM_Init(899,0);
	 //Timer_Init(49,7199);
	 //TIME_us=1;    //设置曝光时间
	 
   while(1)
	 {
			Motor_Go();
		  delay_ms(500);
		  Motor_Stop();
		  delay_ms(10000);
//		 for(a=0;a<100;a++)
//		 {
//			 OLED_Clear();
//			 OLED_ShowString(0,0,"TIM2-CH2:",16);
//			 OLED_ShowChinese(0,48,13,16);   // 版
//	     OLED_ShowChinese(16,48,14,16);  // 本
//	     OLED_ShowChinese(32,48,15,16);  // 号
//	     OLED_ShowString(48,48,":V0.0.1",16); //显示字符串
//		   TIM_SetCompare2(TIM2,125-a);
//			 delay_ms(20);
//		 	 TIM_SetCompare1(TIM2,25+a);
//			 OLED_ShowNum(80,0,a,2,16);
//			 OLED_Refresh();
//		 }
//		 for(a=0;a<100;a++)
//		 {
////			 OLED_Show();
//			 OLED_Clear();
//			 OLED_ShowString(0,0,"TIM2-CH2:",16);
//			 OLED_ShowChinese(0,48,13,16);   // 版
//	     OLED_ShowChinese(16,48,14,16);  // 本
//	     OLED_ShowChinese(32,48,15,16);  // 号
//	     OLED_ShowString(48,48,":V0.0.1",16); //显示字符串
//		   TIM_SetCompare1(TIM2,125-a);
//			 delay_ms(20);
//		 	 TIM_SetCompare2(TIM2,25+a);
//			 OLED_ShowNum(100,0,a,2,16);
//			 OLED_Refresh();
//		 }


//		 	delay_ms(1000);
//		 TIM_SetCompare2(TIM2,0);
//		 delay_ms(1000);
//		 TIM_SetCompare2(TIM2,700);
//		 delay_ms(1000);
//		 TIM_SetCompare2(TIM2,0);
//		 delay_ms(1000);
		 //RD_TSL();
		 //Find_CCD_Zhongzhi();

//		 sendToPc();
		 
		 
	 }
 }



