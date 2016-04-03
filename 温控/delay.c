#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"

/******************
      —” ±us
******************/
void delay_us(uint t){
  int i, j;
  for (i=0; i<t; i++)
    for(j=0; j<16; j++)
      ;
}

/******************
      —” ±ms
******************/
void delay_ms(uint t){
  int i,j,k;
  for(k=0;k<t;k++)
    for(i=0;i<27;i++)
      for(j=0;j<50;j++)
        ;
}

void delay(int t){
  int i = 0;
  int j;
  t = t * 100;
  for (;i < t;i++)
    for(j = 0;j < 160;j++)
      ;
}