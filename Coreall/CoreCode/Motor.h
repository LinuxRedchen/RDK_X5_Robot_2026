#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "main.h"
void Robot_Forward(void);
void Robot_Back(void);
void Robot_Turn_Left(void);
void Robot_Turn_Right(void);
void Robot_Move_Left(void);
void Robot_Move_Right(void);
void Robot_Stop(void);
void Robot_Motion_PWM(unsigned char PWM_Value);
void Robot_PWMSetStart(void);

#endif
