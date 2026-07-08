#ifndef __CABINETEMP_H__
#define __CABINETEMP_H__

#include "main.h"

/* 引脚定义：PA8 */
#define DHT11_GPIO_PORT    GPIOA
#define DHT11_GPIO_PIN     GPIO_PIN_8

/* 函数声明 */
// DHT11引脚初始化（对外接口，替代原DHT11_GPIO_Init）
uint8_t DHT11_GPIO_Init(void);
// 获取湿度值（对外接口）
uint8_t Get_humidity(void);
// 获取温度值（对外接口）
uint8_t Get_temperature(void);
// 读取DHT11温湿度数据（核心函数）
uint8_t DHT11_Read_Data(void);

#endif