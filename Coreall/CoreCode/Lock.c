#include "Lock.h"

void Lock_Control(void)
{
	if(Cabinet_Flag == 1)
	{
		HAL_GPIO_WritePin(Control_Lock_GPIO_Port,Control_Lock_Pin,GPIO_PIN_SET);
		delay_ms(1500);
		Cabinet_Flag=0;
	}
	else
	{
		HAL_GPIO_WritePin(Control_Lock_GPIO_Port,Control_Lock_Pin,GPIO_PIN_RESET);
	}
}