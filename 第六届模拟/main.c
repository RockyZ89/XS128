#include "ds18b20.h"
#include "K60S2.h"


/****************************变量 & 宏定义************************/
/*---------------系统全局-------------*/
#define RESET 0
#define SET 1
#define Write_138(com , dat) {P2 = com; P0 = dat;}

ulong time_base_2ms = 0;
enum{
	win_state_def,
	win_state_edit
}win_state ;//= win_state_edit;


/*---------------LED-------------*/
#define LED_ON 0
#define LED_OFF 1

bit L1_flash_flg = SET;
enum{
	L1_flash_interval_2,
	L1_flash_interval_4,
	L1_flash_interval_8
}L1_flash_interval;

/*---------------数码管-------------*/
code uchar smg_code[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90,0xff,0xbf};
uchar smg_buf[8]={10,10,10,10,10,10,10,1};
bit smg_changed_flg = RESET; 

/*---------------键盘-------------*/
uchar key_value = 0;
bit key_scan_flg = SET;
uchar key_buf[4]={10,10,10,10};
uchar key_buf_point = 0;
bit put_set_key_flg = RESET;
bit input_finished_flg = RESET;

/*---------------DS18B20-------------*/
bit temp_read_flg = SET;
uchar temp_max = 30;
uchar temp_min = 20;
uchar current_temp = 0;
bit temp_arg_err_flg = RESET;
enum{
	temp_interval_0,
	temp_interval_1,
	temp_interval_2
}temp_interval;


/*-----------蜂鸣器 & 继电器-----------*/
#define RELAY_ON 0
#define RELAY_OFF 1

/**************************函数************************/
/*=================LED================*/
void Led_disp(uchar L_x , bit state){
	if(state == LED_ON){
		switch(L_x){
			case 1:
				Write_138(0x1f , 0xff);
				Write_138(0x04<<5 , 0xfe);
				Write_138(0x1f , 0x00);
				break;
			case 2:
				Write_138(0x1f , 0xff);
				Write_138(0x04<<5 , 0xfd);
				Write_138(0x1f , 0x00);
				break;		
		}
	}
	else{
		switch(L_x){
			case 1:
				Write_138(0x1f , 0xff);
				Write_138(0x04<<5 , 0xff);
				Write_138(0x1f , 0x00);
				break;
			case 2:
				Write_138(0x1f , 0xff);
				Write_138(0x04<<5 , 0xff);
				Write_138(0x1f , 0x00);
				break;		
		}
	}	
}
/*================数码管================*/
void Smg_disp(){
	static uchar smg_addr = 0;
	Write_138(0x06<<5 , 0x00);

	Write_138(0x1f , 0xff);
	Write_138(0x07<<5 , smg_code[smg_buf[smg_addr]]);
  	Write_138(0x1f , 0x00);
	Write_138(0x06<<5 , 0x01 << smg_addr);	

	Write_138(0x1f , 0x00);

	smg_addr++;
	if(smg_addr >= 8) smg_addr = 0;		
}

/*================蜂鸣器 & 继电器================*/
void Relay(bit state){
	if(state == RELAY_ON){
		Write_138(0x05<<5 , 0x01<<4);			
	}
	else{
		Write_138(0x05<<5 , 0x00);
	}	
} 

/*================矩阵键盘================*/
void Key_scan(){
	uchar row = 1;
	uchar col = 0;
	uchar tmp = 0;

	P3 = 0x0f;
	P42 = 0;
	P44 = 0;
	tmp = P3 & 0x0f;  
	if(tmp != 0x0f){
		switch(tmp){
			case 0x0e:
				row = 1;
				break;
			case 0x0d:
				row = 2;
				break;
			case 0x0b:
				row = 3;
				break;
			case 0x07:
				row = 4;
				break;
			
		}
		P3 = 0xf0;
		P42 = 1;
		P44 = 1;
		if(P44 == 0) col = 1; 		
		else if(P42 == 0) col = 2;	
		else if(P35 == 0) col =3;	
		else if(P34 == 0) col = 4;
		while(!P44 || !P42 || !P35 || !P34) ;	
		key_value = 4*(row-1) + col; 
	}
	else key_value = 0;	 
}
void Put_set_key(){
	if(win_state == win_state_def){
		win_state = win_state_edit;
		key_buf[0] = 10;
		key_buf[1] = 10;
		key_buf[2] = 10;
		key_buf[3] = 10;
		key_buf_point = 0;
		input_finished_flg = RESET;
		smg_changed_flg = RESET;
   	}
	else{
		if(input_finished_flg == SET && temp_arg_err_flg == RESET){
			win_state = win_state_def;
			smg_changed_flg = RESET;	
		}		
	}		
}
void Put_reset_key(){
	if(win_state == win_state_edit){
		key_buf[0] = 10;
		key_buf[1] = 10;
		key_buf[2] = 10;
		key_buf[3] = 10;
		input_finished_flg = RESET;		
		temp_arg_err_flg = RESET;
		key_buf_point = 0;
	}
}
void Key_ack(){
	if(key_value != 0){
		switch(key_value){
			case 1:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 0;
					key_buf_point++;
				}
				break;	
			case 2:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 1;
					key_buf_point++;
				}
				break;
			case 3:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 2;
					key_buf_point++;
				}
				break;
			case 5:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 3;
					key_buf_point++;
				}
				break;
			case 6:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 4;
					key_buf_point++;
				}
				break;
			case 7:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 5;
					key_buf_point++;
				}
				break;
			case 9:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 6;
					key_buf_point++;
				}
				break;
			case 10:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 7;
					key_buf_point++;
				}
				break;
			case 11:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 8;
					key_buf_point++;
				}
				break;	
			case 13:
				if(input_finished_flg == RESET){
					key_buf[key_buf_point] = 9;
					key_buf_point++;
				}
				break;
			case 14:
				Put_set_key();			
				break;
			case 15:
				Put_reset_key();
				break;		
		}
		key_value = 0;
		if(key_buf_point >= 4) input_finished_flg = SET;		
	}		
}

