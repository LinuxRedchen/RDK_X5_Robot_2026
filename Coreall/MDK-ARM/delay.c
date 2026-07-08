#include "delay.h"

void delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    HAL_TIM_Base_Start(&htim4);
    while (__HAL_TIM_GET_COUNTER(&htim4) < us);
    HAL_TIM_Base_Stop(&htim4);
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        delay_us(1000);
    }
}