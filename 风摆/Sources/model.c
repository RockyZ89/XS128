#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "model.h"


uint T=0;
uchar flag_fan =0;

/*********************������**********************/
void buzzer_s(){                //����
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay_ms(1000);
  PORTK_PK7 = 1;
}
void buzzer_l(){                //����
  DDRK_DDRK7 = 1;
  PORTK_PK7 = 0;
  delay_ms(3000);
  PORTK_PK7 = 1;
}

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



/******************************************************
*                          PIT                        *
******************************************************/
void init_PIT(){
  PITCFLMT = 0x00;//��ֹPITģ��
  PITCE_PCE0 = 1; //ʹ�ܶ�ʱ��ͨ��0
  PITMUX = 0x00;  //��ʱ��ͨ��0ʹ�� ΢��ʱ��׼0
  PITMTLD0 = 0xff;//����8λ��������ֵ
  PITLD0 = 0x0270;//����16λ��������ֵ  ��ʱ���� 1ms  
  PITINTE = 0x01; //ʹ��PIT��ʱ��ͨ��0�ж�
  PITCFLMT = 0x80;//ʹ��PITģ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void PIT_ISR(void){
  PITTF_PTF0 = 1;//����жϱ�־
  
}
#pragma CODE_SEG DEFAULT



/******************************************************
*                          PWM                        *
******************************************************/
void init_PWM(){
	PWMPRCLK = 0x44;                           //����ABʱ�ӷ�Ƶ����
	PWMSCLA = (500000 / (100000*2));                            //����SA��Ƶ����
	PWMSCLB = (500000 / (100000*2));
//	PWMSCLB = CH_SB_DIV;
	
//	PWMCTL_CON23 = 1;                                //01 23������16λPWM���
//	PWMCTL_CON01 = 1;
	
	PWMCLK_PCLK0 = 1;                               //��SA,SB
	PWMCLK_PCLK1 = 1;                               //��SA
	PWMCLK_PCLK4 = 1;                               //��SA
	PWMCLK_PCLK5 = 1;                               //��SA
	
	PWMCLK_PCLK2 = 1;                               //��SA,SB
	PWMCLK_PCLK3 = 1;                               //��SA
	PWMCLK_PCLK6 = 1;                               //��SA
	PWMCLK_PCLK7 = 1; 
//	PWMCLK_PCLK3 = 1;
	
	PWMPER0 = (100000 / 1000);             //����Ϊ��50000/1000=50
	PWMPER1 = (100000 / 1000);             //����Ϊ��5000000/1000=5000
	PWMPER4 = (100000 / 1000);             //����Ϊ��5000000/1000=5000
	PWMPER5 = (100000 / 1000);             //����Ϊ��5000000/1000=5000
	
	PWMPER2 = (100000 / 1000);             //����Ϊ��50000/1000=50
	PWMPER3 = (100000 / 1000);             //����Ϊ��5000000/1000=5000
	PWMPER6 = (100000 / 1000);             //����Ϊ��5000000/1000=5000
	PWMPER7 = (100000 / 1000);
	
	PWMPOL_PPOL0 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL1 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL4 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL5 = 1;                               //����Ϊ�����ȸߺ�͡�
	
	PWMPOL_PPOL2 = 0;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL3 = 0;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL6 = 0;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL7 = 0;                               //����Ϊ�����ȸߺ�͡�
	
	PWMCAE =0x00;                                   //��ȱ��ռ�ձȺ�PWME���ã�������ռ�ձȣ���ʹ��
	
}


void motor0_open(unsigned int duty)                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���
{
	PWME_PWME0 = 1;                                //ʹ��IN1
	PWME_PWME2 = 1;
//	PWME_PWME5 = 0;                                //��ֹIN2
//	DDRP_DDRP5 = 1;                                //PP5����͵�ƽ
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

void motor1_open(unsigned int duty)                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���
{
	PWME_PWME1 = 1;                                //ʹ��IN1
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

void motor4_open(unsigned int duty)                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���
{
	PWME_PWME4 = 1;                                //ʹ��IN1
	PWME_PWME6 = 1;
	PWMDTY4 = duty;	                                //ʹ��IN1
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

void motor5_open(unsigned int duty)                      //ֱ�������ת����ת�Ļ�����Ҫ��һ·PWM��PP5���п���
{
	PWME_PWME5 = 1;                                //ʹ��IN1
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
  Error = pp->SetPoint - NextPoint; // ƫ�� 
  pp->SumError += Error; // ���� 
  dError = pp->LastError - pp->PrevError; // ��ǰ΢�� 
  pp->PrevError = pp->LastError; 
  pp->LastError = Error; 
  return (pp->Proportion * Error//����
  + pp->Integral * pp->SumError  //������
  + pp->Derivative * dError); //   ΢���� 
} 
*/
