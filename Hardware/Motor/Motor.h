#ifndef _MOTOR_H
#define _MOTOR_H

#include "stm32f10x.h"
#include "sys.h"

#define speed 300

void Motor_init(void);  // ��ʼ��TB6612�������ź�STBY����
void Motor_PWM_Init(u16 arr,u16 psc);  // ��ʼ��TB6612��PWMA��PWMB
void Motor_Go(void);  // ǰ������
void Motor_Back(void);  // ���˺���
void Motor_Left(void);  // ��ת����
void Motor_Right(void);  // ��ת����
void Motor_Stop(void);  // ֹͣ����



#endif


