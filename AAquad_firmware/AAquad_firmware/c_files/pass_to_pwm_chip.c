#include "../header_files/pass_to_pwm_chip.h"

void decode_motors(uint8_t motor, uint8_t* motors, uint8_t* instruction);



int pass_to_pwm_chip(uint8_t* motors){

// this function will communicate over I2C to the pwmchip for final controll of the motors


	uint8_t instruction[2];	// this array will hold the values that the decodefunction deciphers
	
	
	
	decode_motors(0, motors, instruction);

	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x06) ){	//LED0_ON_L

		return 0;
	}

	if( (send(0) & send(0) & send(instruction[0]) & send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();
	






	decode_motors(1, motors, instruction);
	
	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x16) ){	//LED4_ON_L

		return 0;
	}


	if( (send(0) & send(0) & send(instruction[0]) & send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();


	


	decode_motors(2, motors, instruction);

	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x2E) ){	//LED15_ON_L

		return 0;
	}


	if( (send(0) & send(0) & send(instruction[0]) & send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();


	




	decode_motors(3, motors, instruction);
	
	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x42) ){	//LED15_ON_L

		return 0;
	}


	if( (send(0) & send(0) & send(instruction[0]) & send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();





	return 1;


}







void decode_motors(uint8_t motor, uint8_t* motors, uint8_t*instruction){
	
	// input of 100 corresponds to an output of 410
	
	uint16_t temp = motors[motor] * 2;	// the actualslope of this curve is 2.05
	
	temp += 205;	// 205 is the value corresponding to 0 for the esc
	
	instruction[0] = ( temp & 0xff );	// conserves only the low byte
	
	instruction[1] = (temp >> 8);	// conserves only the high half-byte
	
	return;
	
}