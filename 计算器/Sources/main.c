#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "key.h"
#include "1602.h"
#include "delay.h"
#include "calculate.h"
#include "display.h"


unsigned int a=0,b = 0,c=0;
double value = 0;

unsigned char math[MAXLEN];
unsigned int s = 0;
unsigned int i = 0;

void init(){
  init_1602();
  memset(math,0,30*sizeof(unsigned char));    
}

void main(void) {
  /* put your own code here */
  

  
  init();

	DisableInterrupts;


  for(;;) {
  
    judge_key(scan_key());
    
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
