#include "K60S2.h"
#include "intrins.h"
#include "absacc.h"
#include "DS1302.h"
#include "iic.h"

#define RESET 0
#define SET 1

#define write_com(a,b) {P0 = a; P2 = b;}


/**********************��������**********************/
/*------------ϵͳȫ��-------------*/
ulong time_base_2ms = 0;  
bit uart_send_flg = RESET;

#define K 0.38671875
uchar current_humidity;
uchar humidity_threshold = 50;
bit  humidity_disp_flg = RESET;
bit humidity_threshold_set_flg = RESET;
enum{
	water_state_close,
	water_state_open	
} water_state = water_state_close;
enum{
	system_work_state_auto,
	system_work_state_manul
}system_work_state;
enum{
	system_state_pilot_L1,
	system_state_pilot_L2		
}system_state_pilot;


/*---------------U24[]����-----------*/
code uchar U24[] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0};
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
bit prompt_flash_flg = RESET;

#define LED_ON    0
#define LED_OFF	  1


/*---------------������ & �̵���--------------*/
bit buzzer_warn_flg = SET;
enum{
	buzzer_state_close,
	buzzer_state_open	
} buzzer_state;



/*---------------����ܶ���-----------*/
code uchar Smg_Duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff,0xbf};
		             //  0     1     2      3     4     5     6     7     8     9  ��     -
uchar Smg_add = 0;
uchar SmgBuf[]={10,10,10,10,10,10,10,10};
bit smg_changed_flg = RESET;
			  //   ȫ��
/*--------------������ && �̵���--------------*/
#define UNL  5
#define relay_act 1
#define relay_stop 0

/*---------------���̶���-----------*/
bit Key_scan_flg = RESET;

/*---------------DS1302-----------*/
bit read_time_flg = SET;

/*---------------PCF8591-----------*/
bit AD_read_flg = RESET;

/*---------------AT24B02-----------*/
bit EEPROM_read_flg = RESET;
bit EEPROM_write_flg = RESET;

/*--------------���̶���-------------------*/
typedef enum{
	Key_Free,
	Key_S4,
	Key_S5,
	Key_S6,
	Key_S7
} KeyValue_Typedef;
KeyValue_Typedef Key_Value = Key_Free;


/**********************������ & �̵���****************/
void Buzzer(bit state){
	if(state == 1){	 //��
		P2 = U24[UNL];
		P0 = P0 | (0x01 << 6); 
	}
	else{
		P2 = U24[UNL];
		P0 = P0 & (~(0x01 << 6)); 
	}
}
void Relay(bit state){
	if(state == 1){	 //��
		P2 = U24[UNL];
		P0 = P0 | (0x01 << 4);
	}
	else{
		P2 = U24[UNL];	
		P0 = P0 & (~(0x01 << 4));		
	}
}


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
void L2_disp(bit state){
	P2 = 0x1f;
	P0 = 0xff;
	P2 = U24[4];
	if(state == LED_ON)
		P0 = 0xfd;
	else P0 = 0xff; 
	P2 = 0x1f;
	P0 = 0x00;
}

