#include "Motor.h"

void Motor_LF(GPIO_PinState IN1,GPIO_PinState IN2)
{
	HAL_GPIO_WritePin(L_Front_In_GPIO_Port,L_Front_In_Pin,IN1);
	HAL_GPIO_WritePin(L_Front_Out_GPIO_Port,L_Front_Out_Pin,IN2);
}
void Motor_LA(GPIO_PinState IN1,GPIO_PinState IN2)
{
	HAL_GPIO_WritePin(L_After_In_GPIO_Port,L_After_In_Pin,IN2);
	HAL_GPIO_WritePin(L_After_Out_GPIO_Port,L_After_Out_Pin,IN1);
}
void Motor_RF(GPIO_PinState IN1,GPIO_PinState IN2)
{
	HAL_GPIO_WritePin(R_Front_In_GPIO_Port,R_Front_In_Pin,IN1);
	HAL_GPIO_WritePin(R_Front_Out_GPIO_Port,R_Front_Out_Pin,IN2);
}
void Motor_RA(GPIO_PinState IN1,GPIO_PinState IN2)
{
	HAL_GPIO_WritePin(R_After_In_GPIO_Port,R_After_In_Pin,IN2); 
	HAL_GPIO_WritePin(R_After_Out_GPIO_Port,R_After_Out_Pin,IN1);
}

void Robot_Forward(void)
{
	Motor_LF(HIGH,LOW);
	Motor_LA(HIGH,LOW);
	Motor_RF(HIGH,LOW);
	Motor_RA(HIGH,LOW);
}
void Robot_Back(void)
{
	Motor_LF(LOW,HIGH);
	Motor_LA(LOW,HIGH);
	Motor_RF(LOW,HIGH);
	Motor_RA(LOW,HIGH);
}
void Robot_Turn_Left(void)
{
	Motor_LF(LOW,HIGH);
	Motor_LA(LOW,HIGH);
	Motor_RF(HIGH,LOW);
	Motor_RA(HIGH,LOW);
}
void Robot_Turn_Right(void)
{
	Motor_LF(HIGH,LOW);
	Motor_LA(HIGH,LOW);
	Motor_RF(LOW,HIGH);
	Motor_RA(LOW,HIGH);
}
void Robot_Move_Left(void)
{
	Motor_LF(HIGH,LOW);
	Motor_LA(LOW,HIGH);
	Motor_RF(LOW,HIGH);
	Motor_RA(HIGH,LOW);
}
void Robot_Move_Right(void)
{
	Motor_LF(LOW,HIGH);
	Motor_LA(HIGH,LOW);
	Motor_RF(HIGH,LOW);
	Motor_RA(LOW,HIGH);
}

void Robot_Stop(void)
{
	Motor_LF(LOW,LOW);
	Motor_LA(LOW,LOW);
	Motor_RF(LOW,LOW);
	Motor_RA(LOW,LOW);
}
void Robot_PWMSetStart(void)
{
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
}
void Robot_Motion_PWM(unsigned char PWM_Value)
{
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,PWM_Value);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,PWM_Value);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,PWM_Value);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,PWM_Value);	
}
