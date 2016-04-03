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
*                      变量定义                       *
******************************************************/
double now_T=0;      //温度
double set_T=0;
uchar nt[5]={'0','0','.','0',' '};
uchar st[2]={'0','0'};

double ek=0;         //PID
double ep=0;          
uint pid_dtyh=100,pid_dtym=180;         
int  pid_out=0;
             
uint sec=0;                         //程序运行时间RT
uint l_sec=0;                       //响应时间AT

uint val[40]={0};                   //滤波数组

uint s=0,c=0;                     //模式标志位；设置温度数组下标

uint flag_T=1;                  //AT仅设置一次标志位




/******************************************************
*                          AD                         *
******************************************************/
void init_AD(){
  ATD0DIEN = 0x00;//禁止数字输入功能
  ATD0CTL1 = 0x20;//AD分辨率选择10位，且采样前不放电
  ATD0CTL2 = 0x40;//AD模块快速清零，禁止外部触发，禁止中断
  ATD0CTL3 = 0x88;//AD转换结果右对齐，每个序列1个通道，非FIFO模式
  ATD0CTL4 = 0x67;//采样时间4个周期，PRS=3
  ATD0CTL5 = 0x20;//对通道0单通道连续采样，同时启动AD转换序列 
}

void get_T(){ 
  uint i,result;
  uint temp=0; 
                                           
  for(i=0;i<40;i++){
    while(!ATD0STAT2L_CCF0) ;
    val[i] = ATD0DR0;
  }                                     //AD平均滤波
  for(i=1;i<39;i++)
    temp =  temp + val[i];
  result = temp / 38.0; 
                        
  now_T = result * 500.0 / 1024.0 + 0.05;           //实时温度数值

  temp = (int)(now_T * 10);                        //实时温度数组
  nt[0] = temp / 100 + 0x30;                                      
  nt[1] = temp / 10 % 10 + 0x30;
  nt[3] = temp % 10 +0x30; 
  
}



/******************************************************
*                          PIT                        *
******************************************************/
void init_PIT(){
  PITCFLMT = 0x00;//禁止PIT模块
  PITCE_PCE0 = 1; //使能定时器通道0
  PITMUX = 0x00;  //定时器通道0使用 微定时基准0
  PITMTLD0 = 0xff;//设置8位计数器初值
  PITLD0 = 0x7A11;//设置16位计数器初值  定时周期 1s
  PITINTE = 0x01; //使能PIT定时器通道0中断
  PITCFLMT = 0x80;//使能PIT模块
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//清除中断标志
  sec++;
  if(sec>=100) sec=0; 
  draw_curve_12864(now_T);     //画实时温度曲线图
  write_num_12864(sec,0x86);   //12864显示运行时间RT
  write_str_12864(nt,0x92,5);  //12864显示实时温度
}
#pragma CODE_SEG DEFAULT



/******************************************************
*                          PWM                        *
******************************************************/
void init_PWM(){
  PWME = 0x00;          //禁止PWM模块
  
  PWMPRCLK = 0x01;      //CLOCKA 2分频
  PWMSCLA = 2;          //SA为A的4分频
   
  PWMCLK_PCLK0 = 1;    //选择SA作为PWM时钟
  PWMCLK_PCLK1 = 1;  
  
  
  PWMPER0 = 200;       //通道0周期寄存器设置为200
  PWMPER1 = 200;       //通道1周期寄存器设置为200
  
  PWMDTY0 = 100;         //通道0占空比寄存器设置
  PWMDTY1 = 100;         //通道1占空比寄存器设置
  
  PWMPOL_PPOL0 = 1;     //PWM极性选择先高后低
  PWMPOL_PPOL1 = 1; 
  
  PWMCAE = 0x00;        //左对齐输出
  PWMCTL = 0x00;        //不级联
  
  PWME_PWME0 = 1;
  PWME_PWME1 = 1; 
}

void heat_PWM(uint temp){    //加热模块输出 temp 占空比值    0-200可调
    PWMDTY0 = temp;
  //  PWME_PWME0 = 1;            
}

void fan_PWM(uint temp){    //风扇模块输出 temp 占空比值    0-200可调
    PWMDTY1 = temp;
 //   PWME_PWME1 = 1;            
}  

/*****************************************************
*                       控温                         *
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
    write_num_12864(l_sec,0x96);   //显示上次响应的秒数
    flag_T = 0;   //只响应一次标志位   
  }
}

/*******************PID**************/
int PID(double ek)
{
	float Kp, Ti, T, Td, a0, a1, a2, mn;
	static float ek1 = 0, ek2 = 0, eth;
	int Mn, Out;
	
	Kp = 10;                           //比例系数
	Ti = 0;                           //积分系数
	Td = 5;                           //微分系数
	T  = 10;                         //采样周期（控制周期）10ms
	eth = 0.5;                         //误差阀值
	a0 =  Kp * (1 + T/Ti + Td/T);    //中间系数
	a1 = -Kp * (1 + 2*Td/T);
	a2 =  Kp * Td/T;
	mn  = 0;                         //计算结果
	Mn  = 0;
	Out = 0;                         //输出结果
	
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
  clr_GDR_12864(0);     //12864清全屏
  write_str_12864("set:--  ",0x80,8);
  write_str_12864(" RT:00s",0x84,7);
  write_str_12864("now:--.-",0x90,8);
  write_str_12864(" AT:00s",0x94,7);
}
void main(void) {
  /* put your own code here */
  
  
  int i,j,k;
 
  init();
  
  for(i=0;i<2;i++){                    //读取上次设置温度
    st[i] = read_byte_24c02(i);
  }
  
  set_T = (st[0]-'0')*10  + (st[1]-'0') ;       //读取上次设置温度数值
                     
  write_str_12864(st,0x80+2,2);
  
  l_sec = read_byte_24c02(7);     //读取存储的响应时间 
  write_num_12864(l_sec,0x96);   //显示上次响应的秒数
  
  EnableInterrupts;


  for(;;) {
    get_T();
    adjust_T();
    judge_key(scan_key()); 
    
  }/* loop forever */
  /* please make sure that you never leave main */
}
