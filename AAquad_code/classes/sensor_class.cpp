#include "avr_compiler.h"

#include "sensor_class.h"

using namespace std;



sensor::sensor(){


	PRR &= ~(1 <<  7); // ensures a clock is provided to the TWI (instead of power saver) 

	TWCR &= ~(1 << TWIE); // no need for interrupts just yet

	TWBR = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

	TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)


}

sensor::sensor(int bit_rate){


	PRR &= ~(1 <<  7); // ensures a clock is provided to the TWI (instead of power saver) 

	TWCR &= ~(1 << TWIE); // no need for interrupts just yet

	TWBR = bit_rate;

	TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)


}

void sensor::set_slave(int address){

	slave_address = address;	// slave address + write 

}

void sensor::set_write_reg(int reg){

	write_reg = reg;
}

void sensor::set_read_reg(int reg){

	read_reg = reg;
}

bool sensor::write(int data_in){


	//  send Start condition


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready



	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x08){ // confirms that status is infact start condition has gone through

		return false; 
	}



	// send slave address + write bit

	TWDR = slave_address;

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // confirms that slave has received address and sent ACK

		return false; 
	}



	// send (bit that determines wether read once or multiple times (1 for multiple)) + address of register to be read

	TWDR = write_reg;  // ( only one byte to be written)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return false; 
	}



	// send databyte

	TWDR = data_in;

	TWCR = ((1 << TWINT) | (1 << TWEN));
	
	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( ((TWSR & 0xf8) != 0x28) ){ // confirms that slave has received address of register and sent ACK

		return false; 
	}

	// stop

	TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 

	return true;


}





bool sensor::read(int* data_out){


	//  send Start condition


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready



	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return false; 
	}


	// send slave address + write bit

	TWDR = slave_address;	// slave address + write (connected to VCC) (11010110)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // comfirms that slave has received address and sent ACK

		return false; 
	}



	// send (bit that determines wether read once or multiple times (1 for multiple)) + address of register to be read

	TWDR = read_reg; // ( only one byte to be read)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return false; 
	}


	// send repeat start condition // because we are changing from write to read



	TWCR = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));

	while(! (TWCR & (1 << TWINT)) );

	if ( (TWSR & 0xf8) != 0x10){ 

		return false; 
	}



	// send slave address + read bit

	TWDR = (slave_address | 1);	// slave address + read (connected to Vcc) (00110001)

	TWCR = ((1 << TWEN) | (1 << TWINT)); //  enable and clear the flag  

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x40){ // comfirms that slave has received address and sent ACK

		return false; 
	}


	// slave sends data


	TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  (also sends NACK) ( FOr ACK, add TWEA);


	while(! (TWCR & (1 << TWINT)) ); // wait for data transmitted


	if ( (TWSR & 0xf8) != 0x58){ // confirms that slave has understood that data has been recived and NACK was sent out

		return false;
	}

	*data = TWDR;


	// send stop condition

	TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 



	// TWI is finished. Data has been collected



return true;

}









