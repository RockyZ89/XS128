#ifndef __24C02_H__
#define __24C02_H__

#define sda PTJ_PTJ6
#define scl PTJ_PTJ7 

#define sda_out DDRJ_DDRJ6 = 1
#define sda_in  DDRJ_DDRJ6 = 0
#define scl_out DDRJ_DDRJ7 = 1
#define scl_in  DDRJ_DDRJ7 = 0

void init_i2c();
void start_i2c();
void stop_i2c();
void ack_i2c();
void write_byte_24c02(uchar dat,uchar add);
uchar read_byte_24c02(uchar add);


#endif
