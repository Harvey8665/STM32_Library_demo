#include "Motor.h"
#include "stm32f10x.h"

/////////////////////////////////////////////////////////
//
//  ���ܣ���ʼ��TB6612�����ţ����Ƶ��ǰ������ת��ĺ���
//  ���ߣ�AIN1->PB12��V1.2�װ��ѽӺã�
//        AIN2->PB13��V1.2�װ��ѽӺã�
//        BIN1->PB14��V1.2�װ��ѽӺã�
//        BIN2->PB15��V1.2�װ��ѽӺã�
//        STBY->PC13��V1.2�װ��ѽӺã�
//        PWMA->PB10��V1.3�װ��ѽӺã�
//        PWMB->PB11 ��V1.3�װ��ѽӺã�
//
/////////////////////////////////////////////////////////

// TB6612���������ź�STBY���ų�ʼ������
void Motor_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��A�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //��ʼ��GPIOB12,13,14,15
 	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  // ��GPIOB12,13,14,15�ø�
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	  //��ʼ��GPIOC13
	// STBY����Ϊ�ߵ�ƽʱ��TB6612��������
 	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�ø�
}

// TB6612��PWMA��PWMB���ŵĳ�ʼ������
void Motor_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO����
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //��ֹJTAG���ܣ���PB3��PB4��Ϊ��ͨIO��ʹ��
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);  //TIM2����������ӳ��
	
	// PWMA
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //TIM2_CH1��ӳ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	// PWMB
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //TIM2_CH2��ӳ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2 Channe1234 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC3
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 OC4
	
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
//	TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	
}

// ǰ������
void Motor_Go(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�ø�
	
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
	
	TIM_SetCompare1(TIM2,speed);
	TIM_SetCompare2(TIM2,speed);
}

// ���˺���
void Motor_Back(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�ø�
	
}

// ��ת����
void Motor_Left(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�ø�
	
}

// ��ת����
void Motor_Right(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�ø�
	
}

// ֹͣ����
void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);  // ��GPIOC13�õ�
}







