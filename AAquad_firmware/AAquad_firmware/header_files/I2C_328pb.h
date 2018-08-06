#include <avr\io.h>


#ifndef I2C_328PB 
#define I2C_328PB 

int start();
int repeat_start();
int send_slave();
int send_reg(int reg);
int send(int data); 
void stop(); 

#endif