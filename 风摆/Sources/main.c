#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <math.h>    //Keil library  
#include "key.h"
//#include "12864.h"
#include "delay.h"
#include "mpu6050.h"
//#include "24c02.h"
#include "model.h"     

/******************************************************
*                      ��������                       *
******************************************************/ 
#define Hight 70.0

#define ZrAgX 17
#define ZrAgY 12
#define ZrAgRtX 3
#define ZrAgRtY 29

#define ErAgX 43
#define ErAgY 43
#define ErAgRtX 47
#define ErAgRtY 11 

#define DrAgRtX 300                //400 30�Ƚ�  9s
#define DrAgRtY 300

int anglex,angley,anglez;
int gyrox,gyroy,gyroz;
int Eranglex,Erangley,Eranglez;
int Ergyrox,Ergyroy,Ergyroz;
int angleX,angleY;
int angrateX,angrateY;            //�Ƕȼ����ٶ�Ҫ /10


int max_ang_X=0,max_rat_X=0;
int max_ang_mX=0,max_rat_mX=0; 
int max_ang_Y=0,max_rat_Y=0;
int max_ang_mY=0,max_rat_mY=0;

uchar i;

                             
double LtoA(uchar l){         //���볤�ȣ������Ƕ�
  return ((180 / 3.1415925) * atan(l / Hight));             
}


struct PID { 
unsigned int SetPoint; // �趨Ŀ�� Desired Value 
unsigned int Proportion; // �������� Proportional Const 
unsigned int Integral; // ���ֳ��� Integral Const 
unsigned int Derivative; // ΢�ֳ��� Derivative Const 
unsigned int LastError; // Error[-1] 
unsigned int PrevError; // Error[-2] 
unsigned int SumError; // Sums of Errors 
}; 
struct PID spid; // PID Control Structure 

void SetCLK_40M(){        
  CLKSEL=0X00;
  PLLCTL_PLLON=1;      // ����  PLL          
  SYNR=0x40 | 0x04;      // pllclock=2*osc*(1+SYNR)/(1+REFDV)=80MHz;  
  REFDV=0x80 | 0x01;           /*1_0x80|0x01; 2_ 3_0xc0|0x03 4_ 0x40|0x04; 5_0XC0 | 0X05;*/     
  POSTDIV=0x00;              
  _asm(nop);                    // BUS CLOCK=40M   
  _asm(nop);  
  while(!(CRGFLG_LOCK==1));      //�ȴ�PLL�ȶ�
    CLKSEL_PLLSEL =1; //fBUS���ø���PLLƵ���趨         
}


void main(void)  
{       


   DisableInterrupts;        
   SetCLK_40M();   // PIT_Init(); 
  
   delay_key(30);  //�ϵ���ʱ  
   init_PWM();
   init_PIT();
   I2C_Init();    
   MPU6050_Init(); //��ʼ��MPU6050   
   init_12864();    
  // EnableInterrupts;  
                                        // 0ǰ 1�� ��4 ��5
          
      buzzer_s(); 
   
   write_str_12864("L:--cm ",0x80);
   write_str_12864("A:--C",0x84);  
   write_str_12864("R:--cm",0x90); 
                  
  //motor0_open(40);
  
//  motor0_open(50);
//   motor1_open(50);
// motor4_open(50);
motor5_open(0);   

   while(1){
      
     
     
     /* READ_MPU6050();
      if(angleY > ZrAgY){
        motor0_open(100);
        motor1_close();
        READ_MPU6050();
        if(angrateY < ZrAgRtY){      
          motor0_open(100);
          motor1_close();
        }
        READ_MPU6050();
        if(angleY < ZrAgY && angrateY > (ZrAgRtY+DrAgRtY)){
          motor0_close();
        }
      }
      READ_MPU6050();
      if(angleY < ZrAgY){
        motor1_open(100);
        motor0_close();
        READ_MPU6050();
        if(angrateY > ZrAgRtY){      
          motor1_open(100);
          motor0_close();
        }
        READ_MPU6050();
        if(angleY > ZrAgY && angrateY < (ZrAgRtY+DrAgRtY)){
          motor1_close();
        }
      }
      READ_MPU6050();
      if(angleX > ZrAgX){
        motor4_open(100);
        motor5_close();
        READ_MPU6050();
        if(angrateX < ZrAgRtX){      
          motor4_open(100);
          motor5_close();
        }
        READ_MPU6050();
        if(angleX < ZrAgX && angrateX > (ZrAgRtX+DrAgRtX)){
          motor4_close();
        }
      }
      READ_MPU6050();
      if(angleX < ZrAgX){
        motor5_open(100);
        motor4_close();
        READ_MPU6050();
        if(angrateX > ZrAgRtX){      
          motor5_open(100);
          motor4_close();
        }
        READ_MPU6050();
        if(angleX > ZrAgX && angrateX < (ZrAgRtX+DrAgRtX)){
          motor5_close();
        }
      }
      READ_MPU6050();
      if(    ((angleY-ZrAgY<ErAgY || angleY-ZrAgY>-ErAgY) && (angleX-ZrAgX<ErAgX || angleX-ZrAgX>-ErAgX))\
      || ( (angrateY-ZrAgRtY<ErAgRtY || angrateY-ZrAgRtY>-ErAgRtY) && (angrateX-ZrAgRtX<ErAgRtX || angrateX-ZrAgRtX>-ErAgRtX) )     ){
        motor0_close();
        motor1_close();
        motor4_close();
        motor5_close();
        buzzer_l();
      }   */            
      
     
   } 
}