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
  DDRK = 0x0f;      //���ÿ���E RW RS PSB�������
	PSB = 1;          //ѡ���з�ʽ
	write_com_12864(0x30);		 //ʹ�û���ָ�
	write_com_12864(0x30);		 //ʹ�û���ָ�
	write_com_12864(0x0c);		 //����ʾ���α�
	write_com_12864(0x06);		 //����ָ���Լ�
	write_com_12864(0x01);		 //��ʾ����
	
	clr_GDR_12864(2);  //���°���
}

void check_busy_12864(){
  uint t = 0;
  DDRA = 0x00;          //����
  E= 0;
  RS = 0;
  RW = 1;
  E = 1;
  
  while((PORTA & 0x80) && ++t != 0);
  E = 0;
  _asm(nop);
  DDRA = 0xff;             //���
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
  DDRA = 0x00;      //�������ݷ�������
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
 	write_com_12864(add);					  						  				//����ַ���
	while(count){	
		write_dat_12864 (*str);
		str++;
		count--;			
	}
}
void draw_dot_12864(uchar x,uchar y){
  uint Wb=0,temp=0;             //WhichBit , temp����һ���ֵ�����
  uchar x_add=0,y_add=0,Hb=0,Lb=0;    
  y_add = 0x80 + y % 32;    //����Y���ַ����ַΪ 0x80���� �к�  
  if(y > 31)                //�°��
    x_add = 0x88 + x / 16;  //����X���ַ
  else x_add = 0x80 + x / 16;  //����X���ַ
  Wb = 0x8000 >> (x%16);     //�жϴ˵������ֽ�����һλ��
  write_com_12864(0x34);
  write_com_12864(0x36);
  write_com_12864(y_add);
  write_com_12864(x_add);
  temp = read_dat_12864();          //�ȼٶ�һ��
  temp = read_dat_12864();
  temp <<= 8;
  temp = temp | read_dat_12864();    //��temp�����һ����
  Wb = Wb | temp;                    //�������ĵ�ӽ�ԭ������
  Hb = Wb >> 8;                      //��λ
  Lb = Wb;                           //��λ .
  write_com_12864(y_add);
  write_com_12864(x_add);
  write_dat_12864(Hb);
  write_dat_12864(Lb);
  write_com_12864(0x30);             //ת�ػ���ָ�������д�ַ�
} 

void clr_GDR_12864(uchar flag){          //GDR 0ȫ 1 ��  2�� 
  uchar i,j;
  write_com_12864(0x34);             //��չָ�
  write_com_12864(0x36);
  if(flag == 0){           //��
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //дY����
      write_com_12864(0x80);           //дX����
      for(j=0;j<32;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag == 1){           //��
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //дY����
      write_com_12864(0x80);           //дX����
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  if(flag== 2){            //��
    for(i=0;i<32;i++){
      write_com_12864(0x80+i);         //дY����
      write_com_12864(0x88);           //дX����
      for(j=0;j<16;j++)
        write_dat_12864(0x00);
    }
  }
  write_com_12864(0x30);      //����ָ�
}                
void draw_curve_12864(double temp)                   //������,ÿ��ֻ��һ���㣬�����¶�ֵ
{
	static uchar x = 0;
	int y;
	if(x >= 128)                             //x��ͷ��ͷ����
	{
		x = 0;
		clr_GDR_12864(2);
	}
	y = 63 - (int)((temp - 25)/2.0 +0.5); 
	draw_dot_12864(x, y);
	x++;
}
