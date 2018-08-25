#include "I2C_328pb.h"


I2C_328pb::I2C_328pb(int bit_rate){

	// takes care of any and all initialsation

	TWBR0 = bit_rate;

}



int I2C_328pb::start(){

	TWCR0 = ( (1 << TWEN) | (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return 0; 
	}

	return 1;

}


int I2C_328pb::repeat_start(){

	TWCR0 = ( (1 << TWEN) | (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x10){ // comfirms reapeated start

		return 0; 
	}

	return 1;

}


int I2C_328pb::send_slave(int address){

	// send slave address + write bit

	TWDR0 = address;

	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR0 & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return 0;
	}

	return 1;


}

int I2C_328pb::send_reg(int reg){

	// send  address of register to be written

	TWDR0 = reg; 

  	TWCR0 = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR0 & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return 0; 
	}

	return 1;


}


int I2C_328pb::send(int data){

	TWDR0 = data;

	TWCR0 = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR0 & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR0 & 0xf8) != 0x28) ){ // comfirms that slave has accepted data and sent ACK

		return 0; 
	}

	return 1;


}

void I2C_328pb::stop(){


	TWCR0 |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 

	return;
}