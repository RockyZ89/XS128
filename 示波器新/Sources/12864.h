#ifndef __12864_H__
#define __12864_H__


#define PSB PORTK_PK3  // 1²¢ 0´®
#define E   PORTK_PK2
#define RW  PORTK_PK1
#define RS  PORTK_PK0


void init_12864();
void check_busy_12864();
void write_com_12864(uchar);
void write_dat_12864(uchar);
uchar read_dat_12864();
void write_ch_12864(uchar,uchar);
void write_num_12864(uchar,uchar);
void write_str_12864(uchar *,uchar,uchar);
void clr_GDR_12864(uchar flag);
void draw_dot_12864(uchar x,uchar y);



#endif