#include "i2c.h"

unsigned char ack;

void I2C_Init(void){
  i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;  
	i2c_scl= 1;
	i2c_sda= 1;
}

void i2c_start(void){
  i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;
	i2c_sda= 1;
	//nop();
	delay_us(1);
	i2c_scl= 1;
	//nop();
	delay_us(5);
	i2c_sda= 0;
	//nop();
	delay_us(4);
	i2c_scl = 0;
	delay_us(2);	
}

void i2c_end(void){
  i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;	
	i2c_sda= 0;
	//nop();
	delay_us(1);
	i2c_scl= 1;
	//nop();
	delay_us(5);
	i2c_sda= 1;
	//nop();
	delay_us(4); 
}

void i2c_writebyte(unsigned char data){
  unsigned char i;
	i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;
	for(i=0;i<8;i++){	
		if((data << i) & 0x80)
		  i2c_sda = 1;
		else
		  i2c_sda = 0;
		delay_us(1);
		i2c_scl = 1;
		delay_us(5);
		i2c_scl = 0;
	}	
	delay_us(2);
	i2c_sda = 1;
	i2c_sda_ddr = 0;
	delay_us(2);
	i2c_scl = 1;
	delay_us(3);
	if(i2c_sda)
	  ack = 0;
	else
	  ack = 1;
	i2c_scl = 0;
	delay_us(2);
	i2c_sda_ddr = 1;
}
/*
void i2c_writebyte(uchar data){
  uchar i;
	i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;
	for(i=0;i<8;i++){	
		i2c_scl= 0;
  	//nop();
	  delay_us(4);
	  i2c_sda= (data & 0x80) >> 7;
	  data <<= 1;
		//nop();
	  delay_us(4);
		i2c_scl= 1;
		//nop();
  	delay_us(4);
	}	
	i2c_scl= 0;
	//nop();
	delay_us(4);
	i2c_sda= 1;
	//nop();
	delay_us(4);
}
*/

unsigned char i2c_readbyte(){
  unsigned char i,data = 0;
	i2c_scl_ddr = 1;
	i2c_sda_ddr = 0;			  
	for(i=0;i<8;i++){
		delay_us(1);
		i2c_scl= 0;
		//nop();
	  delay_us(5);
	  i2c_scl = 1;
	  delay_us(2);
		data = data << 1;
		if(i2c_sda_i)data = data + 1;
  	delay_us(2);	
	}
	i2c_scl = 0;
	delay_us(2);
	i2c_sda_ddr = 1;
	return data;
}

/*
uchar i2c_readbyte(){
  uchar i,data;
	i2c_scl_ddr = 1;
	i2c_sda_ddr = 1;
	i2c_sda= 1;
	//nop();
	delay_us(4);
  i2c_sda_ddr = 0;
	i2c_scl= 0;						     
	//nop();
	delay_us(4);								  
	for(i=0;i<8;i++){
		i2c_scl= 1;
		//nop();
	  delay_us(4);
		data=(data<<1) | i2c_sda;
		i2c_scl= 0;
		//nop();
  	delay_us(4);	
	}
	return data;
}
*/
/*
void i2c_rec_ack(){
  uchar i = 255;
  i2c_scl_ddr = 1;
  i2c_sda_ddr = 0;
  i2c_scl = 1;
	//nop();
	delay_us(4);
  while(i2c_sda && i > 0)
    i--;
  i2c_scl = 0;
	//nop();
	delay_us(4);
}
*/

void i2c_send_ack(unsigned char a){
  i2c_scl_ddr = 1;
  i2c_sda_ddr = 1;
  if(a == 0)
    i2c_sda = 0;
  else
    i2c_sda = 1;
  delay_us(3);
  i2c_scl = 1;
	//nop();
	delay_us(5);
  i2c_scl = 0;
	//nop();
	delay_us(2);
}

unsigned char i2c_write_component(unsigned char component,unsigned char add,uchar length,unsigned char *data){
  unsigned char i;
  i2c_start();
  i2c_writebyte(component);
    if(ack == 0)return 0;
  //i2c_rec_ack();
  i2c_writebyte(add);
    if(ack == 0)return 0;
  //i2c_rec_ack();
  for(i = 0;i < length;i++) {
    i2c_writebyte(*data);
      if(ack == 0)return 0;
    //i2c_rec_ack();
    data++;
  }
  i2c_end();
  delay_ms(1);
  return 0;
}

unsigned char i2c_read_component(unsigned char component,unsigned char add,unsigned char length,unsigned char *data){
  unsigned char i;
  i2c_start();
  i2c_writebyte(component);
    if(ack == 0)return 0;
  //i2c_rec_ack();
  i2c_writebyte(add);
    if(ack == 0)return 0;
  //i2c_rec_ack();
  
  i2c_start();
  i2c_writebyte(component+1);
    if(ack == 0)return 0;
  //i2c_rec_ack();
  for(i = 0;i < length - 1;i++){    
    *data = i2c_readbyte();
    i2c_send_ack(0);
    data++;
    delay_ms(20);
  }
  *data = i2c_readbyte();
  i2c_send_ack(1);
  i2c_end();
  return 0;
}