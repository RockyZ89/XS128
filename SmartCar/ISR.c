/***********************************************
*   
*                中断服务子程序
*
*
************************************************/

#include "Modules.h"
#include "Peripherals.h"




#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 66 irq_PIT0(void)     //PIT0中断函数
{ 
  if(PITTF_PTF0 != 0) 
  {
      PITTF_PTF0 = 1;
      /******代码添加区******/
      PORTB_PB0 = ~PORTB_PB0;
	}   
}
#pragma CODE_SEG DEFAULT





#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 67 irq_PIT1(void)     //PIT1中断函数
{ 
  if(PITTF_PTF1 != 0) 
  {
      PITTF_PTF1 = 1;
      /******代码添加区******/
      PORTB_PB1 = ~PORTB_PB1;
	}   
}
#pragma CODE_SEG DEFAULT








#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 68 irq_PIT2(void)     //PIT2中断函数
{ 
  if(PITTF_PTF2 != 0) 
  {
      PITTF_PTF2 = 1;
      /******代码添加区******/
      PORTB_PB2 = ~PORTB_PB2;
	}   
}
#pragma CODE_SEG DEFAULT







#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 69 irq_PIT3(void)     //PIT3中断函数
{ 
  if(PITTF_PTF3 != 0) 
  {
      PITTF_PTF3 = 1;
      /******代码添加区******/
      PORTB_PB3 = ~PORTB_PB3;
	}   
}
#pragma CODE_SEG DEFAULT







#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 25 irq_PORTH(void)     //PORTH端口外部中断中断函数
{ 
  if(PIFH_PIFH0 != 0)                 //PORTH_PTH0触发中断
  {
      PIFH_PIFH0 = 1;
      /******代码添加区******/
      PORTB_PB3 = ~PORTB_PB3;
	}   
}
#pragma CODE_SEG DEFAULT




#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 24 irq_PORTJ(void)     //PORTJ端口外部中断中断函数
{ 
  if(PIFJ_PIFJ6 != 0)                 //PORTJ_PTJ6触发中断
  {
      PIFJ_PIFJ6 = 1;
      /******代码添加区******/
      PORTB_PB3 = ~PORTB_PB3;
	}   
}
#pragma CODE_SEG DEFAULT



#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内
interrupt 20 void irq_SCI0(void)         //SCI0接收中断
{
    unsigned char data_receive;
    data_receive = SCI_Receive(UART0);   //将SCI接收的数据传送给data_receive变量
    SCI_Send_Char(UART0,data_receive);
    
}



#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内
interrupt 21 void irq_SCI1(void)         //SCI1接收中断
{
    unsigned char data_receive;
    data_receive = SCI_Receive(UART1);   //将SCI接收的数据传送给data_receive变量
    SCI_Send_Char(UART1,data_receive);
}




#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内               行中断
interrupt 8 void irq_TIMER0(void)        //输入捕捉timer0
{
   TFLG1_C0F = 1;
  m++;
  if ( m<4 || m>240 )       
  {
    return;//判断是否从新的一场开始
  } 

  
  //Interval=6;
  if(m % Interval == 0)
  {
    for()
    /*Image_Data[Line_C][0] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][1] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][2] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][3] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][4] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][5] = PORTA;Threshold_sum = Threshold_sum + PORTA;                                                 
    Image_Data[Line_C][6] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][7] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][8] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][9] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][10] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][11] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][12] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][13] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][14] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][15] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][16] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][17] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][18] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][19] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][20] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][21] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][22] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][23] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][24] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][25] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][26] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][27] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][28] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][29] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][30] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][31] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][32] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][33] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][34] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][35] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][36] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][37] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][38] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][39] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][40] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][41] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][42] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][43] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][44] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][45] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][46] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][47] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][48] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][49] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][50] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][51] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][52] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][53] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][54] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][55] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][56] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][57] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][58] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][59] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][60] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][61] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][62] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][63] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][64] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][65] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][66] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][67] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][68] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][69] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][70] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][71] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][72] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][73] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][74] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][75] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][76] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][77] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][78] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][79] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    
 /*   Image_Data[Line_C][80] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][81] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][82] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][83] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][84] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][85] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][86] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][87] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][88] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][89] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][90] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][91] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][92] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][93] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][94] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][95] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][96] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][97] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][98] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][99] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][100] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][101] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][102] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][103] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][104] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][105] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][106] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][107] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][108] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][109] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][110] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][111] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][112] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][113] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][114] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][115] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][116] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][117] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][118] = PORTA;Threshold_sum = Threshold_sum + PORTA;
    Image_Data[Line_C][119] = PORTA;Threshold_sum = Threshold_sum + PORTA;  
 */
    Threshold_sum = Threshold_sum / COLUMN;
    Threshold[Line_C] = Threshold_sum;
    Line_C++;
  }
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内               场中断
interrupt 9 void irq_TIMER1(void)        //输入捕捉timer1
{
  TFLG1_C1F = 1; //清场中断
  TFLG1_C0F = 1; //清行中断

  Line_C = 0; //行计数器
  VSYN_C = ++VSYN_C;
   
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内
interrupt 10 void irq_TIMER2(void)        //输入捕捉timer2
{
   TFLG1_C2F = 1;//清中断标志位 

}
