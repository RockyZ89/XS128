#include "K60S2.h"
#include <intrins.h>


sbit DQ = P1^4;
//单总线延时函数
void Delay_OneWire(unsigned int t)
{
  uchar i;
  while(t--){
  	for(i=0; i<12; i++) ;	
  }
}

//DS18B20芯片初始化
bit Init_DS18B20(void)
{
	bit initflag = 0;
	DQ = 1;
	Delay_OneWire(12);
	DQ = 0;
	Delay_OneWire(80); 
	DQ = 1;
	Delay_OneWire(10); 
	initflag = DQ;    
	Delay_OneWire(5);
  
	return initflag;
}

//通过单总线向DS18B20写一个字节
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//从DS18B20读取一个字节
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

uchar Read_temp(){
	uchar temp_l,temp_h,temp;
//	EA = 0;
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	Delay_OneWire(200);
	Init_DS18B20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
//	EA = 1;
	temp_l = Read_DS18B20();
	temp_h = Read_DS18B20();
	temp = (temp_h << 4) | (temp_l >> 4);
	return temp; 
}


