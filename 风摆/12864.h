#ifndef __12864_H__
#define __12864_H__
   
#define RS PORTK_PK0 //PTS_PTS7 
#define RW PORTK_PK1 //PTJ_PTJ7 
#define EN PORTK_PK2 //PTJ_PTJ6 
#define PSB PORTK_PK3 

void delay_10us(int n);
void init_12864();
void check_busy_12864();
void write_com_12864(uchar);
void write_dat_12864(uchar);
uchar read_dat_12864();
void write_ch_12864(uchar,uchar);
void write_ang_12864(int,uchar);
void write_str_12864(uchar *,uchar);

void clr_GDR_12864(uchar flag);
void draw_dot_12864(uchar x,uchar y);
void draw_curve_12864(double temp);


#endif