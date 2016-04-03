#include "K60S2.h"
#include "intrins.h"
#include "DS18B20.h"
#include "absacc.h"
#include "DS1302.h"

/**********************��������**********************/
ulong time_base_2ms = 0;   //ʱ����
bit Uart_send_flg = 1;
uint sec= 0;
bit prompt_flash_flg = 1; 
bit system_run_reset = 0;


/*---------------U24[]����-----------*/
uchar U24[] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0};
/*
  3		 8255��CE
  4		 LED����
  5		 UNL 2003
  6		 �����λѡ
  7		 ����ܶ�ѡ
*/
#define LED  4
#define WEI 6
#define DUAN 7

/*---------------LED����--------------*/
enum {
	L1_flash_start,
	L1_flash_stop,		
} L1_flash_flg = L1_flash_stop;
bit L1_flash_state = 0;

#define LED_ON    0
#define LED_OFF	  1



/*---------------����ܶ���-----------*/
code uchar Smg_Duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff,0xbf};
		             //  0     1     2      3     4     5     6     7     8     9  ��     -
uchar Smg_add = 0;
uchar SmgBuf[]={10,10,10,10,10,10,10,10};
uchar Smg_count =0;
			  //   ȫ��
/*--------------������--------------*/
#define UNL  5

/*---------------���̶���-----------*/
bit Key_scan_flg = 0;

/*---------------DS1302-----------*/
bit read_time_flg = 1;

/*----------------DS18B20-------------*/

bit Temp_disp_flg = 1;
bit Temp_collect_reset = 1;
uchar Temp_buf_point = 0;
uchar Temp_buf[10]={10};
enum {
	Temp_interval_1s,							 
	Temp_interval_5s,
	Temp_interval_30s,											
	Temp_interval_60s,
} Temp_interval_time;
enum {
	Temp_collect_start,
	Temp_collect_stop,
} Temp_collect_flg = Temp_collect_stop;

/*---------------����״̬-----------------*/
typedef enum{
	Win_State_1,
	Win_State_2,
	Win_State_3
} WinState_Typedef;
WinState_Typedef Win_State = Win_State_1;
/*--------------���̶���-------------------*/
typedef enum{
	Key_Free,
	Key_S4,
	Key_S5,
	Key_S6,
	Key_S7
} KeyValue_Typedef;
KeyValue_Typedef Key_Value = Key_Free;


/**********************LED��ˮ��**********************/
void L1_disp(bit state){
	P2 = 0x1f;
	P0 = 0xff;
	P2 = U24[4];
	if(state == LED_ON)
		P0 = 0xfe;
	else P0 = 0xff; 
	P2 = 0x1f;
	P0 = 0x00;
}
/**********************�����**********************/
void Smg_disp(uchar SmgBuf[]){
	P2 = U24[WEI];;
	P0 = 0x00;

	P2 = 0x1f;
	P0 = 0xff;
	P2 = U24[DUAN];
	P0 = Smg_Duan[SmgBuf[Smg_add]];
	
	P2 = 0x1f;
	P0 = 0x00;
	P2 = U24[WEI];
	P0 = 1<< Smg_add;

	P2 = 0x1f;
	P0 = 0x00;
	
	Smg_add++;
	if(Smg_add >=8) Smg_add = 0;		
}



/**********************�������********************/
void Key_Scan(){
	P44 = 0;
	if(P33 == 0){		
		Key_Value = Key_S4;
		while(P33 == 0);
	}
	else if(P32 == 0){
		Key_Value = Key_S5;
		while(P32 == 0);
	}	
	else if(P31 == 0){
		Key_Value = Key_S6;
		while(P31 == 0);
	}
	else if(P30 == 0){
		Key_Value = Key_S7;
		while(P30 == 0);
	}
}
void put_S4(){
	if(Win_State == Win_State_1){
		Key_Value = Key_Free;
		Temp_interval_time++;
		if(Temp_interval_time > Temp_interval_60s) 
			Temp_interval_time = Temp_interval_1s;
	}
}
void put_S5(){
	if(Win_State == Win_State_1){
		Key_Value = Key_Free;
		Win_State = Win_State_2;
		Temp_collect_flg = Temp_collect_start; 
		if(system_run_reset == 0)
			DS1302_Init();
	}	
}
void put_S6(){
	if(Win_State == Win_State_3){
		Key_Value = Key_Free;
		if(L1_flash_flg == L1_flash_start)
			L1_flash_flg = L1_flash_stop;
		else{
			Temp_buf_point++;
			if(Temp_buf_point >= 10)
				Temp_buf_point = 0;		
		}

	}
}
void put_S7(){
	if((Win_State == Win_State_3) && (L1_flash_flg == L1_flash_stop)){
		Key_Value = Key_Free;
		Win_State = Win_State_1;
		Temp_buf_point = 0;
		system_run_reset = 1;	
	}
}

void Key_Ack(){
	switch(Key_Value){
		case Key_S4:
			put_S4();
			break;
		case Key_S5:
			put_S5();
			break;
		case Key_S6:
			put_S6();
			break;
		case Key_S7:
			put_S7();
			break;
	}			
}

