#include "Modules.h"
#include "Control.h"

/*****�����50תȦֱ��80 ����100ֱ��50�� ��200ֱ��ԭ��תȦ *******/

/*------------------------------------------���-���ڼĴ���200-----------------------------------*/
void motorL_forward(unsigned char speed)           //ֱ�������ת��δ����ʱ255�͹��ˣ���ת�Ļ�����Ҫ��һ·PWM���п���
{
	PWMDTY0 = speed;
	PWMDTY1 = 0;
}

void motorL_backward(unsigned char speed)          //�տ�ʼ����Ҫ���������Űɣ�û��Ҫע�͵�
{
	PWMDTY0 = 0;
	PWMDTY1 = speed;
}

void motorR_forward(unsigned char speed)            //ֱ�������ת��δ����ʱ255�͹��ˣ���ת�Ļ�����Ҫ��һ·PWM���п���
{
	PWMDTY7 = speed;
	PWMDTY6 = 0;
}

void motorR_backward(unsigned char speed)
{
  PWMDTY6 = speed;
	PWMDTY7 = 0;	
}



/*---------------------------------------���-���ڼĴ���20000-----------------------------------*/
void servo_turn(unsigned int duty)                 //�ı������  
{                                                 //��ֵΪ1430 ������Ϊ1580���Ҽ���ֵΪ1280
    PWMDTY23 = duty;                     
}  
void servo_turn_R(uint duty){              //���ֵ150
    PWMDTY23 = 1430 - duty;    
}
void servo_turn_L(uint duty){              //���ֵ150
    PWMDTY23 = 1430 + duty ;    
}