/**********************�����**********************/
void Smg_disp(uchar SmgBuf[]){
	P2 = U24[WEI];
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
void Put_S4(){
	if(system_work_state == system_work_state_auto){
		if(humidity_threshold_set_flg == SET){
			humidity_threshold--;	
			smg_changed_flg = RESET;		
		}
	}
	else{
		water_state = water_state_close;
	}
}
void Put_S5(){
	if(system_work_state == system_work_state_auto){
		if(humidity_threshold_set_flg == SET){
			humidity_threshold++;
			smg_changed_flg = RESET;
		}
	}
	else{
		water_state = water_state_open;
	}
}
void Put_S6(){
	if(system_work_state == system_work_state_auto){
		humidity_threshold_set_flg = ~humidity_threshold_set_flg;
		smg_changed_flg = RESET;
		if(humidity_threshold_set_flg == SET){
			humidity_disp_flg = RESET;			
		}
		else{
			humidity_disp_flg = SET;	
			EEPROM_write(0x03,humidity_threshold);
		}
	}
	else{
		buzzer_warn_flg = ~buzzer_warn_flg;			
	}
}
void Put_S7(){
	system_work_state++;
	if(system_work_state > system_work_state_manul)
		system_work_state = system_work_state_auto;	
	smg_changed_flg = RESET;
}

void Key_Ack(){
	switch(Key_Value){
		case Key_S4:
			Put_S4();
			break;
		case Key_S5:
			Put_S5();
			break;
		case Key_S6:
			Put_S6();
			break;
		case Key_S7:
			Put_S7();
			break;
	}
	Key_Value = Key_Free;			
}

/*********************����״̬**********************/
void windows_def(){
	if(prompt_flash_flg ==SET){
			SmgBuf[2] = 11;	 
		}
		else SmgBuf[2] = 10;
	if(smg_changed_flg == RESET){
		uchar tmp;
		tmp =  Ds1302_Single_Byte_Read(0x83);
		SmgBuf[3] = tmp / 10;
		SmgBuf[4] = tmp % 10;
		tmp =  Ds1302_Single_Byte_Read(0x85);
		SmgBuf[0] = tmp / 10;
		SmgBuf[1] = tmp % 10; 
		SmgBuf[6] = current_humidity / 10;
		SmgBuf[7] = current_humidity % 10;
		smg_changed_flg = SET;
	}   //�����ʼ��
	if(read_time_flg == SET){
		uchar tmp;
		read_time_flg = RESET;
		tmp = Ds1302_Single_Byte_Read(0x81);
	   	if((tmp == 0x59) || (tmp == 0x00) || (tmp == 0x01)){
			tmp = Ds1302_Single_Byte_Read(0x83);	
			SmgBuf[3] = tmp / 10;
			SmgBuf[4] = tmp % 10;
			smg_changed_flg = SET;
			if(tmp == 0x00){
				tmp = Ds1302_Single_Byte_Read(0x85);
				SmgBuf[0] = tmp / 10;
				SmgBuf[1] = tmp % 10;
				smg_changed_flg = SET;	
			}	
		}
	}   //��ʱ����ʾ
	if(humidity_disp_flg == SET){
		SmgBuf[6] = current_humidity / 10;
		SmgBuf[7] = current_humidity % 10;
		humidity_disp_flg = RESET;
		smg_changed_flg = SET;	
	} //��ǰ��ֵ��ʾ		
}
void System_auto_mode(){
	system_state_pilot = system_state_pilot_L1;
	Buzzer(RESET);	
	if(humidity_threshold_set_flg == RESET){
		windows_def();
		if(current_humidity < humidity_threshold){
			Relay(SET);	
		}
		else{
			Relay(RESET);	
		}			
	}
	else{
		if(smg_changed_flg == RESET){
			SmgBuf[0] = 11;
			SmgBuf[1] = 11;
			SmgBuf[2] = 10;
			SmgBuf[3] = 10;
			SmgBuf[4] = 10;
			SmgBuf[5] = 10;
			SmgBuf[6] = humidity_threshold / 10;
			SmgBuf[7] = humidity_threshold % 10;
			smg_changed_flg = SET;
		}	 //���������ʼ��	
	}
}
void System_manul_mode(){
	system_state_pilot = system_state_pilot_L2;
	windows_def();
	if(water_state == water_state_open){
		Relay(SET);
	}
	else{
		Relay(RESET);
	}
	if(buzzer_warn_flg == SET){
		if(current_humidity < humidity_threshold){
			Buzzer(SET);
		}
		else{
			Buzzer(RESET);
		}
	}
}
void System_work(){
	switch(system_work_state){
		case system_work_state_auto:
			System_auto_mode();
			break;
		case system_work_state_manul:
			System_manul_mode();
			break;			
	}	
} 
void System_pilot(){
	switch(system_state_pilot){
		case system_state_pilot_L1:
			L2_disp(LED_OFF);
			L1_disp(LED_ON);		
			break;
		case system_state_pilot_L2:
			L1_disp(LED_OFF);
			L2_disp(LED_ON);
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
	P2 = U24[5];	 //�ط����� & �̵���
	P0 = 0x00;
	

	P2 = U24[LED];	 //��LED
	P0 = 0xff;
}


void system_init(){
	Board_init();				 
	UartInit();				    
	Timer0Init();
	AD_init();
	DS1302_Init();
}

/**********************������**********************/
void main(){
	system_init();
	EEPROM_write(2,0x44);
    while(1)
    {
		System_work();
		if(Key_scan_flg == SET){
			Key_scan_flg = 0;
			Key_Scan();	
			Key_Ack();
		}
		if(AD_read_flg == SET){
			AD_read_flg = RESET;
			current_humidity = (uchar)(K * AD_read() + 0.5);
			humidity_disp_flg = SET;			
		}	
    }
}
/**********************�ж����********************/
void Timer0() interrupt 1
{		 
	time_base_2ms++;
	Smg_disp(SmgBuf);
	System_pilot();
	if(time_base_2ms % 5 == 0){
		Key_scan_flg = SET;	
	} 
	if(time_base_2ms % 50 == 0){
		AD_read_flg = SET;
	} 
	if(time_base_2ms % 250 == 0){
		read_time_flg = SET;
	}
	if(time_base_2ms % 500 == 0){
		prompt_flash_flg = ~prompt_flash_flg;
	}
	
}
void Uart1() interrupt 4
{
	TI = 0;	
}
		  