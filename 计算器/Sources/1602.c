#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "1602.h"


/*********************1602初始化*********************/
void init_1602(){
  DDRK = 0x07;      //设置控制E RW RS方向输出
  DDRA = 0xff;      //设置数据方向输出
	RW= 0;
	write_com_1602(0x38);		   //显示模式设置
	write_com_1602(0x0f);		   //开显示不显光标
	write_com_1602(0x06);		   //数据指针自加
	write_com_1602(0x01);		   //显示清屏
}

/***********************写指令***********************/
void write_com_1602(uchar com){
	E= 0;
	RS= 0;
	PORTA = com;
	delay_ms(1);
	E= 1;
	delay_ms(1);
	E= 0;
}

/***********************写数据***********************/
void write_dat_1602(uchar dat){
	E= 0;
	RS= 1;
	PORTA= dat;
	delay_ms(1);
	E= 1;
  delay_ms(1);
	E= 0;
}

/**********************写入字符**********************/
void write_ch_1602(uchar add,uchar ch){
	write_com_1602(add);
	write_dat_1602(ch);
}

void write_str_1602(uchar* str,uchar add,uchar count){
	uchar i,sum=add+count;					  						  				//输出字符串
	for(i=add;i< sum;i++,str++){
			write_com_1602(i);
			write_dat_1602(*str);								 
	}			
}

/********************写入两位数*********************/
void write_num_1602(uchar add,uchar num){
	uchar gewe,shwe;
	gewe= num% 10;
	shwe= num/ 10;
	write_com_1602(add);						//适用于指针自加模式
	write_dat_1602(0x30+shwe);
	write_dat_1602(0x30+gewe);
}



