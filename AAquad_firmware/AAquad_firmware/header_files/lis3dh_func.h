/* 

Some useful functions fo dealing with lis3dh accelerometer

*/

#include "avr_compiler.h"

#ifndef LIS3DH_FUNC

#define LIS3DH_FUNC	


int acc_write(int reg, int data);

int acc_read(int reg, int* data);

#endif

// currently, these functions poll the CPU, have no prescaler and when atmega runs at 1 MHz, common clock will run at 50 KHz

// When using, do not forget to set up the correct controll register to enable the accelerometer.

// These functions return 1 if all goes fine and 0 if communication breaks down