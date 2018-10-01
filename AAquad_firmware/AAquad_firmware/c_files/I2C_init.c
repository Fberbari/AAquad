#include "I2C_init.h"


// this function does everything required toset up I2C correctly

void I2C_init(){

	TWBR0 = 150;
}