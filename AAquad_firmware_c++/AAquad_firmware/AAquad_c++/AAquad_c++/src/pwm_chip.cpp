#include "pwm_chip.h"



pwm_chip::pwm_chip(I2C_328pb i2c, int prescaler){
	
	
	DDRB |= (1 << 2);	// set OE to 0;

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 

	//TWBR0 = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ


	if  ( ! i2c.start() ){

		return 0;
	} 

	if ( ! i2c.send_slave(0x9E) ){

		return 0;
	} 

	if ( ! i2c.send_reg(0x0) ){	// mode register 1

		return 0;
	}

	if ( ! i2c.send(0x21) ){	// clock on, auto-increment enable

		return 0;
	}

	if ( ! i2c.repeat_start() ){

		return 0;
	}

	if ( ! i2c.send_slave(0x9E) ){

		return 0;
	}

	if ( ! i2c.send_reg(0xFE) ){	// pre scale register

		return 0;
	}


	if ( ! i2c.send(prescaler) ){

		return 0;
	}

	i2c.stop();

	return 1;

}




int pwm_chip::pass(I2C_328pb i2c, uint8_t* motors){

// this function will communicate over I2C to the pwmchip for final controll of the motors
	
	decode_motors(0, motors, instruction);

	if (! i2c.start() ){

		return 0;
	}

	if (! i2c.send_slave(0x9E) ){

		return 0;
	}

	if (! i2c.send_reg(0x06) ){	//LED0_ON_L

		return 0;
	}

	if( (i2c.send(0) & i2c.send(0) & i2c.send(instruction[0]) & i2c.send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	i2c.stop();




	i2c.decode_motors(1, motors, instruction);
	
	if (! i2c.start() ){

		return 0;
	}

	if (! i2c.send_slave(0x9E) ){

		return 0;
	}

	if (! i2c.send_reg(0x16) ){	//LED4_ON_L

		return 0;
	}


	if( (i2c.send(0) & i2c.send(0) & i2c.send(instruction[0]) & i2c.send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	i2c.stop();


	


	i2c.decode_motors(2, motors, instruction);

	if (! i2c.start() ){

		return 0;
	}

	if (! i2c.send_slave(0x9E) ){

		return 0;
	}

	if (! i2c.send_reg(0x2E) ){	//LED15_ON_L

		return 0;
	}


	if( (i2c.send(0) & i2c.send(0) & i2c.send(instruction[0]) & i2c.send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	i2c.stop();


	




	i2c.decode_motors(3, motors, instruction);
	
	if (! i2c.start() ){

		return 0;
	}

	if (! i2c.send_slave(0x9E) ){

		return 0;
	}

	if (! i2c.send_reg(0x42) ){	//LED15_ON_L

		return 0;
	}


	if( (i2c.send(0) & i2c.send(0) & i2c.send(instruction[0]) & i2c.send(instruction[1]) ) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	i2c.stop();





	return 1;


}