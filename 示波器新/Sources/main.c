#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "12864.h"
#include "24c02.h"
#include "key.h"

/******************************************************
*                      变量定义                       *
******************************************************/
uchar sec=0;                          //中断计数

uchar flag_m=0;
uchar flag_v=0;                       //扫描速度键盘标志位
uchar flag_r=0;                       //量程选择标志位   默认选择5V  按键选择0.5V
uchar RorP=1;                         //程序运行或暂停标志位

uint ad[112]=0;
uchar cur[112]=0;                      //当前波形Y坐标数组
uchar sav[112]=0;                      //存储波形Y坐标数组

uchar max_V[4]={'0','.','0','0'};
uchar min_V[4]={'0','.','0','0'};




/******************************************************
*                      时钟超频                       *
******************************************************/
void SetBusCLK_80M(){
  CLKSEL_PLLSEL = 0;
  PLLCTL_PLLON = 1;
  SYNR =0xc0 | 0x09;                          
  REFDV=0x80 | 0x01; 
  POSTDIV = 0X00;
  _asm(nop);
  _asm(nop);
  while(!(CRGFLG_LOCK==1)) ;
  CLKSEL_PLLSEL = 1;
}

 
/******************************************************
*                          AD                         *
******************************************************/
void init_AD(){
  ATD0DIEN = 0x00;//禁止数字输入功能
  ATD0CTL1 = 0x40;//AD分辨率选择12位，且采样前不放电
  ATD0CTL2 = 0x40;//AD模块快速清零，禁止外部触发，禁止中断
  ATD0CTL3 = 0x88;//AD转换结果右对齐，每个序列1个通道，非FIFO模式
  ATD0CTL4 = 0x00;//采样时间为4个周期，PRS=0，即采样频率为 1M Hz
  ATD0CTL5 = 0x20;//对通道0单通道连续采样，同时启动AD转换序列 
}

void delay_ad(uint c){
  while(c--) _asm(nop);
}
void get_V(){          
  uchar i,j;
  uint *p = ad;                       //156.25    14nop      // 5ms     140
  for(i=0;i<112;i++){                  //312.5us   3
    while(!ATD0STAT2L_CCF0) ;          //625us    12
      *p = ATD0DR0;                    //1.25ms    30
      p++;                           
    if(flag_v==7) _asm(nop);
    else if(flag_v==6){
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);    
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
        _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
        _asm(nop);_asm(nop);
      }    
    else if(flag_v==5) delay_ad(10);    // 10ms   285
    else if(flag_v==4) delay_ad(28);
    else if(flag_v==3) delay_ad(64);
    else if(flag_v==2) delay_ad(138);                    // 20ms  573     */
    else if(flag_v==1) delay_ad(285);
    else delay_ad(573);
  }
  for(i=0;i<112;i++){
    if(flag_r==0)
      cur[i] = 63 - (ad[i] / 64.0 +0.5);         // 4V量程打点处理
    else cur[i] = 63 - (ad[i] *10 / 64.0 +0.5);   // 0.4V量程打点处理
     
  }
  for(i=0;i<112;i++){      
    char a,b;
    if(i<111){
      a = cur[i] - cur[i+1];
      b = cur[i+1] - cur[i];
    }
    if(a >= 6){
      for(j=0;j<=a;j++){
        draw_dot_12864(i,cur[i+1]+j);  
      }
    }else if(b >=6){
      for(j=0;j<=b;j++){
        draw_dot_12864(i,cur[i]+j);  
      }
    }else draw_dot_12864(i,cur[i]);
  }
}


/******************************************************
*                          PIT                        *
******************************************************/
void init_PIT(){
  PITCFLMT = 0x00;//禁止PIT模块
  PITCE_PCE0 = 1; //使能定时器通道0
  PITMUX = 0x00;  //定时器通道0使用 微定时基准0
  PITMTLD0 = 0xff;//设置8位计数器初值
  PITLD0 = 0x6A00;  //设置16位计数器初值  定时周期 1s
  PITINTE = 0x01; //使能PIT定时器通道0中断
  PITCFLMT = 0x80;//使能PIT模块
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//清除中断标志
  sec++;
  if(sec>=100) sec=0;
  if(sec % 2 == 0){   
    init_OSC_12864();
    get_V();
  }
}
#pragma CODE_SEG DEFAULT





void main(void){
  /* put your own code here */

  uint i,j,k;

 // SetBusCLK_80M();
 
  init_12864();
  init_OSC_12864();
  init_PIT();
  init_AD();
  
  
  write_str_12864(" 5",0x80+7,2);
  write_str_12864(" V",0x90+7,2);
  write_str_12864(" 1",0x88+7,2);
  write_str_12864(" L",0x98+7,2); 
    
 
  
  get_V();
  
  EnableInterrupts;RorP=1;
  
  for(;;) {
    judge_key(scan_key());    //扫描键盘
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
