#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"

/******************
      延时us
******************/
void delay_us(int n)
{
	int i;
	int j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<2;j++)
		{
			asm("nop");
		}
	}
}

void delay_ms(uint n)    //80M频率下的ms延时
{
	int i;
	int j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<3000;j++)
		{
			asm("nop");
		}
	}
}
void delay(int ms) 
   {    int ii,jj;  
     if (ms<1) ms=1; 
       for(ii=0;ii<ms;ii++)     
        for(jj=0;jj<2600;jj++);    //busclk:16MHz--1ms     (12*1335/16000)=1    6*8000/48000 
   } 
   
void delay_key(int t){                //软仿0.116s
  int i = 0;
  int j;
  t = t * 100;
  for (;i < t;i++)
    for(j = 0;j < 160;j++)
      ;
}     