/*********************����״̬**********************/
void Windows_State_1(){
	SmgBuf[0] = 10;
	SmgBuf[1] = 10;
	SmgBuf[2] = 10;
	SmgBuf[3] = 10;
	SmgBuf[4] = 10;
	SmgBuf[5] = 11;
	switch(Temp_interval_time){
		case Temp_interval_1s:
			SmgBuf[6] = 0;
			SmgBuf[7] = 1;
			break;
		case Temp_interval_5s:
			SmgBuf[6] = 0;
			SmgBuf[7] = 5;
			break;
		case Temp_interval_30s:
			SmgBuf[6] = 3;
			SmgBuf[7] = 0;
			break;
		case Temp_interval_60s:
			SmgBuf[6] = 6;
			SmgBuf[7] = 0;
			break;
	}
}
void Windows_State_2(){
	uchar tmp;
	if(prompt_flash_flg){
		SmgBuf[5] = 11;
		SmgBuf[2] = 11;
	}
	else{
	 	SmgBuf[5] = 10;
		SmgBuf[2] = 10;
	}
	if(read_time_flg){
		read_time_flg = 0;
		tmp	= Ds1302_Single_Byte_Read(0x81);
		SmgBuf[7] = tmp % 10;
		SmgBuf[6] = tmp / 10;
		tmp = Ds1302_Single_Byte_Read(0x83);
		SmgBuf[4] = tmp % 10;
		SmgBuf[3] = tmp / 10;
		tmp = Ds1302_Single_Byte_Read(0x85);
		SmgBuf[1] = tmp % 10;
		SmgBuf[0] = tmp / 10;						
	}  
	if(Temp_collect_reset){
		Temp_collect_reset = 0;
		Temp_buf[Temp_buf_point] = Temp_read();
	//	SBUF = Temp_buf[Temp_buf_point];
	   	Temp_buf_point++;
	}
	if(Temp_buf_point >= 10){
		Temp_buf_point = 0;
		Temp_collect_flg = Temp_collect_stop;
		L1_flash_flg = L1_flash_start;
		Win_State = Win_State_3;
	}
}
void Windows_State_3(){
	if(L1_flash_flg == L1_flash_start){
		if(L1_flash_state)
			L1_disp(LED_ON);
		else L1_disp(LED_OFF);		
	}
	else L1_disp(LED_OFF);
	SmgBuf[0] = 11;
	SmgBuf[1] = Temp_buf_point / 10;
	SmgBuf[2] = Temp_buf_point % 10;
	SmgBuf[3] = 10;
	SmgBuf[4] = 10;
	SmgBuf[5] = 11;
	SmgBuf[6] = Temp_buf[Temp_buf_point] / 10;
	SmgBuf[7] = Temp_buf[Temp_buf_point] % 10;
}
void Windows_State(){
	switch(Win_State){
		case Win_State_1:
			Windows_State_1();
			break;
		case Win_State_2:
			Windows_State_2();
			break;			
		case Win_State_3:
			Windows_State_3();
			break;
	}	
} 

/**********************��ʼ������*******************/
void Timer0Init(void)		//1����@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
//	TL0 = 0xCD;		//���ö�ʱ��ֵ		
//	TH0 = 0xD4;		//���ö�ʱ��ֵ
	TL0 = 0x9A;		//���ö�ʱ��ֵ
	TH0 = 0xA9;		//���ö�ʱ��ֵ
//	TL0 = 0x00;		//���ö�ʱ��ֵ
//	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;
	EA = 1;
}

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x40;		//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//�趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
	TL1 = 0xE0;		//�趨��ʱ��ֵ
	TH1 = 0xFE;		//�趨��ʱ��ֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
}
void Board_init(){
	P2 = U24[5];	 //�ط�����
	P0 = 0x00;

	P2 = U24[LED];	 //��LED
	P0 = 0xff;
}


void system_init(){
	Board_init();				 
	UartInit();				    
	Timer0Init();
	Temp_read();
	Temp_read();
}

/**********************������**********************/
void main(){
	system_init();
    while(1)
    {
		Windows_State();
		if(Key_scan_flg){
			Key_scan_flg = 0;
			Key_Scan();	
			Key_Ack();
		}		
	/*	if(Temp_disp_flg == 1){
			Temp_disp_flg = 0;

			SBUF = Temp_read();
		} */
    }
}
/**********************�ж����********************/
void Timer0() interrupt 1
{		 
	time_base_2ms++;
	Smg_disp(SmgBuf);
	if(time_base_2ms % 5 == 0){
		Key_scan_flg = 1;	
	}  
	if(time_base_2ms % 100 == 0){
	//	Temp_disp_flg = 1;
		if(L1_flash_flg == L1_flash_start){
			L1_flash_state = ~L1_flash_state;
		}		  
	}	
	if(time_base_2ms % 250 == 0){
		Uart_send_flg = 1;
		read_time_flg = 1;
	}	 		   	
	if(time_base_2ms % 500 == 0){
		prompt_flash_flg = ~prompt_flash_flg;
	}
	if(Temp_collect_flg == Temp_collect_start){
		switch(Temp_interval_time){
			case Temp_interval_1s:
				if(time_base_2ms % 500 ==0){
					Temp_collect_reset = 1;						
				}
				break;
			case Temp_interval_5s:
				if(time_base_2ms % (500*5) ==0){
					Temp_collect_reset = 1;
				}
				break;
			case Temp_interval_30s:
				if(time_base_2ms % (500*30) ==0){
					Temp_collect_reset = 1;					
				}
				break;
			case Temp_interval_60s:
				if(time_base_2ms % (500*60) ==0){
					Temp_collect_reset = 1;
				}
				break; 
		}								 
	}
	
}
void Uart1() interrupt 4
{
	TI = 0;	
}
		  