#ifndef _MODULES_H
#define _MODULES_H


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


/*，，，，，，，，，，，，，，，，，，，，，，，，，，，，，，，方象窃侏崎，，，，，，，，，，，，，，，，，，，，，，，，，，，，*/
#define uchar unsigned char
#define uint_16 unsigned int
#define int_16 int
#define uint_32 unsigned long int
#define int_32  long int
#define ufloat_32 unsigned float
#define float_32  float
#define ufloat_64 unsigned double
#define float_64 double

#define true 1
#define false 0




void delay_ms_80M(uint xms);
void delay_us_80M(uint xus);

void init_PIT();
void init_PWM();
void init_TIM();
void init_SCI();

void Interrput_Priority_Set(unsigned char irq,unsigned char set_val);


void init_PLL(void);

/*
void SetBusCLK_16M(void);     
void SetBusCLK_32M(void);     
void SetBusCLK_40M(void);     
void SetBusCLK_48M(void);     
void SetBusCLK_64M(void);  
void SetBusCLK_80M(void);      
void SetBusCLK_88M(void);     
void SetBusCLK_96M(void);      
void SetBusCLK_104M(void);      
void SetBusCLK_120M(void);  
*/


#endif
