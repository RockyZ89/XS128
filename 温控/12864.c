#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "12864.h"


void delay_10us(int n)         //10us 
{     
	int x;     
	while(n--)     
		for(x=0;x<26;x++)     
		{        
			_asm(nop);       
			_asm(nop);       
			_asm(nop);       
			_asm(nop);       
			_asm(nop);       
			_asm(nop);     
		} 
} 

void init_12864(){
  DDRK = 0x0f;      //设置控制E RW RS PSB方向输出
	PSB = 1;          //选择并行方式
	write_com_12864(0x30);		 //使用基本指令集
	write_com_12864(0x30);		 //使用基本指令集
	write_com_12864(0x0c);		 //开显示关游标
	write_com_12864(0x06);		 //数据指针自加
	write_com_12864(0x01);		 //显示清屏
	
	clr_GDR_12864(2);  //清下半屏
}

void check_busy_12864(){
  uint t = 0;
  DDRA = 0x00;          //输入
  E= 0;
  RS = 0;
  RW = 1;
  E = 1;
  
  while((PORTA & 0x80) && ++t != 0);
  E = 0;
  _asm(nop);
  DDRA = 0xff;             //输出
}

void write_com_12864(uchar com){
  check_busy_12864();
	check_busy_12864(); 
	RS=0;  
	RW=0;  
	E=1;  
	PORTA = com;  
	delay_10us(1);  
	E=0;  
	delay_10us(3); 		
}
void write_dat_12864(uchar dat){
  check_busy_12864();
	RS=1;  
	RW=0;  
	E=1;  
	PORTA = dat;  
	delay_10us(1);  
	E=0;  
	delay_10us(3); 
}
uchar read_dat_12864(){
  uchar dat=0;
  DDRA = 0x00;      //设置数据方向输入
  E=0;
  RS=1;
  RW=1;
  E=1;
  delay_ms(1);
  dat = PORTA;
  delay_ms(1);
  E=0;
  return dat;
}
void write_ch_12864(uchar add,uchar ch){
 	write_com_12864(add);
	write_dat_12864(ch);
}
void write_num_12864(uchar num,uchar add){
	uchar shwe,gewe;
	shwe= num/ 10;
	gewe= num% 10;
	write_com_12864(add);
	write_dat_12864(0x30+shwe);
	write_dat_12864(0x30+gewe);
}

void write_str_12864(uchar* str,uchar add,uchar count){
 	write_com_12864(add);					  						  				//输出字符串
	while(count){	
		write_dat_12864 (*str);
		str++;
		count--;			
	}
}
void draw_dot_12864(uchar x,uchar y){
  uint Wb=0,temp=0;             //WhichBit , temp接收一个字的数据
  uchar x_add=0,y_add=0,Hb=0,Lb=0;    
  y_add = 0x80 + y % 32;    //计算Y轴地址，地址为 0x80加上 行号  
  if(y > 31)                //下半屛
    x_add = 0x88 + x / 16;  //计算X轴地址
  else x_add = 0x80 + x / 16;  //计算X轴地址
  Wb = 0x8000 >> (x%16);     //判断此点在两字节中哪一位置
  write_com_12864(0x34);
  write_com_12864(0x36);
  write_com_12864(y_add);
  write_com_12864(x_add);
  temp = read_dat_12864();          //先假读一次
  temp = read_dat_12864();
  temp <<= 8;
  temp = temp | read_dat_12864();    //将temp处理成一个字
  Wb = Wb | temp;                    //将新增的点加进原来的字
  Hb = Wb >> 8;                      //高位
  Lb = Wb;                           //低位 .
  write_com_12864(y_add);
  write_com_12864(x_add);
  write_dat_12864(Hb);
  write_dat_12864(Lb);
  write_com_12864(0x30);             //转回基本指令集，用于写字符
} 

void clr_GDR_12864(uchar flag){          //GDR 0全 1 上  2下 
  uchar i,j;
  write_com_12864(0x34);             //扩展指令集
  write_com_12864(0x36);
  if(flag == 0){           //上
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //写Y坐标
      write_com_12864(0x80);           //写X坐标
      for(j=0;j<32;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag == 1){           //上
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //写Y坐标
      write_com_12864(0x80);           //写X坐标
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag== 2){            //下
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //写Y坐标
      write_com_12864(0x88);           //写X坐标
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  write_com_12864(0x30);      //基本指令集
}                
void draw_curve_12864(double temp)                   //画曲线,每次只画一个点，输入温度值
{
	static uchar x = 0;
	int y;
	if(x >= 128)                             //x到头从头再来
	{
		x = 0;
		clr_GDR_12864(2);
	}
	y = 63 - (int)((temp - 25)/2.0 +0.5); 
	draw_dot_12864(x, y);
	x++;
}
