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
