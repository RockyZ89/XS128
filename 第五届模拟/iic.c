/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨(12MHz)
  ��    ��: 2011-8-9
*/

#include "iic.h"


void Delay5us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	i = 12;
	while (--i);
}

void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


//������������
void IIC_Start(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	SDA = 0;
	somenop;
	SCL = 0;
	somenop;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	somenop;
	SCL = 1;
	somenop;
	SDA = 1;
	somenop;
}

//Ӧ��λ����
void IIC_Ack(unsigned char ackbit)
{
	if(ackbit) 
	{	
		SDA = 0;
	}
	else 
	{
		SDA = 1;
	}
	somenop;
	SCL = 1;
	somenop;
	SCL = 0;
	somenop;
	SDA = 1; 
	somenop;
}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
	SDA = 1;
	somenop;
	SCL = 1;
	somenop;
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		somenop;
		SCL = 1;
		byt <<= 1;
		somenop;
		SCL = 0;
		somenop;
	}
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
	unsigned char da = 0;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		da <<= 1;
		SCL = 1;
		somenop;	
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		somenop;
	}
	return da;
}

//PCF8591
void AD_init(){
	IIC_Start();
	IIC_SendByte(0x90);
	IIC_WaitAck();
	IIC_SendByte(0x03);	
	IIC_WaitAck();
	IIC_Stop();
	Delay10ms();
}
uchar AD_read(){
	uint tmp= 0xaa;
	IIC_Start();
	IIC_SendByte(0x91);
	IIC_WaitAck();
	tmp = IIC_RecByte();
	IIC_Ack(0);
	IIC_Stop();
	Delay10ms();
	return tmp;
}


//EEPROM
void EEPROM_write(uchar addr, uchar dat){
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);	
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();	
	IIC_Stop();
	Delay10ms();		
}
/*
uchar EEPROM_read(uchar addr){
	uchar tmp = 0xaa;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);	
	IIC_WaitAck();	  

	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	tmp = IIC_RecByte();
	IIC_Ack(0);	
	IIC_Stop();
	Delay10ms();
	return tmp;
}		  */