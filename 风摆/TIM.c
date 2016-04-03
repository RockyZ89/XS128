void init_TIM_IC(){
  TSCR2 = 0X06;  //关闭溢出中断且64分频
  TIOS_IOS0 = 0;  //定通道0为输入捕捉，1为输出比较
  TCTL4 = 0X01;   //上升沿捕捉，0x02为下降沿捕捉
  TIE_C0I = 1;   //通道0中断使能
  TSCR1 = 0X80;   //开启总定时器中断
  TFLG1_C0F = 1;   //标志位清零
}
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt 8 TIM(void){
  TFLG1_C0F = 1;//标志位清0
  TSCR1 = 0X80; //关总中断
}
#pragma CODE_SEG DEFAULT