#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h" 
#include "delay.h"
#include "12864.h"
#include "key.h"
#include "24c02.h"

/******************************************************
*                      ��������                       *
******************************************************/
double now_T=0;      //�¶�
double set_T=0;
uchar nt[5]={'0','0','.','0',' '};
uchar st[2]={'0','0'};

double ek=0;         //PID
double ep=0;          
uint pid_dtyh=100,pid_dtym=180;         
int  pid_out=0;
             
uint sec=0;                         //��������ʱ��RT
uint l_sec=0;                       //��Ӧʱ��AT

uint val[40]={0};                   //�˲�����

uint s=0,c=0;                     //ģʽ��־λ�������¶������±�

uint flag_T=1;                  //AT������һ�α�־λ




/******************************************************
*                          AD                         *
******************************************************/
void init_AD(){
  ATD0DIEN = 0x00;//��ֹ�������빦��
  ATD0CTL1 = 0x20;//AD�ֱ���ѡ��10λ���Ҳ���ǰ���ŵ�
  ATD0CTL2 = 0x40;//ADģ��������㣬��ֹ�ⲿ��������ֹ�ж�
  ATD0CTL3 = 0x88;//ADת������Ҷ��룬ÿ������1��ͨ������FIFOģʽ
  ATD0CTL4 = 0x67;//����ʱ��4�����ڣ�PRS=3
  ATD0CTL5 = 0x20;//��ͨ��0��ͨ������������ͬʱ����ADת������ 
}

void get_T(){ 
  uint i,result;
  uint temp=0; 
                                           
  for(i=0;i<40;i++){
    while(!ATD0STAT2L_CCF0) ;
    val[i] = ATD0DR0;
  }                                     //ADƽ���˲�
  for(i=1;i<39;i++)
    temp =  temp + val[i];
  result = temp / 38.0; 
                        
  now_T = result * 500.0 / 1024.0 + 0.05;           //ʵʱ�¶���ֵ

  temp = (int)(now_T * 10);                        //ʵʱ�¶�����
  nt[0] = temp / 100 + 0x30;                                      
  nt[1] = temp / 10 % 10 + 0x30;
  nt[3] = temp % 10 +0x30; 
  
}



/******************************************************
*                          PIT                        *
******************************************************/
void init_PIT(){
  PITCFLMT = 0x00;//��ֹPITģ��
  PITCE_PCE0 = 1; //ʹ�ܶ�ʱ��ͨ��0
  PITMUX = 0x00;  //��ʱ��ͨ��0ʹ�� ΢��ʱ��׼0
  PITMTLD0 = 0xff;//����8λ��������ֵ
  PITLD0 = 0x7A11;//����16λ��������ֵ  ��ʱ���� 1s
  PITINTE = 0x01; //ʹ��PIT��ʱ��ͨ��0�ж�
  PITCFLMT = 0x80;//ʹ��PITģ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//����жϱ�־
  sec++;
  if(sec>=100) sec=0; 
  draw_curve_12864(now_T);     //��ʵʱ�¶�����ͼ
  write_num_12864(sec,0x86);   //12864��ʾ����ʱ��RT
  write_str_12864(nt,0x92,5);  //12864��ʾʵʱ�¶�
}
#pragma CODE_SEG DEFAULT



/******************************************************
*                          PWM                        *
******************************************************/
void init_PWM(){
  PWME = 0x00;          //��ֹPWMģ��
  
  PWMPRCLK = 0x01;      //CLOCKA 2��Ƶ
  PWMSCLA = 2;          //SAΪA��4��Ƶ
   
  PWMCLK_PCLK0 = 1;    //ѡ��SA��ΪPWMʱ��
  PWMCLK_PCLK1 = 1;  
  
  
  PWMPER0 = 200;       //ͨ��0���ڼĴ�������Ϊ200
  PWMPER1 = 200;       //ͨ��1���ڼĴ�������Ϊ200
  
  PWMDTY0 = 100;         //ͨ��0ռ�ձȼĴ�������
  PWMDTY1 = 100;         //ͨ��1ռ�ձȼĴ�������
  
  PWMPOL_PPOL0 = 1;     //PWM����ѡ���ȸߺ��
  PWMPOL_PPOL1 = 1; 
  
  PWMCAE = 0x00;        //��������
  PWMCTL = 0x00;        //������
  
  PWME_PWME0 = 1;
  PWME_PWME1 = 1; 
}

