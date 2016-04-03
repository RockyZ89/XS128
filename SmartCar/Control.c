#include "Modules.h"
#include "Control.h"

/*****电机给50转圈直径80 ，给100直径50， 给200直径原地转圈 *******/

/*------------------------------------------电机-周期寄存器200-----------------------------------*/
void motorL_forward(unsigned char speed)           //直流电机正转，未级联时255就够了，反转的话还需要打开一路PWM进行控制
{
	PWMDTY0 = speed;
	PWMDTY1 = 0;
}

void motorL_backward(unsigned char speed)          //刚开始不需要，但是留着吧，没必要注释掉
{
	PWMDTY0 = 0;
	PWMDTY1 = speed;
}

void motorR_forward(unsigned char speed)            //直流电机正转，未级联时255就够了，反转的话还需要打开一路PWM进行控制
{
	PWMDTY7 = speed;
	PWMDTY6 = 0;
}

void motorR_backward(unsigned char speed)
{
  PWMDTY6 = speed;
	PWMDTY7 = 0;	
}



/*---------------------------------------舵机-周期寄存器20000-----------------------------------*/
void servo_turn(unsigned int duty)                 //改变舵机打角  
{                                                 //中值为1430 ，左极限为1580，右极限值为1280
    PWMDTY23 = duty;                     
}  
void servo_turn_R(uint duty){              //最大值150
    PWMDTY23 = 1430 - duty;    
}
void servo_turn_L(uint duty){              //最大值150
    PWMDTY23 = 1430 + duty ;    
}
