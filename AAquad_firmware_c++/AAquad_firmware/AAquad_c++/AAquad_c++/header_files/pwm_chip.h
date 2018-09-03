#include "avr_compiler.h"
#include "I2C_328pb.h"

#ifndef PWM_CHIP_H 
#define PWM_CHIP_H





class pwm_chip
{

public:

	pwm_chip(I2C_328pb i2c, uint8_t prescaler);	// takes care of all initialisation including setting the period of pwm

	void pass(I2C_328pb i2c, int* motors);



private:

	void encode_motors(uint8_t motor, int* motors, uint8_t* instruction);

	uint8_t instruction[2];	// this array will hold the values that the decodefunction deciphers (high instruction and low instruction)

	
};




#endif