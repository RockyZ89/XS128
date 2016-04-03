#ifndef _MODEL_H
#define _MODEL_H


void init_AD();

void init_PIT();

void buzzer_s();
/******************************************************
*                          PWM                        *
******************************************************/
void init_PWM();

/*******************PID**************/
int PID(double ek);

void motor0_open(unsigned int duty);                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制


void motor0_close();


void motor1_open(unsigned int duty);                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制

void motor1_close();

void motor4_open(unsigned int duty);                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制


void motor4_close();

void motor5_open(unsigned int duty);                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制

void motor5_close();



extern uint T;
extern uchar flag_fan;




#endif