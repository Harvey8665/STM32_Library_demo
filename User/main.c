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

 int main(void)
 {
//	 int a=25;
//	 int b=125;
	 int a=0;
//	 u8 t;
//	 u8 len;	
	 
   delay_init();	    	 //延时函数初始化	  
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 OLED_Init();			//初始化OLED
	 uart3_init(115200);	 //串口初始化为115200
	 //Adc_Init();  //ADC初始化
	 //Ccd_Init();   //CCD初始化
	 TIM2_PWM_Init(999,1439); //=====定时中断初始化
	 TIM3_PWM_Init(999,1439);
	 //TIM3_PWM_Init(1,1);
	 //Timer_Init(49,7199);
	 //TIME_us=1;    //设置曝光时间
	 
   while(1)
	 {

		 for(a=0;a<100;a++)
		 {
			 OLED_Show();
		   TIM_SetCompare2(TIM2,125-a);
			 delay_ms(20);
		 	 TIM_SetCompare1(TIM2,25+a);
			 OLED_ShowNum(80,0,a,2,16);
		 }
		 for(a=0;a<100;a++)
		 {
			 OLED_Show();
		   TIM_SetCompare1(TIM2,125-a);
			 delay_ms(20);
		 	 TIM_SetCompare2(TIM2,25+a);
			 OLED_ShowNum(80,0,a,2,16);
		 }
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



