#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"

/******************
      —” ±us
******************/
void delay_us(int t){
  int i, j;
  for (i=0; i<t; i++)
    for(j=0; j<16; j++)
      ;
}

/******************
      —” ±ms
******************/
void delay_ms(int t){
  int i = 0;
  int j;
  t = t * 100;
  for (;i < t;i++)
    for(j = 0;j < 160;j++)
      ;
}