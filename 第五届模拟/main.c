#include "K60S2.h"
#include "intrins.h"
#include "absacc.h"
#include "DS1302.h"
#include "iic.h"

#define RESET 0
#define SET 1

#define write_com(a,b) {P0 = a; P2 = b;}


/**********************变量定义**********************/
/*------------系统全局-------------*/
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


/*---------------U24[]定义-----------*/
code uchar U24[] = {0x00, 0x20, 0x40, 0x60, 0x80, 0xa0, 0xc0, 0xe0};
/*
  3		 8255的CE
  4		 LED锁存
  5		 UNL 2003
  6		 数码管位选
  7		 数码管段选
*/
#define LED  4
#define WEI 6
#define DUAN 7

/*---------------LED定义--------------*/
bit prompt_flash_flg = RESET;

#define LED_ON    0
#define LED_OFF	  1


/*---------------蜂鸣器 & 继电器--------------*/
bit buzzer_warn_flg = SET;
enum{
	buzzer_state_close,
	buzzer_state_open	
} buzzer_state;



/*---------------数码管定义-----------*/
code uchar Smg_Duan[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff,0xbf};
		             //  0     1     2      3     4     5     6     7     8     9  无     -
uchar Smg_add = 0;
uchar SmgBuf[]={10,10,10,10,10,10,10,10};
bit smg_changed_flg = RESET;
			  //   全关
/*--------------蜂鸣器 && 继电器--------------*/
#define UNL  5
#define relay_act 1
#define relay_stop 0

/*---------------键盘定义-----------*/
bit Key_scan_flg = RESET;

/*---------------DS1302-----------*/
bit read_time_flg = SET;

/*---------------PCF8591-----------*/
bit AD_read_flg = RESET;

/*---------------AT24B02-----------*/
bit EEPROM_read_flg = RESET;
bit EEPROM_write_flg = RESET;

/*--------------键盘动作-------------------*/
typedef enum{
	Key_Free,
	Key_S4,
	Key_S5,
	Key_S6,
	Key_S7
} KeyValue_Typedef;
KeyValue_Typedef Key_Value = Key_Free;


/**********************蜂鸣器 & 继电器****************/
void Buzzer(bit state){
	if(state == 1){	 //打开
		P2 = U24[UNL];
		P0 = P0 | (0x01 << 6); 
	}
	else{
		P2 = U24[UNL];
		P0 = P0 & (~(0x01 << 6)); 
	}
}
void Relay(bit state){
	if(state == 1){	 //打开
		P2 = U24[UNL];
		P0 = P0 | (0x01 << 4);
	}
	else{
		P2 = U24[UNL];	
		P0 = P0 & (~(0x01 << 4));		
	}
}


/**********************LED流水灯**********************/
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

/**********************数码管**********************/
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



/**********************矩阵键盘********************/
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

/*********************工作状态**********************/
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
	}   //界面初始化
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
	}   //读时间显示
	if(humidity_disp_flg == SET){
		SmgBuf[6] = current_humidity / 10;
		SmgBuf[7] = current_humidity % 10;
		humidity_disp_flg = RESET;
		smg_changed_flg = SET;	
	} //当前阈值显示		
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
		}	 //调整界面初始化	
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

/**********************初始化程序*******************/
void Timer0Init(void)		//1毫秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
//	TL0 = 0xCD;		//设置定时初值		
//	TH0 = 0xD4;		//设置定时初值
	TL0 = 0x9A;		//设置定时初值
	TH0 = 0xA9;		//设置定时初值
//	TL0 = 0x00;		//设置定时初值
//	TH0 = 0x28;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0 = 1;
	EA = 1;
}

void UartInit(void)		//9600bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设定定时器1为16位自动重装方式
	TL1 = 0xE0;		//设定定时初值
	TH1 = 0xFE;		//设定定时初值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
}
void Board_init(){
	P2 = U24[5];	 //关蜂鸣器 & 继电器
	P0 = 0x00;
	

	P2 = U24[LED];	 //关LED
	P0 = 0xff;
}


void system_init(){
	Board_init();				 
	UartInit();				    
	Timer0Init();
	AD_init();
	DS1302_Init();
}

/**********************主函数**********************/
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
/**********************中断入口********************/
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
		  