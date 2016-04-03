#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "delay.h"
#include "SCI.h"


float OutData[4];

void init_SCI(){
  SCI1BD = 52;      //设置波特率为9600bps
  SCI1CR1 = 0X00;    //LOOPS=0,SCISWAI=0,RSRC=0,M=0
                     //WAKE=0,ILT=0,PE=0,PT=0
  SCI1CR2 = 0x0c;    //TIE=0,TCIE=0,RIE=0,ILIE=0
                     //TE=1,RE=1,RWU=0,SBK=0
  
}



/************************
*      virtual scope
************************/
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
	unsigned short CRC_Temp;
	unsigned char i, j;
	CRC_Temp = 0xffff;

	for (i = 0; i<CRC_CNT; i++){
		CRC_Temp ^= Buf[i];
		for (j = 0; j<8; j++) {
			if (CRC_Temp & 0x01)
				CRC_Temp = (CRC_Temp >> 1) ^ 0xa001;
			else
				CRC_Temp = CRC_Temp >> 1;
		}
	}
	return(CRC_Temp);
}

void OutPut_vs(void)
{
	int temp[4] = { 0 };
	unsigned int temp1[4] = { 0 };
	uchar databuf[10] = { 0 };
	unsigned char i;
	unsigned short CRC16 = 0;
	for (i = 0; i<4; i++)
	{

		temp[i] = (int)OutData[i];
		temp1[i] = (unsigned int)temp[i];

	}

	for (i = 0; i<4; i++)
	{
		databuf[i * 2] = (unsigned char)(temp1[i] % 256);
		databuf[i * 2 + 1] = (unsigned char)(temp1[i] / 256);
	}

	CRC16 = CRC_CHECK(databuf, 8);
	databuf[8] = CRC16 % 256;
	databuf[9] = CRC16 / 256;

	for (i = 0; i<10; i++)
		send_SCI(databuf[i]);
	//uart_putchar(databuf[i]);
}


void send_SCI(uchar dat){
  while(!SCI1SR1_TDRE) ;               
    SCI1DRL = dat; 
  //delay_ms(10); 
}