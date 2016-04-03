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

void motor0_open(unsigned int duty);                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���


void motor0_close();


void motor1_open(unsigned int duty);                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���

void motor1_close();

void motor4_open(unsigned int duty);                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���


void motor4_close();

void motor5_open(unsigned int duty);                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���

void motor5_close();



extern uint T;
extern uchar flag_fan;




#endif