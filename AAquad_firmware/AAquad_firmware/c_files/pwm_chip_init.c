#include "pwm_chip_init.h"



int pwm_chip_init(){
	
	
	DDRB |= (1 << 2);	// set OE to 0;

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 

	//TWBR0 = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ


	if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0x9E) ){

		return 0;
	} 

	if ( ! send_reg(0x0) ){	// mode register 1

		return 0;
	}

	if ( ! send(0x21) ){	// clock on, auto-increment enable

		return 0;
	}

	if ( ! repeat_start() ){

		return 0;
	}

	if ( ! send_slave(0x9E) ){

		return 0;
	}

	if ( ! send_reg(0xFE) ){	// pre scale register

		return 0;
	}


	if ( ! send(122) ){

		return 0;
	}

	stop();

	return 1;




}
