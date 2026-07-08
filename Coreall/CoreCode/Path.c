#include "Path.h"

void Path_Judge_Proc(void)
{//黑线高电平 白线低电平
	if(Gray1 == LOW && Gray2 == LOW && Gray3 == HIGH && Gray4 == HIGH && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//前进 0011 0000
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == HIGH && Gray5 == HIGH && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//前进 0001 1000
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == HIGH && Gray6 == HIGH && Gray7 == LOW && Gray8 == LOW)//前进 0000 1100
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == HIGH && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//前进 0010 0000
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == HIGH && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//前进 0001 0000
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == HIGH && Gray7 == LOW && Gray8 == LOW)//前进 0000 0100
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == HIGH && Gray6 == HIGH && Gray7 == LOW && Gray8 == LOW)//前进 0000 1100
	{
		Robot_Forward();
	}
	else if(Gray1 == LOW && Gray2 == HIGH && Gray3 == HIGH && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//右移 0110 0000
	{
		Robot_Move_Right();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == HIGH && Gray7 == HIGH && Gray8 == LOW)//左移 0000 0110
	{
		Robot_Move_Left();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == HIGH && Gray8 == LOW)//左移 0000 0010
	{
		Robot_Move_Left();
	}
	else if(Gray1 == LOW && Gray2 == HIGH && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//右移 0100 0000
	{
		Robot_Move_Right();
	}
	else if(Gray1 == HIGH && Gray2 == HIGH && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//右移 1100 0000
	{
		Robot_Move_Right();
	}
	else if(Gray1 == HIGH && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//右移 1000 0000
	{
		Robot_Move_Right();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == HIGH && Gray8 == HIGH)//左移 0000 0011
	{
		Robot_Move_Left();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == HIGH)//左移 0000 0001
	{
		Robot_Move_Left();
	}
	else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//后退 0000 0000
	{
		Robot_Back();
	}
}
