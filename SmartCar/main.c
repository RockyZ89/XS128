#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Modules.h"         //XS128内部模块
#include "Control.h"         //运动控制
#include "Peripherals.h"     //板载外设、图像传输



uchar i,j,k;   //循环计数变量

void main(void) {
  /* put your own code here */
  
    
  
    init_PLL(); 
    Buzzer_ms(500);
    DisableInterrupts;
    init_PWM();
   // init_PIT();
    LED_Init();
    init_SCI();
    init_TIM();
    

    DDRA = 0x00;
    
    
    
    EnableInterrupts;
    
  //  LED_Fill(0xff);
    for(i=0;i<62;i++){
        Image_Data[i][81] = 1;  
    }
    

//    

   //  LED_P6x8Str(0,1,(uchar *)"abc");
  for(;;) {    
    if(VSYN_flg) 
    {
                   
    //  SCI1_Transmit();  
      
    /*  for (i=0; i<ROW; i++)
        for(j=0; j<COLUMN; j++)
          Image_Data[i][j] = (Image_Data[i][j] > 240) ?LED_IMAGE_WHITE :LED_IMAGE_BLACK;*/
      for(i=0;i<62;i++){
        Image_Data[i][0] = 1;  
      }    
      LED_PrintImage(Image_Data,ROW,COLUMN);
      

      VSYN_flg=0;
      EnableInterrupts;
    }
 
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内               行中断
interrupt 8 void irq_TIMER0(void)        //输入捕捉timer0
{
  TFLG1_C0F = 1;

  HREF_C++;
  if (HREF_C<4 || HREF_C>240 )       
  {
    return;//判断是否从新的一场开始
  } 

  if(HREF_C % INTERVAL == 0)
  {
    for(i=0;i<80;i++){
     
       Image_Data[Line_C][i] = (PORTA < THRESHOLD) ?LED_IMAGE_BLACK :LED_IMAGE_WHITE;
       RowData[Line_C] = RowData[Line_C] + Image_Data[Line_C][i];
       ColData[i] = ColData[i] + Image_Data[Line_C][i];
    }
    _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
    _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);

    Line_C++;
    /*
    Image_Data[Line_C][0] = PORTA;Threshold_sum = Threshold_sum + PORTA;
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
    Image_Data[Line_C][80] = PORTA;Threshold_sum = Threshold_sum + PORTA;
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
    Image_Data[Line_C][119] = PORTA;Threshold_sum = Threshold_sum + PORTA;  */
 
    
  }
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED   //中断函数置于非分页区内               场中断
interrupt 9 void irq_TIMER1(void)        //输入捕捉timer1
{
  TFLG1_C1F = 1; //清场中断
  TFLG1_C0F = 1; //清行中断

  HREF_C = 0;  //行计数清零
  Line_C = 0; //有效行计数清零

  VSYN_flg = 1; //场完成标志
   
}

#pragma CODE_SEG DEFAULT

