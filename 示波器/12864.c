#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "12864.h"



void init_12864(){
  DDRK = 0x0f;      //�1�7�1�7�1�7�0�7�1�7�1�7�1�7E RW RS PSB�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
	PSB = 1;          //�0�5�1�7�1�7�1�7�1�7�1�7�ل1�7�0�4
	write_com_12864(0x30);		 //�0�0�1�7�0�4�1�7�1�7�1�7�0�8�1�7�8�8
	write_com_12864(0x30);		 //�0�0�1�7�0�4�1�7�1�7�1�7�0�8�1�7�8�8
	write_com_12864(0x0c);		 //�1�7�1�7�1�7�1�7�0�5�1�7�1�7�1�7���1�7
	write_com_12864(0x06);		 //�1�7�1�7�1�7�1�7�0�8�1�7�1�7�1�7�0�4�1�7
	write_com_12864(0x01);		 //�1�7�1�7�0�5�1�7�1�7�1�7�1�7

}

void check_busy_12864(){
  DDRA = 0x00;          //�1�7�1�7�1�7�1�7
  E= 0;
  RS = 0;
  RW = 1;
  E = 1;
  while(PORTA & 0x80);
  E = 0;
  _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
  _asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
  DDRA = 0xff;             //�1�7�1�7�1�7�1�7
}

void write_com_12864(uchar com){
  check_busy_12864();
	RS=0;  
	RW=0;  
	E=1;  
	PORTA = com;    
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
	E=0;
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);  		
}
void write_dat_12864(uchar dat){
  check_busy_12864();
	RS=1;  
	RW=0;  
	E=1;  
	PORTA = dat;   
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop);
	E=0;  
	_asm(nop);_asm(nop);_asm(nop);_asm(nop);_asm(nop); 
}
uchar read_dat_12864(){
  uchar dat=0;
  DDRA = 0x00;      //�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�5�1�7�1�7�1�7�1�7�1�7�1�7�1�7
  E=0;
  RS=1;
  RW=1;
  E=1;
  delay_us(3);
  dat = PORTA;
  delay_us(3);
  E=0;
  return dat;
}
void write_ch_12864(uchar add,uchar ch){
  write_com_12864(0x30);
 	write_com_12864(add);
	write_dat_12864(ch);
}
void write_num_12864(uchar num,uchar add){
	uchar shwe,gewe;
	shwe= num/ 10;
	gewe= num% 10;
	write_com_12864(0x30);
	write_com_12864(add);
	write_dat_12864(0x30+shwe);
	write_dat_12864(0x30+gewe);
}

void write_str_12864(uchar* str,uchar add,uchar count){
  write_com_12864(0x30);
 	write_com_12864(add);					  						  				//�1�7�1�7�1�7�1�7�1�7�0�7�1�7�1�7�1�7
	while(count){	
		write_dat_12864 (*str);
		str++;
		count--;			
	}
}
void draw_dot_12864(uchar x,uchar y){
  uint Wb=0,temp=0;             //WhichBit , temp�1�7�1�7�1�7�1�7�0�5�1�7�1�7�1�7�0�5�1�7�1�7�1�7�1�7�1�7
  uchar x_add=0,y_add=0,Hb=0,Lb=0;    
  y_add = 0x80 + y % 32;    //�1�7�1�7�1�7�1�7Y�1�7�1�7�1�7�1�7�0�7�1�7�1�7�1�7�1�7�0�7�0�2 0x80�1�7�1�7�1�7�1�7 �1�7�܄1�7  
  if(y > 31)                //�1�7��1�7�1�7�1�7
    x_add = 0x88 + x / 16;  //�1�7�1�7�1�7�1�7X�1�7�1�7�1�7�1�7�0�7
  else x_add = 0x80 + x / 16;  //�1�7�1�7�1�7�1�7X�1�7�1�7�1�7�1�7�0�7
  Wb = 0x8000 >> (x%16);     //�1�7�؁0�2�0�5�1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�0�5�˄1�7�1�7
  write_com_12864(0x34);
  write_com_12864(y_add);
  write_com_12864(x_add);
  temp = read_dat_12864();          //�1�7�0�7�1�8�1�7�0�5�1�7�1�7
  temp = read_dat_12864();
  temp <<= 8;
  temp = temp | read_dat_12864();    //�1�7�1�7temp�1�7�1�7�1�7�1�7�1�7�1�7�0�5�1�7�1�7�1�7�1�7
  Wb = Wb | temp;                    //�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�7�1�7�1�7�0�1�1�7�0�9�1�7�1�7�1�7�1�7�1�7�1�7
  Hb = Wb >> 8;                      //�1�7�1�7��
  Lb = Wb;                           //�1�7�1�7�� 
  write_com_12864(y_add);
  write_com_12864(x_add);
  write_dat_12864(Hb);
  write_dat_12864(Lb);
  write_com_12864(0x36);             //�0�8�1�7�1�9�1�7�1�7�1�7�0�8�1�7�8�8�1�7�1�7�1�7�1�7�1�7�1�7�Մ1�7�0�7�1�7
} 

void clr_GDR_12864(uchar flag){          //GDR 0�0�0 1 �1�7�1�7  2�1�7�1�7 
  uchar i,j;
  write_com_12864(0x34);             //�1�7�1�7�0�5�0�8�1�7�8�8
  if(flag == 0){           //�0�0
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //��Y�1�7�1�7�1�7�1�7
      write_com_12864(0x80);           //��X�1�7�1�7�1�7�1�7
      for(j=0;j<32;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag == 1){           //�1�7�1�7
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //��Y�1�7�1�7�1�7�1�7
      write_com_12864(0x80);           //��X�1�7�1�7�1�7�1�7
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag== 2){            //�1�7�1�7
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //��Y�1�7�1�7�1�7�1�7
      write_com_12864(0x88);           //��X�1�7�1�7�1�7�1�7
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  write_com_12864(0x36);      //�1�7�1�7�1�7�1�7�0�8�1�7�8�8
}                

void init_OSC_12864(){
  uint i,j;
  
  clr_GDR_12864(0);
	
  for(i=0;i<112;i++){  
    draw_dot_12864(i,0);         //�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�1�7�1�7�1�7
    draw_dot_12864(i,63);
  }                              //4 * 8 div
  for(i=0;i<64;i++){
    draw_dot_12864(0,i);         //�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
    draw_dot_12864(111,i);
  }  
  for(i=1;i<4;i++){
    draw_dot_12864(i*28,1);         //�0�8�0�9�1�7�0�1�1�7�1�7�1�7�0�228�1�7�1�7/div
    draw_dot_12864(i*28,2);
    draw_dot_12864(i*28,61);
    draw_dot_12864(i*28,62); 
    if(i==2){
      draw_dot_12864(i*28,3);
      draw_dot_12864(i*28,60);  
    }/* 
    for(j=1;j<8;j++){    
      draw_dot_12864(i*28,j*8);
    }*/
  }
  for(j=1;j<8;j++){
    draw_dot_12864(1,j*8);
    draw_dot_12864(2,j*8);
    draw_dot_12864(109,j*8);
    draw_dot_12864(110,j*8);
    if(j==4){
      draw_dot_12864(3,j*8);
      draw_dot_12864(108,j*8);
   /*   for(i=1;i<8;i++)
        draw_dot_12864(i*14,j*8);
        i++;      */
    }    
  }
 
}
