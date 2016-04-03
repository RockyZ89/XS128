#ifndef _IIC_H
#define _IIC_H

#include "K60S2.h"
#include "intrins.h"

#define somenop Delay5us()
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//�������Ŷ���
sbit SDA = P2^1;  /* ������ */
sbit SCL = P2^0;  /* ʱ���� */

//��������
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 

//  PCF8591
void AD_init();

uchar AD_read();


//  AT24C02
void EEPROM_write(uchar addr, uchar dat);

uchar EEPROM_read(uchar addr);

#endif