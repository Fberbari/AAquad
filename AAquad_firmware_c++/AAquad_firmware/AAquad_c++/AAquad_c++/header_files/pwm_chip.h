#include <avr/io.h>
#include "I2C_328pb.h"

#ifndef PWM_CHIP_INIT 
#define PWM_CHIP_INIT 





class pwm_chip
{

public:

	pwm_chip(I2C_328pb i2c, int prescaler);	// takes care of all initialisation including setting the period of pwm

	int pass(uint8_t* motors);



private:

	void decode_motors(I2C_328pb i2c, uint8_t motor, uint8_t* motors, uint8_t* instruction);

	uint8_t instruction[2];	// this array will hold the values that the decodefunction deciphers (high instruction and low instruction)

	
};




#endif