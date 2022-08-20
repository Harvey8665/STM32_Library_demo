#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f10x.h"
#include "sys.h"

#define speed 300

void Motor_init(void);  // 初始化TB6612输入引脚和STBY引脚
void Motor_PWM_Init(u16 arr,u16 psc);  // 初始化TB6612的PWMA、PWMB
void Motor_Go(void);  // 前进函数
void Motor_Back(void);  // 后退函数
void Motor_Left(void);  // 左转函数
void Motor_Right(void);  // 右转函数
void Motor_Stop(void);  // 停止函数



#endif


