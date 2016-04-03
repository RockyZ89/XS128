#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "key.h"
#include "12864.h"
#include "delay.h"
#include "24c02.h"
#include "model.h"

uchar L=0,S=0,B=0,R=0;
uchar len[2]={0};
uchar ang[3]={0};
uchar rad[2]={0};
uchar SetL=0,SetA=0,SetR=0;
uchar ct=0;


/********扫描按键返回按键号**********/



//keyscan
int scan_key() 
{
   
   unsigned char key=0,num=0; 
   key_dir=0xff;
   key_out=0x01;
   key_dir=0x0f;
   key=key_in&0xf0;
   while(key!=0x00) 
   {
      delay(10);
      key=key_in&0xf0;
      while(key!=0x00)
      {
         key=key_in&0xf0;
         switch(key)
         {
            case 0x10: num=1;  break;
            case 0x20: num=2;  break;
            case 0x40: num=3;  break;
            case 0x80: num=4;  break;
            default:break; 
         }
         while(key!=0x00)
            key=key_in&0xf0; 
      }
      
   }
   
   
  
 
   key_dir=0xff;
   key_out=0x02;
   key_dir=0x0f;
   key=key_in&0xf0; 
   while(key!=0x00) 
   {
      delay(10);
      key=key_in&0xf0; 
      while(key!=0x00)
      {
         key=key_in&0xf0; 
         switch(key)
         {
            case 0x10: num=5;  break;
            case 0x20: num=6;  break;
            case 0x40: num=7;  break;
            case 0x80: num=8;  break;
            default:break; 
         }
         while(key!=0x00)
            key=key_in&0xf0;  
      }
      
   }
   
 
 
   
   key_dir=0xff;
   key_out=0x04;
   key_dir=0x0f;
   key=key_in&0xf0; 
   while(key!=0x00) 
   {
      delay(10);
      key=key_in&0xf0; 
      while(key!=0x00)
      {
         key=key_in&0xf0; 
         switch(key)
         {
            case 0x10: num=9;  break;
            case 0x20: num=10;  break;
            case 0x40: num=11;  break;
            case 0x80: num=12;  break;
            default:break; 
         }
         while(key!=0x00)
           key=key_in&0xf0;  
      }
      
   }  
   
   
 
 
   key_dir=0xff;
   key_out=0x08;
   key_dir=0x0f;
   key=key_in&0xf0; 
   while(key!=0x00) 
   {
      delay(10);
      key=key_in&0xf0; 
      while(key!=0x00)
      {
         key=key_in&0xf0; 
         switch(key)
         {
            case 0x10: num=13;  break;
            case 0x20: num=14;  break;
            case 0x40: num=15;  break;
            case 0x80: num=16;  break; 
            default:break;
         }
         while(key!=0x00) 
         {
            key=key_in&0xf0;
         }
            
      }
      
   }
          
   return num;
   
}


void judge_key(int key){
  switch(key){                              
    case 0:break;
    case 1:
      if(S && ct<2){
        if(S==1){
          len[ct]=7;
        } 
        else if(S==2){
          ang[ct]=7;
        } 
        else{
          rad[ct]=7;
        }
        ct++;
        write_dat_12864('7');
          
      }
      break;
      
    case 2:
      if(S && ct<2){
        if(S==1){
          len[ct]=8;
        } 
        else if(S==2){
          ang[ct]=8;
        } 
        else{
          rad[ct]=8;
        }
        ct++;
        write_dat_12864('8');
          
      }
      break;
      
    case 3:
      if(S && ct<2){
        if(S==1){
          len[ct]=9;
        } 
        else if(S==2){
          ang[ct]=9;
        } 
        else{
          rad[ct]=9;
        }
        ct++;
        write_dat_12864('9');
          
      }
      break;
          
    case 5:
      if(S && ct<2){
        if(S==1){
          len[ct]=4;
        } 
        else if(S==2){
          ang[ct]=4;
        } 
        else{
          rad[ct]=4;
        }
        ct++;
        write_dat_12864('4');
          
      }
      break;
      
    case 6:
      if(S && ct<2){
        if(S==1){
          len[ct]=5;
        } 
        else if(S==2){
          ang[ct]=5;
        } 
        else{
          rad[ct]=5;
        }
        ct++;
        write_dat_12864('5');
          
      }
      break;
      
    case 7:
      if(S && ct<2){
        if(S==1){
          len[ct]=6;
        } 
        else if(S==2){
          ang[ct]=6;
        } 
        else{
          rad[ct]=6;
        }             
        ct++;
        write_dat_12864('6');
          
      }
      break;
      

    case  9:
      if(S && ct<2){
        if(S==1){
          len[ct]=1;
        } 
        else if(S==2){
          ang[ct]=1;
        } 
        else{
          rad[ct]=1;
        }
        ct++;
        write_dat_12864('1');
          
      }
      break;
      
           
    case 10:
      if(S && ct<2){
        if(S==1){
          len[ct]=2;
        } 
        else if(S==2){
          ang[ct]=2;
        } 
        else{
          rad[ct]=1;
        }
        ct++;
        write_dat_12864('2');
          
      }
      break;
      
    case 11:
      if(S && ct<2){
        if(S==1){
          len[ct]=3;
        } 
        else if(S==2){
          ang[ct]=3;
        } 
        else{
          rad[ct]=3;
        }
        ct++;
        write_dat_12864('3');
          
      }
      break;
      
    case 13:
      if(S && ct<2){
        if(S==1){
          len[ct]=0;
        } 
        else if(S==2){
          ang[ct]=0;
        } 
        else{
          rad[ct]=0;
        }
        ct++;
        write_dat_12864('0');
          
      }
      break;
      
    case 4:
      L = ~L;
      break;
      
    case 8:
      R = ~R;
    /*  if(R){
        write_str_12864("R:--cm",0x80);    
      } 
      else {
        write_str_12864("L:--cm  ",0x80);
        write_str_12864("A:---C",0x84);  
      }    */
      break;
      
    case 12:
      B = ~B;
      if(B){
        write_str_12864("Braking",0x80); 
        write_str_12864("       ",0x80+4);
        write_str_12864("       ",0x90);   
      } 
      if(!B){
        write_str_12864("L:--cm ",0x80);
        write_str_12864("A:---C",0x84);  
        write_str_12864("R:--cm",0x90); 
      }
      break;

    case 16:
      S++;
      if(S==1){
        write_com_12864(0x0e);    //开光标
        write_com_12864(0x80+1);
        ct=0;
      }
      else if(S==2){
        write_com_12864(0x0e);    //开光标
        write_com_12864(0x84+1);
        ct=0;
      }
      else if(S==3){
        write_com_12864(0x0e);    //开光标
        write_com_12864(0x90+1);
        ct=0;
      }
      else{
        S = 0;
        write_com_12864(0x0c);    //关光标
        ct=0;
        
        SetL = len[0] *10 + len[1];
        SetA = ang[0] *10 + ang[1];
        SetR = rad[0] *10 + rad[1];
      }
         
      break;   
       
  }
}


