#include "Modules.h"


/*·············································内部宏·············································*/
#define BUS_CLOCK 80000000                     //系统时钟80MHz    
#define BAUD_RATE 115200                       //波特率
#define PWM_PRCLK 0x22                        //AB时钟都是4分频
#define CH_A_FREQ  20000000                   //由PWM_PRCLK值算出：80M/4=20M
#define CH_B_FREQ  20000000
#define CH_SA_FREQ 1000000                      //设置的SA频率1M
#define CH_SB_FREQ 1000000                      //SB频率 1M
#define CH_SA_DIV  (CH_A_FREQ / (CH_SA_FREQ*2))   //要写入SA分频寄存器的数据,计算公式就是这样
#define CH_SB_DIV  (CH_B_FREQ / (CH_SB_FREQ*2))   
#define SERVO_FREQ  50                    	  //设置舵机的频率
#define MOTOR_FREQ  5000                       //1000000/5000 = 200,即周期寄存器为200,未级联，最多只能255




/*---------------------------------------------80M延时函数------------------------------------------*/
void delay_ms_80M(uint xms){     //赋值100ms   实测99ms
	uint i,j,k;
	for(i=0; i< xms; i++){
        for(j=0; j<100; j++)
		  for(k=0; k<56; k++) 
		    ;
	}
}

void delay_us_80M(uint xus){      //赋值100us  实测115us
	uint i,j;
	for(i=0; i<xus*2; i++) 
    for(j=0; j<1; j++)
      ; 
}



/*---------------------------------------------PIT定时------------------------------------------*/
uint_32 usflg=0;

