#ifndef _I2C_H
#define _I2C_H

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#define i2c_scl_ddr  DDRJ_DDRJ7
#define i2c_sda_ddr  DDRJ_DDRJ6
#define i2c_scl      PTJ_PTJ7
#define i2c_sda      PTJ_PTJ6
#define i2c_sda_i      PTIJ_PTIJ6

void I2C_Init(void);
void i2c_start(void);
void i2c_end(void);
void i2c_writebyte(unsigned char data);
unsigned char i2c_readbyte(void);
void i2c_rec_ack(void);
void i2c_send_ack(void);
unsigned char i2c_write_component(unsigned char component,unsigned char add,unsigned char length,unsigned char *data);
unsigned char i2c_read_component(unsigned char component,unsigned char add,unsigned char length,unsigned char *data);

#endif