#include "../header_files/pass_to_pwm_chip.h"

int start();
int send_slave();
int send_reg(int reg);
int send(int data);
void stop();


int pass_to_pwm_chip(int* motors){

// this function will communicate over I2C to the pwmchip for final controll of the motors





	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x06) ){	//LED0_ON_L

		return 0;
	}

		if( (send(0x20) & send(0x3) & send(0xB0) & send(0x4)) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();
	



/*



	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x16) ){	//LED4_ON_L

		return 0;
	}


	if( (send(0x20) & send(0x3) & send(0xB0) & send(0x4)) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();



	



	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x26) ){	//LED8_ON_L

		return 0;
	}


	if( (send() & send() & send() & send()) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();



	




	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x36) ){	//LED12_ON_L

		return 0;
	}


	if( (send() & send() & send() & send()) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();


	





	if (! start() ){

		return 0;
	}

	if (! send_slave() ){

		return 0;
	}

	if (! send_reg(0x46) ){	//LED16_ON_L

		return 0;
	}


	if( (send() & send() & send() & send()) == 0){ //ON_L, ON_H, OFF_L, OFF_H

		return 0;
	}

	stop();



*/


	return 1;


}











int start(){

	TWCR0 |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR0 |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	PORTB |= (1 << 1);


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return 0; 
	}

	return 1;

}

int send_slave(){

	// send slave address + write bit

	TWDR0 = 0x9E;	// slave address + write (10011110)

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	return 1;


}

int send_reg(int reg){

	// send  address of register to be written

	TWDR0 = reg; 

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR0 & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}

	return 1;


}




int send(int data){

	TWDR0 = data;

	TWCR0 = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR0 & 0xf8) != 0x28) ){ // comfirms that slave has accepted data and sent ACK

		return 0; 
	}

	return 1;


}

void stop(){


	TWCR0 |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 

	return;
}


