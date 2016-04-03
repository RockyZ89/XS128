#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "key.h"
#include "12864.h"
#include "delay.h"
#include "24c02.h"


extern uchar RorP, flag_m, flag_r, flag_v;
extern uchar cur[112], sav[112];
extern uint ad[112];
extern uchar max_V[4];
extern uchar min_V[4];


/***********************扫描按键返回按键号*********************/
int scan_key(){
  int row=0, col=0, key=0, temp=0;
  DDR_key = 0xff;
  PORT_key = 0xf0;
  DDR_key = 0x00;
  temp = PORT_key;
  if(temp!=0xf0){
    delay(8);
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
/***********************按键响铃*************************/
void buzzer_l(){                //长鸣
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay(20);
  PORTK_PK7 = 1;
}
void buzzer_s(){                //短鸣
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay(5);
  PORTK_PK7 = 1;
}

/*****************************判断按键**********************/
void judge_key(int key){
  uchar i,j;
  uint max=0;
  uint min= ~max;
  double max_v,min_v;
  char a,b;
  switch(key){
    case 0:break;
             
    case 4:
      if(flag_m==0){       
        DisableInterrupts;RorP=0;
        buzzer_s();                        //按下即短鸣
        flag_r++;    
        if(flag_r==1){                     
          write_str_12864(".5",0x80+7,2);  //量程选择0.5V 
          init_OSC_12864(); 
          get_V();
        } 
        else{
          flag_r = 0;
          write_str_12864(" 5",0x80+7,2);   //量程默认选择5V
          init_OSC_12864();
          get_V();
        }
        EnableInterrupts;RorP=1;
      }
      break;  
      
    case 7:
      if(flag_m==0){
        DisableInterrupts;RorP=0; 
        buzzer_s();
        flag_v++;         //默认5ms打一个点 
        if(flag_v==1){     //按键一次10ms
          write_str_12864(" 2",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==2){
          write_str_12864(" 3",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==3){
          write_str_12864(" 4",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==4){
          write_str_12864(" 5",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==5){
          write_str_12864(" 6",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==6){
          write_str_12864(" 7",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==7){
          write_str_12864(" 8",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else{     
          flag_v = 0;
          write_str_12864(" 1",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        EnableInterrupts;RorP=1;
      }
      break;
         
    case 8:
      if(flag_m==0){
        DisableInterrupts;RorP=0; 
        buzzer_s();
        if(flag_v==0) flag_v=8; 
        flag_v--;         //默认5ms打一个点 
        if(flag_v==7){     //按键一次10ms
          write_str_12864(" 8",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==6){
          write_str_12864(" 7",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==5){
          write_str_12864(" 6",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==4){
          write_str_12864(" 5",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==3){
          write_str_12864(" 4",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==2){
          write_str_12864(" 3",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else if(flag_v==1){
          write_str_12864(" 2",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        else{     
          write_str_12864(" 1",0x88+7,2);
          init_OSC_12864();
          get_V();
        }
        EnableInterrupts;RorP=1;
      }
      break;
    
    case 11:
      if(RorP==0 && flag_m==0){     
        for(i=0;i<112;i++){
          sav[i] = cur[i];
          write_byte_24c02(sav[i],i);
        } 
        write_byte_24c02(flag_r,120);
        write_byte_24c02(flag_v,121);            
        buzzer_l();
      }
      break;
      
    case 12:
      if(flag_m==0){
        DisableInterrupts;RorP=0;
        init_OSC_12864();
        for(i=0;i<112;i++){
          sav[i] = read_byte_24c02(i);
          cur[i] = sav[i];
          draw_dot_12864(i,sav[i]);
        }
        for(i=0;i<112;i++){           
          if(i<111){
            a = sav[i] - sav[i+1];
            b = sav[i+1] - sav[i];
          }
          if(a >= 6){
            for(j=0;j<=a;j++){
              draw_dot_12864(i,sav[i+1]+j);  
            }
          }else if(b >=6){
            for(j=0;j<=b;j++){
              draw_dot_12864(i,sav[i]+j);  
            }
          }else draw_dot_12864(i,sav[i]); 
        }
        flag_r = read_byte_24c02(120);
        flag_v = read_byte_24c02(121);
        write_str_12864(" V",0x90+7,2);
        write_str_12864(" L",0x98+7,2);
        if(flag_r==1)                     
          write_str_12864(".5",0x80+7,2);  //量程选择0.5V  
        else
          write_str_12864(" 5",0x80+7,2);   //量程默认选择5V
        if(flag_v==7)
          write_str_12864(" 8",0x88+7,2);
        else if(flag_v==6)
          write_str_12864(" 7",0x88+7,2);
        else if(flag_v==5)
          write_str_12864(" 6",0x88+7,2);
        else if(flag_v==4)
          write_str_12864(" 5",0x88+7,2);
        else if(flag_v==3)
          write_str_12864(" 4",0x88+7,2);
        else if(flag_v==2)
          write_str_12864(" 3",0x88+7,2);
        else if(flag_v==1)
          write_str_12864(" 2",0x88+7,2);
        else     
          write_str_12864(" 1",0x88+7,2);  
        buzzer_l();
      }
      break;
      
    case 15:
      buzzer_s();
      DisableInterrupts;RorP=0;
      flag_m = 1;
      write_str_12864("  ",0x80+7,2);
      write_str_12864("  ",0x90+7,2);
      write_str_12864("  ",0x88+7,2);
      write_str_12864("  ",0x98+7,2);
      clr_GDR_12864(0);
      for(i=0;i<112;i++){
        if(ad[i]>max) max=ad[i];
        if(ad[i]<min) min=ad[i];
      }
      max_v = max / 4096.0 * 5;
      min_v = min / 4096.0 * 5;
      max = max_v * 100;
      min = min_v * 100;
      max_V[0] = max / 100 + 0x30;
      max_V[2] = max / 10 % 10 + 0x30;
      max_V[3] = max % 10 + 0x30;
      min_V[0] = min / 100 + 0x30;
      min_V[2] = min / 10 % 10 + 0x30;
      min_V[3] = min % 10 + 0x30;
      write_str_12864("max:",0x80,4);
      write_str_12864(max_V,0x82,4);
      write_str_12864("V",0x84,1);
      write_str_12864("min:",0x90,4);
      write_str_12864(min_V,0x92,4);
      write_str_12864("V",0x94,1);
      write_str_12864("div:",0x88,4);
      if(flag_v==7)
          write_str_12864("156.25us",0x88+2,8);
        else if(flag_v==6)
          write_str_12864("312.5us",0x88+2,7);
        else if(flag_v==5)
          write_str_12864("625us",0x88+2,5);
        else if(flag_v==4)
          write_str_12864("1.25ms",0x88+2,6);
        else if(flag_v==3)
          write_str_12864("2.5ms",0x88+2,5);
        else if(flag_v==2)
          write_str_12864("5ms",0x88+2,3);
        else if(flag_v==1)
          write_str_12864("10ms",0x88+2,4);
        else     
          write_str_12864("20ms",0x88+2,4);
      break;
    case 16:
      buzzer_s();
      if(RorP==0){
        flag_m = 0;
        write_str_12864("         ",0x80,9);
        write_str_12864("         ",0x90,9);
        write_str_12864("              ",0x88,14);
        write_str_12864(" V",0x90+7,2);
        write_str_12864(" L",0x98+7,2);
        if(flag_r==1)                     
          write_str_12864(".5",0x80+7,2);  //量程选择0.5V  
        else
          write_str_12864(" 5",0x80+7,2);   //量程默认选择5V
        if(flag_v==7)
          write_str_12864(" 8",0x88+7,2);
        else if(flag_v==6)
          write_str_12864(" 7",0x88+7,2);
        else if(flag_v==5)
          write_str_12864(" 6",0x88+7,2);
        else if(flag_v==4)
          write_str_12864(" 5",0x88+7,2);
        else if(flag_v==3)
          write_str_12864(" 4",0x88+7,2);
        else if(flag_v==2)
          write_str_12864(" 3",0x88+7,2);
        else if(flag_v==1)
          write_str_12864(" 2",0x88+7,2);
        else     
          write_str_12864(" 1",0x88+7,2);
        EnableInterrupts;RorP=1;
      } 
      else{
        write_str_12864("         ",0x80,9);
        write_str_12864("         ",0x90,9);
        write_str_12864("              ",0x88,14);
        write_str_12864(" V",0x90+7,2);
        write_str_12864(" L",0x98+7,2);
        if(flag_r==1)                     
          write_str_12864(".5",0x80+7,2);  //量程选择0.5V  
        else
          write_str_12864(" 5",0x80+7,2);   //量程默认选择5V
        if(flag_v==7)
          write_str_12864(" 8",0x88+7,2);
        else if(flag_v==6)
          write_str_12864(" 7",0x88+7,2);
        else if(flag_v==5)
          write_str_12864(" 6",0x88+7,2);
        else if(flag_v==4)
          write_str_12864(" 5",0x88+7,2);
        else if(flag_v==3)
          write_str_12864(" 4",0x88+7,2);
        else if(flag_v==2)
          write_str_12864(" 3",0x88+7,2);
        else if(flag_v==1)
          write_str_12864(" 2",0x88+7,2);
        else     
          write_str_12864(" 1",0x88+7,2);
        
        DisableInterrupts;RorP=0;
      }
      break;
  }
}
 