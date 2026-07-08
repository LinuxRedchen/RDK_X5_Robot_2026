#include "MyDelay.h"

void MyDelay(uint32_t Delay)//1000->1s
{
	uint32_t expireTime = MyGetTick() + Delay;
	while(expireTime > MyGetTick());
}

uint32_t MyGetTick(void)
{
	return currentMiliSeconds;
}



void MyDelayProcFlag(void)
{
	HAL_TIM_Base_Start_IT(&htim1);
}
