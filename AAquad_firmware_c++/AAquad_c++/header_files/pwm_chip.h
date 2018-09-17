#include "avr_compiler.h"
#include "I2C_328pb.h"

#ifndef PWM_CHIP_H 
#define PWM_CHIP_H

/***************************************************************
*	This class takes care of all communication with the chip that outputs the pwm signal to the esc's
*	It just needs to be passed an array of 4 integers that contains the percentage of full power associated to each motor.
***************************************************************/


class pwm_chip{

public:
	
	// takes care of all initialisation including setting the period of pwm
	pwm_chip(I2C_328pb i2c, uint8_t prescaler);

	// sends all data to the esc's
	void pass(I2C_328pb i2c, int* motors);



private:
	
	// converts the percentages received into language that the pwm chip can understand
	void encode_motors(uint8_t motor, int* motors, uint8_t* instruction);	
	
	// this array will hold the values that the decode function deciphers (high instruction and low instruction) and will only be called by the enode_motors method
	uint8_t instruction[2];	

	
};




#endif