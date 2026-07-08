#include "Gray.h"

void Gray_Read_Proc(void)
{
	Gray1 = HAL_GPIO_ReadPin(Gray1_GPIO_Port,Gray1_Pin);
	Gray2 = HAL_GPIO_ReadPin(Gray2_GPIO_Port,Gray2_Pin);
	Gray3 = HAL_GPIO_ReadPin(Gray3_GPIO_Port,Gray3_Pin);
	Gray4 = HAL_GPIO_ReadPin(Gray4_GPIO_Port,Gray4_Pin);
	Gray5 = HAL_GPIO_ReadPin(Gray5_GPIO_Port,Gray5_Pin);
	Gray6 = HAL_GPIO_ReadPin(Gray6_GPIO_Port,Gray6_Pin);
	Gray7 = HAL_GPIO_ReadPin(Gray7_GPIO_Port,Gray7_Pin);
	Gray8 = HAL_GPIO_ReadPin(Gray8_GPIO_Port,Gray8_Pin);
}
