#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "model.h"


uint T=0;
uchar flag_fan =0;

/*********************蜂鸣器**********************/
void buzzer_s(){                //短鸣
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay_ms(1000);
  PORTK_PK7 = 1;
}
void buzzer_l(){                //长鸣
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay_ms(3000);
  PORTK_PK7 = 1;
}

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



/******************************************************
*                          PIT                        *
******************************************************/
void init_PIT(){
  PITCFLMT = 0x00;//禁止PIT模块
  PITCE_PCE0 = 1; //使能定时器通道0
  PITMUX = 0x00;  //定时器通道0使用 微定时基准0
  PITMTLD0 = 0xff;//设置8位计数器初值
  PITLD0 = 0x0270;//设置16位计数器初值  定时周期 1ms  
  PITINTE = 0x01; //使能PIT定时器通道0中断
  PITCFLMT = 0x80;//使能PIT模块
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//清除中断标志
  
}
#pragma CODE_SEG DEFAULT



/******************************************************
*                          PWM                        *
******************************************************/
void init_PWM(){
	PWMPRCLK = 0x44;                           //设置AB时钟分频比例
	PWMSCLA = (500000 / (100000*2));                            //设置SA分频比例
	PWMSCLB = (500000 / (100000*2));
//	PWMSCLB = CH_SB_DIV;
	
//	PWMCTL_CON23 = 1;                                //01 23级联成16位PWM输出
//	PWMCTL_CON01 = 1;
	
	PWMCLK_PCLK0 = 1;                               //用SA,SB
	PWMCLK_PCLK1 = 1;                               //用SA
	PWMCLK_PCLK4 = 1;                               //用SA
	PWMCLK_PCLK5 = 1;                               //用SA
	
	PWMCLK_PCLK2 = 1;                               //用SA,SB
	PWMCLK_PCLK3 = 1;                               //用SA
	PWMCLK_PCLK6 = 1;                               //用SA
	PWMCLK_PCLK7 = 1; 
//	PWMCLK_PCLK3 = 1;
	
	PWMPER0 = (100000 / 1000);             //周期为：50000/1000=50
	PWMPER1 = (100000 / 1000);             //周期为：5000000/1000=5000
	PWMPER4 = (100000 / 1000);             //周期为：5000000/1000=5000
	PWMPER5 = (100000 / 1000);             //周期为：5000000/1000=5000
	
	PWMPER2 = (100000 / 1000);             //周期为：50000/1000=50
	PWMPER3 = (100000 / 1000);             //周期为：5000000/1000=5000
	PWMPER6 = (100000 / 1000);             //周期为：5000000/1000=5000
	PWMPER7 = (100000 / 1000);
	
	PWMPOL_PPOL0 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL1 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL4 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL5 = 1;                               //极性为正，先高后低。
	
	PWMPOL_PPOL2 = 0;                               //极性为正，先高后低。
	PWMPOL_PPOL3 = 0;                               //极性为正，先高后低。
	PWMPOL_PPOL6 = 0;                               //极性为正，先高后低。
	PWMPOL_PPOL7 = 0;                               //极性为正，先高后低。
	
	PWMCAE =0x00;                                   //还缺少占空比和PWME设置，先设置占空比，再使能
	
}


void motor0_open(unsigned int duty)                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制
{
	PWME_PWME0 = 1;                                //使能IN1
	PWME_PWME2 = 1;
//	PWME_PWME5 = 0;                                //禁止IN2
//	DDRP_DDRP5 = 1;                                //PP5输出低电平
//	PTP_PTP5 = 0;
	PWMDTY0 = duty;
	PWMDTY2 = duty;
}

void motor0_close()
{
	PWME_PWME0 = 0;
	PWME_PWME2 = 0;
//	PWME_PWME5 = 0;
	DDRP_DDRP0 = 1;
	DDRP_DDRP2 = 1;
//	DDRP_DDRP5 = 1;
	PTP_PTP0 = 1;
	PTP_PTP2 = 1;
//	PTP_PTP5 = 1;
}

void motor1_open(unsigned int duty)                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制
{
	PWME_PWME1 = 1;                                //使能IN1
	PWME_PWME3 = 1;
	PWMDTY1 = duty;
	PWMDTY3 = duty;
}

void motor1_close()
{
	PWME_PWME1 = 0;
	DDRP_DDRP1 = 1;
	PTP_PTP1 = 1;
	PWME_PWME3 = 0;
	DDRP_DDRP3 = 1;
	PTP_PTP3 = 1;
}

void motor4_open(unsigned int duty)                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制
{
	PWME_PWME4 = 1;                                //使能IN1
	PWME_PWME6 = 1;
	PWMDTY4 = duty;	                                //使能IN1
	PWMDTY6 = duty;
}

void motor4_close()
{
	PWME_PWME4 = 0;
	DDRP_DDRP4 = 1;
	PTP_PTP4 = 1;
	PWME_PWME6 = 0;
	DDRP_DDRP6 = 1;
	PTP_PTP6 = 1;
}

void motor5_open(unsigned int duty)                      //直流电机正转，反转的话还需要打开一路PWM，PP5进行控制
{
	PWME_PWME5 = 1;                                //使能IN1
	PWME_PWME7 = 1;
	PWMDTY5 = duty;
	PWMDTY7 = duty;
}

void motor5_close()
{
	PWME_PWME5 = 0;
	DDRP_DDRP5 = 1;
	PTP_PTP5 = 1;
	PWME_PWME7 = 0;
	DDRP_DDRP7 = 1;
	PTP_PTP7 = 1;
}


void front_fan(uchar duty){
  PWMDTY0 = duty;  
}
void back_fan(uchar duty){
  PWMDTY1 = duty;
}
void left_fan(uchar duty){
  PWMDTY4 = duty;
}
void right_fan(uchar duty){
  PWMDTY5 = duty;
}


/******************************************************
*                          PID                        *
*****************************************************
unsigned int PIDCalc( struct PID *pp, unsigned int NextPoint ) 
{ 
  unsigned int dError,Error; 
  Error = pp->SetPoint - NextPoint; // 偏差 
  pp->SumError += Error; // 积分 
  dError = pp->LastError - pp->PrevError; // 当前微分 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 
  return (pp->Proportion * Error//比例
  + pp->Integral * pp->SumError  //积分项
  + pp->Derivative * dError); //   微分项 
} 
*/
