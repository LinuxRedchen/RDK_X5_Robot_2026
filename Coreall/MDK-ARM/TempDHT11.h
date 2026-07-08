#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

/************************* 引脚定义 *************************/
#define DHT11_GPIO_PORT    GPIOA
#define DHT11_GPIO_PIN     GPIO_PIN_8

/************************* IO操作封装 *************************/
// 输出电平（兼容枚举类型，消除类型警告）
#define DHT11_Set_Level(level)  HAL_GPIO_WritePin(DHT11_GPIO_PORT, DHT11_GPIO_PIN, (GPIO_PinState)level)
// 读取电平
#define DHT11_Get_Level()       HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

/************************* 函数声明 *************************/
// 初始化DHT11（返回0=成功，1=失败）
uint8_t DHT11_Init(void);
// 读取温湿度数据（temp=温度，humi=湿度；返回0=成功，1=失败）
uint8_t DHT11_Read_Data(uint8_t *temp, uint8_t *humi);
// 底层函数（外部无需调用，仅内部使用）
void DHT11_Reset(void);
uint8_t DHT11_Check_Ack(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);

#endif