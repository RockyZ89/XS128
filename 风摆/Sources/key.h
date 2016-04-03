#ifndef _KEY_H
#define _KEY_H

#define  key_out PTH
#define  key_in  PTIH
#define  key_dir DDRH

extern uchar L,S,B,R;
extern uchar len[2];
extern uchar ang[3];
extern uchar rad[2];
extern uchar SetL,SetA,SetR;
extern uchar ct;




int scan_key();
void judge_key(int key);


#endif