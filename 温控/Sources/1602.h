#ifndef _1602_H
#define _1602_H

#define E   PORTK_PK2
#define RW  PORTK_PK1
#define RS  PORTK_PK0

void init_1602();
void write_com_1602(uchar);
void write_dat_1602(uchar);
void write_ch_1602(uchar,uchar);
void write_str_1602(uchar*,uchar,uchar);
void write_num_1602(uchar,uchar);


#endif