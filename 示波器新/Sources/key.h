#ifndef _KEY_H
#define _KEY_H

#define DDR_key   DDRH
#define PORT_key  PTH

int scan_key();
void buzzer_l();
void buzzer_s();
void judge_key(int key);


#endif