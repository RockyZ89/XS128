#include "Modules.h"


/*�������������������������������������������������������������������������������������������ڲ��ꡤ����������������������������������������������������������������������������������������*/
#define BUS_CLOCK 80000000                     //ϵͳʱ��80MHz    
#define BAUD_RATE 115200                       //������
#define PWM_PRCLK 0x22                        //ABʱ�Ӷ���4��Ƶ
#define CH_A_FREQ  20000000                   //��PWM_PRCLKֵ�����80M/4=20M
#define CH_B_FREQ  20000000
#define CH_SA_FREQ 1000000                      //���õ�SAƵ��1M
#define CH_SB_FREQ 1000000                      //SBƵ�� 1M
#define CH_SA_DIV  (CH_A_FREQ / (CH_SA_FREQ*2))   //Ҫд��SA��Ƶ�Ĵ���������,���㹫ʽ��������
#define CH_SB_DIV  (CH_B_FREQ / (CH_SB_FREQ*2))   
#define SERVO_FREQ  50                    	  //���ö����Ƶ��
#define MOTOR_FREQ  5000                       //1000000/5000 = 200,�����ڼĴ���Ϊ200,δ���������ֻ��255




/*---------------------------------------------80M��ʱ����------------------------------------------*/
void delay_ms_80M(uint xms){     //��ֵ100ms   ʵ��99ms
	uint i,j,k;
	for(i=0; i< xms; i++){
        for(j=0; j<100; j++)
		  for(k=0; k<56; k++) 
		    ;
	}
}

void delay_us_80M(uint xus){      //��ֵ100us  ʵ��115us
	uint i,j;
	for(i=0; i<xus*2; i++) 
    for(j=0; j<1; j++)
      ; 
}



/*---------------------------------------------PIT��ʱ------------------------------------------*/
uint_32 usflg=0;

void init_PIT()
{
	PITCFLMT = 0X00;               //��ֹPITģ��
	
	PITCE_PCE0 = 1;                //ʹ�ܶ�ʱ��ͨ��0
	
	PITMUX = 0x00;                 //��ʱ��ͨ��0ʹ��΢��ʱ��׼0
	PITMTLD0 = 0x01;               //΢��ʱ��װ��199����ʱ��װ��399.
	PITLD0 = 0x27;               //��ʱ���� = ��1+1��*��39+1)/80M = 1��s

//	PITINTE = 0x00;
	PITINTE = 0x01;                //ͨ��1�ж�ʹ��
	PITCFLMT = 0x80;               //PITģ��ʹ��
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vpit0 void pit_itr() 
{
	PITTF_PTF0 = 1;
	usflg++;
}
#pragma CODE_SEG DEFAULT  




/*---------------------------------------------PWM���Ƶ�������--------------------------------------*/
void init_PWM()
{
	        /*****************PWMʱ������***************/
	PWMPRCLK = PWM_PRCLK;                           //����ABʱ�ӷ�Ƶ����
	PWMSCLA = CH_SA_DIV;                            //����SA��Ƶ����
	PWMSCLB = CH_SB_DIV;                            //����SB��Ƶ����
	PWMCAE =0x00;                                   //ռ�ձȶ������ã������
		
                       /*��ʼ�����*/
	PWMCLK_PCLK0 = 1;                               //��SA
	PWMCLK_PCLK1 = 1;                               //��SA
	PWMCLK_PCLK6 = 1;                               //��SB
	PWMCLK_PCLK7 = 1;                               //��SB
	PWMCTL_CON01 = 0;                               //01������
	PWMCTL_CON67 = 0;                               //67������
	PWMPER0 = (CH_SA_FREQ / MOTOR_FREQ);             //����Ϊ��200
	PWMPER1 = (CH_SA_FREQ / MOTOR_FREQ);             //����Ϊ��200
	PWMPER6 = (CH_SB_FREQ / MOTOR_FREQ);             //����Ϊ��200
	PWMPER7 = (CH_SB_FREQ / MOTOR_FREQ);             //����Ϊ��200
	PWMPOL_PPOL0 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL1 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL6 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPOL_PPOL7 = 1;                               //����Ϊ�����ȸߺ�͡�

	DDRK_DDRK4=1;                                   //���ʹ�ܣ���ʹ�ܣ���Ҫ��λ������ʱ��˵����DDR��Զ���ö���
	DDRK_DDRK5=1;
	PORTK_PK4=1;
	PORTK_PK5=1; 
                      
					   /*��ʼ�����*/
	PWMCLK_PCLK3 = 1;                               //��SB
	PWMCTL_CON23 = 1;                               //23������16λPWM������ƶ��
	PWMPOL_PPOL3 = 1;                               //����Ϊ�����ȸߺ�͡�
	PWMPER23 = (CH_SB_FREQ / SERVO_FREQ);           //���ڼĴ���Ϊ 20 000
	
	                  
					   /*��ʼ��������ʹ��PWM*/       
	PWMDTY23= 1430;                                  //�����ʼռ�ձȣ�Ҫ��������һ���ԡ�
	PWMDTY0 = 0;                                    //���ռ�ձȸտ�ʼ����Ϊ0��Ȼ����ͨ����������ơ�
	PWMDTY1 = 0;
	PWMDTY6 = 0;
	PWMDTY7 = 0;
	PWME = 0XFF;                                    //PWM���ʹ��
		
}


/*--------------------------------------TIM���벶��-------------------------------------------*/
void init_TIM(void)    
{
	TIOS =0x00;        //��ʱ��ͨ��0��1 Ϊ���벶׽
	TSCR1=0x80;        //��ʱ��ʹ��
	TCTL4=0x09;        //ͨ��0 ��׽�����أ�HREF��ͨ��1 ��׽�½��أ�VSYNC��
	TIE=0x03;          //ͨ��0��1 �ж�ʹ��
	TFLG1=0xFF;        //���жϱ�־λ
}


/*-------------------------------------SCI��ʼ��---------------------------------------------*/
void init_SCI()
{
    SCI1BDL = (byte)((BUS_CLOCK /* CLK freq*/) / BAUD_RATE /* baud rate */ / 16 /*factor*/);
    SCI1CR1 = 0X00;                                      /*normal,no parity*/
    SCI1CR2 = 0X0C;                                      /*RIE=1,TE=1,RE=1, */
}




/*--------------------------------------------PLL���໷--------------------------------------------*/
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









