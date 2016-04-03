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

extern unsigned int s;
extern double value;

int transform(char result[],double value)
{
    int i=0,j=0;
    char temp;
    long int num = value*100;
    for(;num>0;)
    {
        if(i<2)
        {
            result[i] = num%10 + '0';
            i++;
            num /= 10;
        }
        if(i == 2)
        {
            result[i] = '.';
            i++;
        }
        if(i>2)
        {
            result[i] = num%10 + '0';
            i++;
            num /= 10;
        }
    }
    for(;j <= (i-1)/2;j++) {
      temp = result[j];
      result[j] = result[i-1-j];
      result[i-1-j] = temp;
    }
    return i;
}


void display_value(double value,unsigned int sign){
  char result[20]={'0'};
  int length=0,t=0,i=0;
  
  
  
  if(s == 1) {
    if(value < 0){
      write_ch_1602(0xc0 + 1,'-');
      value = -value;
      value = value + 0.005;
      length = transform(result,value);
      write_str_1602(result,0xc2,length);
    } 
    else{
      value = value + 0.005;
      length = transform(result,value);
      write_str_1602(result,0xc1,length);
    }
    
  }
  if(s == 2){   
    if(value < 0){
      write_ch_1602(0xc0 + 1,'-');
      value = -value;
      value = (int)(value + 0.005);
      
      length = transform(result,value);
      write_str_1602("0x",0xc0+2,2);
      write_str_1602(result,0xc2,length);
    } 
    else{
      value = value + 0.005;
      length = transform(result,value);
      write_str_1602(result,0xc1,length);
    }
  } 
    ;
 /* if(s == 3)
    length = sprintf(result,"0x%-.2x",value);
    write_string1602(result,0xc1,length);
    */
}