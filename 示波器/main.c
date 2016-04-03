#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "12864.h"
#include "24c02.h"
#include "key.h"

/******************************************************
*                      ��������                       *
******************************************************/
uchar sec=0;                          //�жϼ���

uchar flag_m=0;
uchar flag_v=0;                       //ɨ���ٶȼ��̱�־λ
uchar flag_r=0;                       //����ѡ���־λ   Ĭ��ѡ��5V  ����ѡ��0.5V
uchar RorP=1;                         //�������л���ͣ��־λ

uint ad[112]=0;
uchar cur[112]=0;                      //��ǰ����Y��������
uchar sav[112]=0;                      //�洢����Y��������

uchar max_V[4]={'0','.','0','0'};
uchar min_V[4]={'0','.','0','0'};




/******************************************************
*                      ʱ�ӳ�Ƶ                       *
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
  ATD0DIEN = 0x00;//��ֹ�������빦��
  ATD0CTL1 = 0x40;//AD�ֱ���ѡ��12λ���Ҳ���ǰ���ŵ�
  ATD0CTL2 = 0x40;//ADģ��������㣬��ֹ�ⲿ��������ֹ�ж�
  ATD0CTL3 = 0x88;//ADת������Ҷ��룬ÿ������1��ͨ������FIFOģʽ
  ATD0CTL4 = 0x00;//����ʱ��Ϊ4�����ڣ�PRS=0��������Ƶ��Ϊ 1M Hz
  ATD0CTL5 = 0x20;//��ͨ��0��ͨ������������ͬʱ����ADת������ 
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
      cur[i] = 63 - (ad[i] / 64.0 +0.5);         // 4V���̴�㴦��
    else cur[i] = 63 - (ad[i] *10 / 64.0 +0.5);   // 0.4V���̴�㴦��
     
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
  PITCFLMT = 0x00;//��ֹPITģ��
  PITCE_PCE0 = 1; //ʹ�ܶ�ʱ��ͨ��0
  PITMUX = 0x00;  //��ʱ��ͨ��0ʹ�� ΢��ʱ��׼0
  PITMTLD0 = 0xff;//����8λ��������ֵ
  PITLD0 = 0x6A00;  //����16λ��������ֵ  ��ʱ���� 1s
  PITINTE = 0x01; //ʹ��PIT��ʱ��ͨ��0�ж�
  PITCFLMT = 0x80;//ʹ��PITģ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//����жϱ�־
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
    judge_key(scan_key());    //ɨ�����
  
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
