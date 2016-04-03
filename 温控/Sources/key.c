#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "key.h"
#include "12864.h"
#include "delay.h"
#include "24c02.h"

extern uint sec;
extern uint  s, c;
extern uchar st[4];
extern double set_T;
extern flag_T;


/********扫描按键返回按键号**********/
int scan_key(){
  int row=0, col=0, key=0, temp=0;
  DDR_key = 0xff;
  PORT_key = 0xf0;
  DDR_key = 0x00;
  temp = PORT_key;
  if(temp!=0xf0){
    delay(10);
    if(temp!=0xf0){
      switch(temp){
        case 0xe0 : col=1; break;                    
        case 0xd0 : col=2; break;
        case 0xb0 : col=3; break;
        case 0x70 : col=4; break;   
      }
      DDR_key = 0xff;
      
      PORT_key = 0x0f;
      DDR_key = 0x00;
      temp = PORT_key;
      switch(temp){
        case 0x0e : row=1; break;
        case 0x0d : row=2; break;
        case 0x0b : row=3; break;
        case 0x07 : row=4; break;
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
      key = 4*(row-1) + col;
      if(key>0) return key;
    }
  }
  else return 0;
}

void judge_key(int key){

  switch(key){
    case 0:break;
    case 1:if(s && c<2){st[c]='7';write_dat_12864('7');write_byte_24c02(st[c],c);c++;}break;
    case 2:if(s && c<2){st[c]='8';write_dat_12864('8');write_byte_24c02(st[c],c);c++;}break;
    case 3:if(s && c<2){st[c]='9';write_dat_12864('9');write_byte_24c02(st[c],c);c++;}break;
                                                              //写入设置温度
               
    case 5:if(s && c<2){st[c]='4';write_dat_12864('4');write_byte_24c02(st[c],c);c++;}break;
    case 6:if(s && c<2){st[c]='5';write_dat_12864('5');write_byte_24c02(st[c],c);c++;}break;
    case 7:if(s && c<2){st[c]='6';write_dat_12864('6');write_byte_24c02(st[c],c);c++;}break;
    

    case  9:if(s && c<2){st[c]='1';write_dat_12864('1');write_byte_24c02(st[c],c);c++;}break;      
    case 10:if(s && c<2){st[c]='2';write_dat_12864('2');write_byte_24c02(st[c],c);c++;}break;
    case 11:if(s && c<2){st[c]='3';write_dat_12864('3');write_byte_24c02(st[c],c);c++;}break;
    case 13:if(s && c<2){st[c]='0';write_dat_12864('0');write_byte_24c02(st[c],c);c++;}break;
  //  case 14:if(s && c<2){st[c]='.';write_dat_12864('.');write_byte_24c02(st[c],c);c++;}break;
   
  /*  case 15:                           
      if(a||b){a=0;b=0;i++;}
      i--;
      write_ch_1602(' ',0x80+i );
      write_com_1602(0x80+i);    //回显光标
      break;     */

    case 16:
      s = ~s;
      
      if(s){                     //进入设置模式
        flag_T = 1;
        DisableInterrupts;
        sec=0;
        c = 0;                //清设置温度数组
        write_str_12864("--",0x80+2,2);
        write_com_12864(0x0e);    //开光标
        write_com_12864(0x82);      //光标显示在横线处
        set_T = (st[0]-'0')*10  + (st[1]-'0') ;  
      } 
      else{                 //退出设置模式，开始动态响应
        flag_T = 1;
        write_com_12864(0x0c);    //关光标
        EnableInterrupts;    
      } 
      break;   
       
  }
}
