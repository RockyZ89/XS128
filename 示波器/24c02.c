#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "24c02.h"




/*****************************I2C总线***********************/
void nop(){									//大约5us
	_asm(nop);
	_asm(nop);
	_asm(nop);
	_asm(nop);
	_asm(nop);  
}
void delay_i2c(uint ms){
	 uint i,j;
   for(i=0;i<ms;i++) 
    for(j=0;j<1600;j++)
      ; 
} 
void init_i2c(){
  scl_out;
  sda_out;  
	scl= 1;
	nop();
	sda= 1;
	nop();		
}
void start_i2c(){
  scl_out;
  sda_out;
	sda= 1;
	nop();
	scl= 1;
	nop();
	sda= 0;
	nop();	
}
void stop_i2c(){
  scl_out;
  sda_out;	
	sda= 0;
	nop();
	scl= 1;
	nop();
	sda= 1;
	nop();
}
void ack_i2c(){
	uchar i= 255;
	scl_out;
  sda_in;
	scl= 1;
	nop();
	while(sda && i>0)
		i--;
	scl= 0;
	nop();
}
void write_byte_i2c(uchar dat){				 //发送一个字节
	uchar i,temp=0;
	temp = dat;
	scl_out;
  sda_out;
	for(i=0;i<8;i++){	
		scl= 0;
		nop();
	  sda= (temp & 0x80) >> 7;
	  temp <<= 1;
		nop();
		scl= 1;
		nop();                         
	}	
	scl= 0;
	nop();
	sda= 1;
	nop();
}
uchar read_byte_i2c(){						 //接收一个字节
	uchar i,dat=0;
	scl_out;
  sda_in;
	scl= 0;
	nop();
	sda= 1;						     
	nop();								  
	for(i=0;i<8;i++){
		scl= 1;
		nop();
		dat=(dat<<1) | sda;
		scl= 0;
		nop();	
	}
	return dat;
}


/************************24c02****************************/
void write_byte_24c02(uchar dat,uchar add){							//给指定地址写一个字节数据
	delay_i2c(1);          //延时必不可少！！！
	start_i2c();
	write_byte_i2c(0xa0);	   //器件地址，写
	ack_i2c();
	write_byte_i2c(add);	   //发送器件指定地址
	ack_i2c();
	write_byte_i2c(dat);	   //写入一个字节数据
	ack_i2c();
	stop_i2c();	
}
uchar read_byte_24c02(uchar add){									//从指定地址读一个字节数据
	uchar dat=0;
	delay_i2c(1);           //延时必不可少！！！
	start_i2c();
	write_byte_i2c(0xa0);	   //器件地址，写									    
	ack_i2c();
	write_byte_i2c(add);	   //发送器件指定地址
	ack_i2c();
	start_i2c();
	write_byte_i2c(0xa1);	   //器件地址，读
	ack_i2c();   
	dat= read_byte_i2c();	   //读出一个字节数据
	return dat;
}

/*void write_dat_24c02(uchar *dat,uchar add,uint count){							//给指定地址写num个字节数据
	uint i;
	start_i2c();
	write_byte_i2c(0xa0);	   //器件地址，写
	ack_i2c();
	write_byte_i2c(add);	   //发送器件指定地址
	ack_i2c();
	for(i=0;i<count;i++){	  
	  write_byte_i2c(*dat);	   //写入一个字节数据
	  ack_i2c();
	  dat++;
	}
	stop_i2c();	
}
void read_dat_24c02(uchar *dat,uchar add,uint count){									//从指定地址读num个字节数据
	uint i;
	start_i2c();
	write_byte_i2c(0xa0);	   //器件地址，写									    
	ack_i2c();
	write_byte_i2c(add);	   //发送器件指定地址
	ack_i2c();
	start_i2c();
	write_byte_i2c(0xa1);	   //器件地址，读
	ack_i2c();
	for(i=0;i<count;i++){
    *dat= read_byte_i2c();	   //读出一个字节数据
    ack_i2c();
    dat++;
	}
	stop_i2c();   
}*/
