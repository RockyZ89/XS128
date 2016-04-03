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

extern unsigned int a, b, c, s, i;
extern unsigned char math[MAXLEN];
extern double value;

void init_calc(){
  init_1602();
  memset(math,0,30*sizeof(unsigned char));
  s=0;
  i=0;
}

/********扫描按键返回按键号**********/
int scan_key(){
  int r=0, c=0, key=0, temp=0;
  DDR_key = 0xff;
  PORT_key = 0xf0;
  DDR_key = 0x00;
  temp = PORT_key;
  if(temp!=0xf0){
    delay_ms(8);
    if(temp!=0xf0){
      switch(temp){
        case 0xe0 : c=1; break;                    
        case 0xd0 : c=2; break;
        case 0xb0 : c=3; break;
        case 0x70 : c=4; break;  
      }
      DDR_key = 0xff;
      PORT_key = 0x0f;
      DDR_key = 0x00;
      temp = PORT_key;
      switch(temp){
        case 0x0e : r=1; break;
        case 0x0d : r=2; break;
        case 0x0b : r=3; break;
        case 0x07 : r=4; break;
      }  
      while(1) {
        DDR_key = 0xff;
        PORT_key = 0xf0;
        DDR_key = 0x00;
        temp = PORT_key;
        temp &= 0xf0;
        if(temp == 0xf0)
          break;
      }
      key = 4*(r-1) + c;
      if(key>0) return key;
    }
  }
  else return 0;
}

void judge_key(int key){
  switch(key){
    case 0:break;
    case 1:if(s) init_calc();if(b){b=0;i++;}math[i] = '7';write_ch_1602(0x80 + i,'7');i++;break;
    case 2:if(s) init_calc();if(b){b=0;i++;}math[i] = '8';write_ch_1602(0x80 + i,'8');i++;break;
    case 3:if(s) init_calc();if(b){b=0;i++;}math[i] = '9';write_ch_1602(0x80 + i,'9');i++;break;
   
    case 4:
      b++;
      if(b==5) b=1;
      if(s) init_calc();
      if(b==1) {
       math[i] = '+';
       write_ch_1602(0x80 + i,'+');
      }
      if(b==2) {
       math[i] = '-';
       write_ch_1602(0x80 + i,'-');
      }
      if(b==3) {
       math[i] = '*';
       write_ch_1602(0x80 + i,'*');
      }
      if(b==4) {
       math[i] = '/';
       write_ch_1602(0x80 + i,'/');
      }
      break;
               
    case 5:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '4';write_ch_1602(0x80 + i,'4');i++;break;
    case 6:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '5';write_ch_1602(0x80 + i,'5');i++;break;
    case 7:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '6';write_ch_1602(0x80 + i,'6');i++;break;
    
    case 8:
      a++;
      if(a==3) a=1;
      if(b){b=0;i++;}
      if(s) init_calc();
      if(a==1){
        math[i] = '^';
        write_ch_1602(0x80 + i,'^');
       
      }
      if(a==2){
        math[i] = '#';
        write_ch_1602(0x80 + i,'#');
        
      }
      break;
    
    case  9:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '1';write_ch_1602(0x80 + i,'1');i++;break;
    case 10:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '2';write_ch_1602(0x80 + i,'2');i++;break;
    case 11:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '3';write_ch_1602(0x80 + i,'3');i++;break;
    case 13:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '0';write_ch_1602(0x80 + i,'0');i++;break;
    case 14:if(s) init_calc();if(a||b){a=0;b=0;i++;}math[i] = '.';write_ch_1602(0x80 + i,'.');i++;break;
   
    case 15:
      if(a||b){a=0;b=0;i++;}
      i--;
      write_ch_1602(0x80+i, ' ');
      write_com_1602(0x80+i);    //回显光标
      break;
    case 12:
      c++;
      if(c==3) c=1;
      if(a||b){a=0;b=0;i++;}
      if(s) init_calc();
      if(c==1){
        math[i] = '(';
        write_ch_1602(0x80 + i,'(');
        i++;
      }
      if(c==2){
        math[i] = ')';
        write_ch_1602(0x80 + i,')');
        i++;
      }
      break;
    
    case 16:
      s++;
      if(s==4) s=1;
      if(a||b){a=0;b=0;i++;}
      write_ch_1602(0xc0,'=');
      value = calc(math,0,strlen(math)-1);
      display_value(value,s);   
      break;    
    }
}
