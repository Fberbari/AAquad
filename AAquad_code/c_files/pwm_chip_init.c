#include "pwm_chip_init.h"

int pwm_chip_init(){

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	PORTB |= (1 << 1);


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return 0; 
	}


	// send slave address + write bit

	TWDR = 0x9E;	// slave address + write (10011110)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	// send address of register to be written

	TWDR = 0x0; //  ( Mode register 1)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}

	// send databyte

	TWDR = 0x21; // (00100001)	auto increment enable, clock on

	TWCR = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return 0; 
	}

	// send repeated start

	TWCR = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));

	while(! (TWCR & (1 << TWINT)) );

	if ( (TWSR & 0xf8) != 0x10){ 

		return 0; 
	}

	// send slave address + write bit

	TWDR = 0x9E;	// slave address + write (10011110)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	// send address of register to be written

	TWDR = 0xfe; //  ( prescale register)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}


	// send databyte

	TWDR = 122; // 50Hz

	TWCR = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return 0; 
	}

	// stop

	TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 

	return 1;




}

