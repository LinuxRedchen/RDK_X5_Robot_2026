#include "Delivery.h"

void Goods_Room_Delivery_Proc(void)
{
	if(Goal_Room==1)
	{
		Robot_Forward();
		delay_ms(2000);
		Robot_Move_Left();
		delay_ms(500);
		Robot_Stop();
		delay_ms(3000);
		Room_LastNum = 1;
		Goal_Room=10;
	}
	if(Goal_Room==2)
	{
		Robot_Forward();
		delay_ms(4500);
		Robot_Turn_Right();
		delay_ms(500);
		Robot_Stop();
		delay_ms(3000);
		Room_LastNum = 2;
		Goal_Room=10;
	}
	if(Goal_Room==3)
	{
		Robot_Forward();
		delay_ms(7000);
		Robot_Turn_Left();
		delay_ms(500);
		Robot_Stop();
		delay_ms(3000);
		Room_LastNum = 3;
		Goal_Room=10;
	}
}

void Back_Delivery_Proc(void)
{
	if(Goal_Room==10)
	{
		if(Room_LastNum==1)
		{
			Robot_Turn_Right();
			delay_ms(500);
			Robot_Back();
			delay_ms(2000);
			Robot_Stop();
			delay_ms(3000);
			Goal_Room=0;
			Room_LastNum=0;
		}
		if(Room_LastNum==2)
		{
			Robot_Turn_Left();
			delay_ms(500);
			Robot_Back();
			delay_ms(4500);
			Robot_Stop();
			delay_ms(3000);
			Goal_Room=0;
			Room_LastNum=0;
		}
		if(Room_LastNum==3)
		{
			Robot_Turn_Right();
			delay_ms(500);
			Robot_Back();
			delay_ms(7000);
			Robot_Stop();
			delay_ms(3000);
			Goal_Room=0;
			Room_LastNum=0;
		}
	}
}
