


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