void heat_PWM(uint temp){    //����ģ����� temp ռ�ձ�ֵ    0-200�ɵ�
    PWMDTY0 = temp;
  //  PWME_PWME0 = 1;            
}

void fan_PWM(uint temp){    //����ģ����� temp ռ�ձ�ֵ    0-200�ɵ�
    PWMDTY1 = temp;
 //   PWME_PWME1 = 1;            
}  

/*****************************************************
*                       ����                         *
*****************************************************/
void adjust_T(){
  ek = set_T - now_T;
  ep = ek / 10.0;
  if(ek > 10){
    heat_PWM(200);
    fan_PWM(0);
  }
  if(ek < -10){
    heat_PWM(0);
    fan_PWM(200);
  }
  if(ek>0 && ek<10){
   	pid_dtyh += pid_out;
   	if(pid_dtyh > 200)
   	  pid_dtyh = 200;
   	if(pid_dtyh < 0)
   	  pid_dtyh = 0;
   	fan_PWM(0);
   	heat_PWM(pid_dtyh); 
  }
  if(ek<0 && ek>-10){
    pid_dtym += pid_out;
    if(pid_dtym > 200)
   	  pid_dtym = 200;
   	if(pid_dtym < 0)
   	  pid_dtym = 0;
    heat_PWM(0);
    fan_PWM(pid_dtym); 
  }
  if(ek< 0.5 && ek >-0.5 && flag_T){
    l_sec = sec;
    write_byte_24c02(l_sec,7);
    write_num_12864(l_sec,0x96);   //��ʾ�ϴ���Ӧ������
    flag_T = 0;   //ֻ��Ӧһ�α�־λ   
  }
}

/*******************PID**************/
int PID(double ek)
{
	float Kp, Ti, T, Td, a0, a1, a2, mn;
	static float ek1 = 0, ek2 = 0, eth;
	int Mn, Out;
	
	Kp = 10;                           //����ϵ��
	Ti = 0;                           //����ϵ��
	Td = 5;                           //΢��ϵ��
	T  = 10;                         //�������ڣ��������ڣ�10ms
	eth = 0.5;                         //��ֵ
	a0 =  Kp * (1 + T/Ti + Td/T);    //�м�ϵ��
	a1 = -Kp * (1 + 2*Td/T);
	a2 =  Kp * Td/T;
	mn  = 0;                         //������
	Mn  = 0;
	Out = 0;                         //������
	
	if( Abs(ek) < eth)
	{
		Out = 0;
	}
	else 
	{
		mn = a0*ek + a1*ek1 + a2*ek2;
		ek2 = ek1;
		ek1 = ek;
		Mn = mn;
		if((mn>0) && ((mn-Mn)>=0.5))
		{
			Mn = Mn + 1;
		}
		if((mn<0) && (Mn-mn>=0.5))
		{
			Mn = Mn - 1;
		}
		Out = Mn;
	}
	return Out;	
}

void init(){
  init_12864();
  init_i2c();
  init_PWM();
  init_PIT();
  init_AD();
  clr_GDR_12864(0);     //12864��ȫ��
  write_str_12864("set:--  ",0x80,8);
  write_str_12864(" RT:00s",0x84,7);
  write_str_12864("now:--.-",0x90,8);
  write_str_12864(" AT:00s",0x94,7);
}
void main(void) {
  /* put your own code here */
  
  
  int i,j,k;
 
  init();
  
  for(i=0;i<2;i++){                    //��ȡ�ϴ������¶�
    st[i] = read_byte_24c02(i);
  }
  
  set_T = (st[0]-'0')*10  + (st[1]-'0') ;       //��ȡ�ϴ������¶���ֵ
                     
  write_str_12864(st,0x80+2,2);
  
  l_sec = read_byte_24c02(7);     //��ȡ�洢����Ӧʱ�� 
  write_num_12864(l_sec,0x96);   //��ʾ�ϴ���Ӧ������
  
  EnableInterrupts;


  for(;;) {
    get_T();
    adjust_T();
    judge_key(scan_key()); 
    
  }/* loop forever */
  /* please make sure that you never leave main */
}
