#ifndef _TIM_H
#define _TIM_H

#include "stm32f10x.h"
#include "sys.h"

// 默认复用功能的初始化函数
void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);

#endif




