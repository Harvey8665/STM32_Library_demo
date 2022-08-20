#include "Motor.h"
#include "stm32f10x.h"

/////////////////////////////////////////////////////////
//
//  功能：初始化TB6612的引脚，控制电机前进后退转弯的函数
//  接线：AIN1->PB12（V1.2底板已接好）
//        AIN2->PB13（V1.2底板已接好）
//        BIN1->PB14（V1.2底板已接好）
//        BIN2->PB15（V1.2底板已接好）
//        STBY->PC13（V1.2底板已接好）
//        PWMA->PB10（V1.3底板已接好）
//        PWMB->PB11 （V1.3底板已接好）
//
/////////////////////////////////////////////////////////

// TB6612的输入引脚和STBY引脚初始化函数
void Motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能A端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOB12,13,14,15
 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  // 将GPIOB12,13,14,15置高
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //初始化GPIOC13
	// STBY引脚为高电平时，TB6612才起作用
 	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置高
}

// TB6612的PWMA、PWMB引脚的初始化函数
void Motor_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //禁止JTAG功能，把PB3，PB4作为普通IO口使用
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);  //TIM2所有引脚重映射
	
	// PWMA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM2_CH1重映射引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	// PWMB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM2_CH2重映射引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channe1234 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC3
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM2 OC4
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
//	TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE 主输出使能	
}

// 前进函数
void Motor_Go(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置高
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	
	TIM_SetCompare1(TIM2,speed);
	TIM_SetCompare2(TIM2,speed);
}

// 后退函数
void Motor_Back(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置高
	
}

// 左转函数
void Motor_Left(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置高
	
}

// 右转函数
void Motor_Right(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置高
	
}

// 停止函数
void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);  // 将GPIOC13置低
}







