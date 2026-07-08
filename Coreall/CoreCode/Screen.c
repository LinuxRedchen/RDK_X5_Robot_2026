#include "Screen.h"

void Screen_Judge_Proc(void)
{
	if(ReceiveDate[0] == 170 && ReceiveDate[1] == 1 && ReceiveDate[2] == 238 )
	{
		Goal_Room = 1;
		Step1=1;
	}
	else if(ReceiveDate[0] == 170 && ReceiveDate[1] == 2 && ReceiveDate[2] == 238 )
	{
		Goal_Room = 2;
		Step1=1;
	}
	else if(ReceiveDate[0] == 170 && ReceiveDate[1] == 3 && ReceiveDate[2] == 238 )
	{
		Goal_Room = 3;
		Step1=1;
	}
	else if(ReceiveDate[0] == 170 && ReceiveDate[1] == 4 && ReceiveDate[2] == 238 )
	{
		Goal_Room = 4;
		Step1=1;
	}
	else if(ReceiveDate[0] == 170 && ReceiveDate[1] == 5 && ReceiveDate[2] == 238)
	{
		Goal_Room = 5;
		Step1=1;
	}
	else if(ReceiveDate[0] == 170 && ReceiveDate[1] == 6 && ReceiveDate[2] == 238 )
	{
		Goal_Room = 6;
		Step1=1;
	}
	else if(ReceiveDate[0] == 187 && ReceiveDate[1] == 1 && ReceiveDate[2] == 238 )
	{
		Cabinet_Flag = 1;
		Step2=1;
	}
	else if(ReceiveDate[0] == 187 && ReceiveDate[1] == 2 && ReceiveDate[2] == 238 )
	{
		Cabinet_Flag = 0;
		Step2=1;
	}
}