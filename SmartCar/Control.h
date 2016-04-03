#ifndef _CONTROL_H
#define _CONTROL_H


void motorL_forward(unsigned char speed);          

void motorL_backward(unsigned char speed);          

void motorR_forward(unsigned char speed);            

void motorR_backward(unsigned char speed);

void servo_turn(unsigned int dir);  

void servo_turn_L(uint duty);

void servo_turn_R(uint duty);



#endif              

