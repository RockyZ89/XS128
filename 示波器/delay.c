#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"

/******************
      ��ʱus
******************/
void delay_us(int n)         //10us 
{        
	while(n--){
	    _asm(nop);
	}
} 

/******************
      ��ʱms
******************/
void delay_ms(uint t){
  int i,j,k;
  for(k=0;k<t;k++)
    for(i=0;i<27;i++)
      for(j=0;j<50;j++)
        fo(dd )
        ;
}

void delay(int t){                //���0.116s
  int i = 0;
  int j;
  t = t * 100;
  for (;i < t;i++)
    for(j = 0;j < 160;j++)
      ;
}