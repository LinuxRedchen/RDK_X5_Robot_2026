#ifndef __DELAY_H
#define __DELAY_H

#include "tim.h"
#include "stdint.h"

// 微秒级延时函数（参数us：延时微秒数，最大65535）
void delay_us(uint32_t us);
// 毫秒级延时函数（参数ms：延时毫秒数，无上限）
void delay_ms(uint32_t ms);

#endif