/*================界面状态================*/
void Windows_def(){
	if(current_temp < temp_min){
		temp_interval = temp_interval_0;
		L1_flash_interval = L1_flash_interval_2;
		Relay(RELAY_OFF);
	}
	else if(current_temp >= temp_min && current_temp <= temp_max){
		temp_interval = temp_interval_1;
		L1_flash_interval = L1_flash_interval_4;
		Relay(RELAY_OFF);	
	}
	else{
		temp_interval = temp_interval_2;
		L1_flash_interval = L1_flash_interval_8;
		Relay(RELAY_ON);	
	}
	smg_buf[1] = temp_interval;
	smg_buf[6] = current_temp / 10;
	smg_buf[7] = current_temp % 10;
	if(smg_changed_flg == RESET){
		smg_buf[0] = 11;
		smg_buf[2] = 11;
		smg_buf[3] = 10;
		smg_buf[4] = 10;
		smg_buf[5] = 10;
		smg_changed_flg = SET;
	}
	if(L1_flash_flg == SET){
		Led_disp(1,LED_ON);		
	}
	else{
		Led_disp(1,LED_OFF);	
	}
}
void Windows_edit(){
	Led_disp(1,LED_OFF);
	if(smg_changed_flg == RESET){
		smg_buf[0] = 11;
		smg_buf[5] = 11;
		smg_buf[3] = 10;
		smg_buf[4] = 10;
		smg_changed_flg = SET;	
	}
	smg_buf[1] = key_buf[0];
	smg_buf[2] = key_buf[1];
	smg_buf[6] = key_buf[2];
	smg_buf[7] = key_buf[3];
	if(input_finished_flg == SET){
		temp_max = key_buf[0] * 10 + key_buf[1];
		temp_min = key_buf[2] * 10 + key_buf[3];
		if(temp_max < temp_min){
			temp_arg_err_flg = SET;
			Led_disp(2,LED_ON);
		}
		else{
			temp_arg_err_flg = RESET;
			Led_disp(2,LED_OFF);
		}
	}			
}
void Windows_disp(){
	switch(win_state){
		case win_state_def:
			Windows_def();
			break;
		case win_state_edit:
			Windows_edit();
			break;
	}
}
/*==============系统初始化=============*/
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
void System_init(){
	Write_138(0x05<<5, 0x00); //关蜂鸣器
	Write_138(0x04<<5, 0xff); //关LED				 
	UartInit();				    
	Timer0Init();
	Read_temp();
	Read_temp();
}
/*================主函数================*/
void main(){
	System_init();
	while(1){
		Windows_disp();
		if(key_scan_flg == SET){
			key_scan_flg = RESET;	
			Key_scan();
			Key_ack();
		}	   
		if(temp_read_flg == SET){
			temp_read_flg = RESET;
			current_temp = Read_temp();
		}	 
	} 
}
/*================中断入口==============*/
void Timer0() interrupt 1
{
	time_base_2ms++;
	Smg_disp();
   	if(time_base_2ms % 10 == 0){
		key_scan_flg = SET;
	}
	if(time_base_2ms % 250 == 0){
		temp_read_flg = SET;
	}
	if(time_base_2ms % 100 == 0){
		switch(L1_flash_interval){
			case L1_flash_interval_2:
					L1_flash_flg = ~L1_flash_flg;
				break;
			case L1_flash_interval_4:
				if(time_base_2ms % 200 == 0){
					L1_flash_flg = ~L1_flash_flg;
				}
				break;
			case L1_flash_interval_8:
				if(time_base_2ms % 400 == 0){
					L1_flash_flg = ~L1_flash_flg;
				}
				break;	 
		}
	}
}
void Uart0() interrupt 4{
	TI = 0;
}
