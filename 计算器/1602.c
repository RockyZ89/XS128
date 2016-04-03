#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "1602.h"


/*********************1602��ʼ��*********************/
void init_1602(){
  DDRK = 0x07;      //���ÿ���E RW RS�������
  DDRA = 0xff;      //�������ݷ������
	RW= 0;
	write_com_1602(0x38);		   //��ʾģʽ����
	write_com_1602(0x0f);		   //����ʾ���Թ��
	write_com_1602(0x06);		   //����ָ���Լ�
	write_com_1602(0x01);		   //��ʾ����
}

/***********************дָ��***********************/
void write_com_1602(uchar com){
	E= 0;
	RS= 0;
	PORTA = com;
	delay_ms(1);
	E= 1;
	delay_ms(1);
	E= 0;
}

/***********************д����***********************/
void write_dat_1602(uchar dat){
	E= 0;
	RS= 1;
	PORTA= dat;
	delay_ms(1);
	E= 1;
  delay_ms(1);
	E= 0;
}

/**********************д���ַ�**********************/
void write_ch_1602(uchar add,uchar ch){
	write_com_1602(add);
	write_dat_1602(ch);
}

void write_str_1602(uchar* str,uchar add,uchar count){
	uchar i,sum=add+count;					  						  				//����ַ���
	for(i=add;i< sum;i++,str++){
			write_com_1602(i);
			write_dat_1602(*str);								 
	}			
}

/********************д����λ��*********************/
void write_num_1602(uchar add,uchar num){
	uchar gewe,shwe;
	gewe= num% 10;
	shwe= num/ 10;
	write_com_1602(add);						//������ָ���Լ�ģʽ
	write_dat_1602(0x30+shwe);
	write_dat_1602(0x30+gewe);
}



