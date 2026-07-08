/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "Gray.h"
#include "Path.h"
#include "tim.h"
#include "Motor.h"
#include "Room.h"
#include "delay.h"
#include "Delivery.h"
#include "Integration.h"
#include "Screen.h"
#include "esp8266.h"
#include "onenet.h"
#include "Lock.h"
#include "CabinetTemp.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint8_t Gray1;
extern uint8_t Gray2;
extern uint8_t Gray3;
extern uint8_t Gray4;
extern uint8_t Gray5;
extern uint8_t Gray6;
extern uint8_t Gray7;
extern uint8_t Gray8;
extern uint8_t Room_LastNum;
extern uint8_t temp;//温度
extern uint8_t humi;//湿度
extern uint8_t Room_LNum,Room_RNum;//左右房间数
extern uint8_t Room_Flag;//检测到房间标志
extern uint8_t Goal_Room;//目标房间
extern uint8_t Actual_Room;//现在房间
extern uint8_t Room_Direction;//1是右 2是左
extern volatile uint32_t currentMiliSeconds;
extern uint8_t ReceiveDate[10],Receive_Cnt;
extern uint8_t Rdata;
extern uint8_t Cabinet_Flag;
extern uint8_t Step1,Step2;

extern uint8_t DHT11_Hum_Int;    // 湿度整数部分（DHT11小数部分恒为0）
extern uint8_t DHT11_Temp_Int;   // 温度整数部分
extern uint8_t DHT11_CheckSum;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define L_After_Out_Pin GPIO_PIN_1
#define L_After_Out_GPIO_Port GPIOC
#define R_Front_Out_Pin GPIO_PIN_2
#define R_Front_Out_GPIO_Port GPIOC
#define Motor_STBY_Pin GPIO_PIN_3
#define Motor_STBY_GPIO_Port GPIOC
#define L_Front_PWM_Pin GPIO_PIN_0
#define L_Front_PWM_GPIO_Port GPIOA
#define R_Front_PWM_Pin GPIO_PIN_1
#define R_Front_PWM_GPIO_Port GPIOA
#define L_Front_In_Pin GPIO_PIN_4
#define L_Front_In_GPIO_Port GPIOA
#define L_After_In_Pin GPIO_PIN_5
#define L_After_In_GPIO_Port GPIOA
#define R_Front_In_Pin GPIO_PIN_6
#define R_Front_In_GPIO_Port GPIOA
#define R_After_In_Pin GPIO_PIN_7
#define R_After_In_GPIO_Port GPIOA
#define L_Front_Out_Pin GPIO_PIN_4
#define L_Front_Out_GPIO_Port GPIOC
#define R_After_Out_Pin GPIO_PIN_5
#define R_After_Out_GPIO_Port GPIOC
#define Gray1_Pin GPIO_PIN_0
#define Gray1_GPIO_Port GPIOB
#define Gray2_Pin GPIO_PIN_1
#define Gray2_GPIO_Port GPIOB
#define L_After_PWM_Pin GPIO_PIN_10
#define L_After_PWM_GPIO_Port GPIOB
#define R_After_PWM_Pin GPIO_PIN_11
#define R_After_PWM_GPIO_Port GPIOB
#define Fan_Pin GPIO_PIN_6
#define Fan_GPIO_Port GPIOG
#define Switch_Pin GPIO_PIN_7
#define Switch_GPIO_Port GPIOG
#define Dht11_Pin GPIO_PIN_8
#define Dht11_GPIO_Port GPIOA
#define L298N_IN1_Pin GPIO_PIN_0
#define L298N_IN1_GPIO_Port GPIOD
#define L298N_IN2_Pin GPIO_PIN_1
#define L298N_IN2_GPIO_Port GPIOD
#define L298N_IN3_Pin GPIO_PIN_4
#define L298N_IN3_GPIO_Port GPIOD
#define L298N_IN4_Pin GPIO_PIN_5
#define L298N_IN4_GPIO_Port GPIOD
#define Gray3_Pin GPIO_PIN_3
#define Gray3_GPIO_Port GPIOB
#define Gray4_Pin GPIO_PIN_4
#define Gray4_GPIO_Port GPIOB
#define Gray5_Pin GPIO_PIN_5
#define Gray5_GPIO_Port GPIOB
#define Gray6_Pin GPIO_PIN_6
#define Gray6_GPIO_Port GPIOB
#define Gray7_Pin GPIO_PIN_7
#define Gray7_GPIO_Port GPIOB
#define Gray8_Pin GPIO_PIN_8
#define Gray8_GPIO_Port GPIOB
#define Control_Lock_Pin GPIO_PIN_9
#define Control_Lock_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