void init_PIT()
{
	PITCFLMT = 0X00;               //禁止PIT模块
	
	PITCE_PCE0 = 1;                //使能定时器通道0
	
	PITMUX = 0x00;                 //定时器通道0使用微定时基准0
	PITMTLD0 = 0x01;               //微定时器装载199，定时器装载399.
	PITLD0 = 0x27;               //定时周期 = （1+1）*（39+1)/80M = 1μs

//	PITINTE = 0x00;
	PITINTE = 0x01;                //通道1中断使能
	PITCFLMT = 0x80;               //PIT模块使能
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void pit_itr() 
{
	PITTF_PTF0 = 1;
	usflg++;
}
#pragma CODE_SEG DEFAULT  




/*---------------------------------------------PWM控制电机、舵机--------------------------------------*/
void init_PWM()
{
	        /*****************PWM时钟设置***************/
	PWMPRCLK = PWM_PRCLK;                           //设置AB时钟分频比例
	PWMSCLA = CH_SA_DIV;                            //设置SA分频比例
	PWMSCLB = CH_SB_DIV;                            //设置SB分频比例
	PWMCAE =0x00;                                   //占空比对其设置，左对齐
		
                       /*初始化电机*/
	PWMCLK_PCLK0 = 1;                               //用SA
	PWMCLK_PCLK1 = 1;                               //用SA
	PWMCLK_PCLK6 = 1;                               //用SB
	PWMCLK_PCLK7 = 1;                               //用SB
	PWMCTL_CON01 = 0;                               //01不级联
	PWMCTL_CON67 = 0;                               //67不级联
	PWMPER0 = (CH_SA_FREQ / MOTOR_FREQ);             //周期为：200
	PWMPER1 = (CH_SA_FREQ / MOTOR_FREQ);             //周期为：200
	PWMPER6 = (CH_SB_FREQ / MOTOR_FREQ);             //周期为：200
	PWMPER7 = (CH_SB_FREQ / MOTOR_FREQ);             //周期为：200
	PWMPOL_PPOL0 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL1 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL6 = 1;                               //极性为正，先高后低。
	PWMPOL_PPOL7 = 1;                               //极性为正，先高后低。

	DDRK_DDRK4=1;                                   //电机使能，先使能，需要上位机控制时再说。但DDR永远不用动了
	DDRK_DDRK5=1;
	PORTK_PK4=1;
	PORTK_PK5=1; 
                      
					   /*初始化舵机*/
	PWMCLK_PCLK3 = 1;                               //用SB
	PWMCTL_CON23 = 1;                               //23级联成16位PWM输出控制舵机
	PWMPOL_PPOL3 = 1;                               //极性为正，先高后低。
	PWMPER23 = (CH_SB_FREQ / SERVO_FREQ);           //周期寄存器为 20 000
	
	                  
					   /*初始化结束后使能PWM*/       
	PWMDTY23= 1430;                                  //舵机初始占空比，要摆正，不一定对。
	PWMDTY0 = 0;                                    //电机占空比刚开始设置为0，然后再通过主程序控制。
	PWMDTY1 = 0;
	PWMDTY6 = 0;
	PWMDTY7 = 0;
	PWME = 0XFF;                                    //PWM舵机使能
		
}


/*--------------------------------------TIM输入捕获-------------------------------------------*/
void init_TIM(void)    
{
	TIOS =0x00;        //定时器通道0，1 为输入捕捉
	TSCR1=0x80;        //定时器使能
	TCTL4=0x09;        //通道0 捕捉上升沿（HREF）通道1 捕捉下降沿（VSYNC）
	TIE=0x03;          //通道0，1 中断使能
	TFLG1=0xFF;        //清中断标志位
}


/*-------------------------------------SCI初始化---------------------------------------------*/
void init_SCI()
{
    SCI1BDL = (byte)((BUS_CLOCK /* CLK freq*/) / BAUD_RATE /* baud rate */ / 16 /*factor*/);
    SCI1CR1 = 0X00;                                      /*normal,no parity*/
    SCI1CR2 = 0X0C;                                      /*RIE=1,TE=1,RE=1, */
}




/*--------------------------------------------PLL锁相环--------------------------------------------*/
void init_PLL(void)  
{     
    CLKSEL=0X00;    // disengage PLL to system  
    PLLCTL_PLLON=1; // turn on PLL  
    
    #if(BUS_CLOCK == 16000000)      // BUS CLOCK=16M  
      SYNR=0x00 | 0x01;
      
  //  #elif(BUS_CLOCK == 24000000)      // BUS CLOCK=24M  
  //    SYNR=0x00 | 0x02;
      
    #elif(BUS_CLOCK == 32000000)      // BUS CLOCK=32M  
      SYNR=0x40 | 0x03;
      
    #elif(BUS_CLOCK == 40000000)      // BUS CLOCK=40M  
      SYNR=0xc0 | 0x04;
      
    #elif(BUS_CLOCK == 48000000)      // BUS CLOCK=48M  
      SYNR=0xc0 | 0x05;
      
    #elif(BUS_CLOCK == 56000000)      // BUS CLOCK=56M  
      SYNR=0xc0 | 0x06;
      
    #elif(BUS_CLOCK == 64000000)      // BUS CLOCK=64M  
      SYNR=0xc0 | 0x07;
      
    #elif(BUS_CLOCK == 72000000)      // BUS CLOCK=72M  
      SYNR=0xc0 | 0x08;
      
    #elif(BUS_CLOCK == 80000000)      // BUS CLOCK=80M  
      SYNR=0xc0 | 0x09;
      
    #elif(BUS_CLOCK == 88000000)      // BUS CLOCK=88M  
      SYNR=0xc0 | 0x0a;
      
    #elif(BUS_CLOCK == 96000000)      // BUS CLOCK=96M  
      SYNR=0xc0 | 0x0b;
      
    #elif(BUS_CLOCK == 104000000)      // BUS CLOCK=104M  
      SYNR=0xc0 | 0x0c;
      
    #elif(BUS_CLOCK == 112000000)      // BUS CLOCK=112M  
      SYNR=0xc0 | 0x0d;
      
    #endif
   
        
    REFDV=0x80 | 0x01;  
    POSTDIV=0x00;       
    _asm(nop);          
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  

/*
    
void SetBusCLK_32M(void)  
{     
    CLKSEL=0X00;                // disengage PLL to system  
    PLLCTL_PLLON=1;         // turn on PLL  
    SYNR =0x40 | 0x03;  // pllclock=2*osc*(1+SYNR)/(1+REFDV)=64MHz;                        
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;    
    _asm(nop);          // BUS CLOCK=32M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_40M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x04;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=80MHz;  
    _asm(nop);          //BUS CLOCK=40M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_48M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x05;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=96MHz;  
    _asm(nop);          //BUS CLOCK=48M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_64M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x07;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=128MHz;  
    _asm(nop);          //BUS CLOCK=64M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_80M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x09;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=160MHz;  
    _asm(nop);          //BUS CLOCK=80M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_88M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x0a;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=176MHz;  
    _asm(nop);          //BUS CLOCK=88M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_96M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x0b;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=192MHz;  
    _asm(nop);          //BUS CLOCK=96M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_104M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x0c;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=208MHz;  
    _asm(nop);          //BUS CLOCK=104M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  
    
void SetBusCLK_120M(void)  
{     
    CLKSEL=0X00;                //disengage PLL to system  
    PLLCTL_PLLON=1;         //turn on PLL  
    SYNR =0xc0 | 0x0d;                          
    REFDV=0x80 | 0x01;   
    POSTDIV=0x00;       //pllclock=2*osc*(1+SYNR)/(1+REFDV)=240MHz;  
    _asm(nop);          //BUS CLOCK=120M  
    _asm(nop);  
    while(!(CRGFLG_LOCK==1));     //when pll is steady ,then use it;  
    CLKSEL_PLLSEL =1;               //engage PLL to system;   
}  */









