#include "Room.h"

void Room_Judge_Proc(void)
{
	if(Room_Flag==0)
	{
		if(Gray1 == HIGH && Gray2 == HIGH && Gray3 == HIGH && Gray4 == HIGH && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//左房+1 1111 0000
		{
			Room_LNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == HIGH && Gray2 == HIGH && Gray3 == HIGH && Gray4 == LOW && Gray5 == LOW && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//左房+1 1110 0000
		{
			Room_LNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == HIGH && Gray2 == HIGH && Gray3 == HIGH && Gray4 == HIGH && Gray5 == HIGH && Gray6 == LOW && Gray7 == LOW && Gray8 == LOW)//左房+1 1111 1000
		{
			Room_LNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == HIGH && Gray6 == HIGH && Gray7 == HIGH && Gray8 == HIGH)//右房+1 0000 1111
		{
			Room_RNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == LOW && Gray5 == LOW && Gray6 == HIGH && Gray7 == HIGH && Gray8 == HIGH)//右房+1 0000 0111
		{
			Room_RNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == LOW && Gray2 == LOW && Gray3 == LOW && Gray4 == HIGH && Gray5 == HIGH && Gray6 == HIGH && Gray7 == HIGH && Gray8 == HIGH)//右房+1 0001 1111
		{
			Room_RNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == HIGH && Gray2 == HIGH && Gray3 == HIGH && Gray4 == HIGH && Gray5 == HIGH && Gray6 == HIGH && Gray7 == LOW && Gray8 == LOW)//左房+1 1111 1100
		{
			Room_LNum+=1;
			Room_Flag=1;
		}
		else if(Gray1 == LOW && Gray2 == LOW && Gray3 == HIGH && Gray4 == HIGH && Gray5 == HIGH && Gray6 == HIGH && Gray7 == HIGH && Gray8 == HIGH)//右房+1 1111 1000
		{
			Room_RNum+=1;
			Room_Flag=1;
		}
	}
	delay_ms(1500);
	Room_Flag=0;
}
