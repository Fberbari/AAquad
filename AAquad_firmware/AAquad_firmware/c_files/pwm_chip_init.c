#include "pwm_chip_init.h"


int pwm_chip_init(){
	
	
	DDRB |= (1 << 2);	// set OE to 0;

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 

	TWBR0 = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

	TWCR0 = ( (1 << TWEN) | (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when read


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return 0; 
	}


	// send slave address + write bit
	
	TWDR0 = 0x9E;	// slave address + write (10011110)
	
	TWCR0 =( (1 << TWEN) | (1 << TWINT) ); // The TWI process takes controll of the I/O pins

	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	// send address of register to be written

	TWDR0 = 0x0; //  ( Mode register 1)

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR0 & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}

	// send databyte

	TWDR0 = 0x21; // (00100000)	auto increment enable, clock on

	TWCR0 = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR0 & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return 0; 
	}

	// send repeated start

	TWCR0 = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));

	while(! (TWCR0 & (1 << TWINT)) );

	if ( (TWSR0 & 0xf8) != 0x10){ 

		return 0; 
	}

	// send slave address + write bit

	TWDR0 = 0x9E;	// slave address + write (10011110)

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	// send address of register to be written

	TWDR0 = 0xfe; //  ( prescale register)

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR0 & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}


	// send databyte

	TWDR0 = 122; // 50Hz

	TWCR0 = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR0 & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return 0; 
	}

	// stop

	TWCR0 |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 

	return 1;




}

