

// sequence for I2C

//setup

PRR &= ~(1 << TWI0); // ensures a clock is provided to the TWI (instead of power saver) 

TWCR &= ~(1 << TWIE); // no need for interrupts just yet

TWBR = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)


//  send Start condition


TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

TWCR |= (1 << TWSTA ); // writes the start condition on the line

TWCR |= (1 << TWINT); // Hardware will clear this bit when ready

While(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

if ( (TWSR & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

	Error(); 
}



// send slave address + write bit

TWDR = 00110000;	// slave address + write (connected to ground)

TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  

While(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

if ( (TWSR & 0xf8) != 0x18){ // comfirms that slave has received address and sent ACK

	Error(); 
}

// send (bit that determines wether read once or multiple times (1 for multiple)) + address of register to be read

TWDR = 0x29; // register OUT_Y_L in this case ( only one byte to be read)

TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  

While(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

if ( (TWSR & 0xf8) != 0x28){ // comfirms that slave has received address of register and sent ACK

	Error(); 
}


// send repeat start condition // because we are changing from write to read



TWCR |= (1 << TWEN); 

TWCR |= (1 << TWSTA ); 			// same as start condition, but different info in TWSR

TWCR |= (1 << TWINT); 

While(! (TWCR & (1 << TWINT)) );

if ( (TWSR & 0xf8) != 0x10){ 

	Error(); 
}


// send slave address + read bit

TWDR = 00110001;	// slave address + read (connected to ground)

TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  

While(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

if ( (TWSR & 0xf8) != 0x18){ // comfirms that slave has received address and sent ACK

	Error(); 
}

// slave sends data


int read_data = TWDR;

TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  (also sends NACK) ( FOr ACK, add TWEA);

While(! (TWCR & (1 << TWINT)) ); // wait for data transmitted

if ( (TWSR & 0xf8) != 0x58){ // confirms that slave has understood that data has been recived and NACK was sent out

	Error(); 
}


// send stop condition

TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 









