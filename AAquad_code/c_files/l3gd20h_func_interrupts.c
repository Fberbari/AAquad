
#include "l3gd20h_func.h"


static int read_0_write_1;


int gyro_write(int reg, int data){

	read_0_write_1 = 1;

	//setup

	PRR &= ~(1 <<  7); // ensures a clock is provided to the TWI (instead of power saver) 

	TWCR |= (1 << TWIE); // Interrupts!

	TWBR = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

	TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)



	//  send Start condition


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready

	return 0;
}




int gyro_read(int reg, int* data){

	read_0_write_1 = 0;


	PRR &= ~(1 <<  7); // ensures a clock is provided to the TWI (instead of power saver) 

	TWCR &= ~(1 << TWIE); // no need for interrupts just yet

	TWBR = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

	TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)



	//  send Start condition


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	PORTB |= (1 << 1);


	//while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		return 0; 
	}


	// send slave address + write bit

	TWDR = 0xD6;	// slave address + write (connected to VCC) (11010110)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // comfirms that slave has received address and sent ACK

		return 0; 
	}



	// send (bit that determines wether read once or multiple times (1 for multiple)) + address of register to be read

	TWDR = reg; // ( only one byte to be read)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		return 0; 
	}


	// send repeat start condition // because we are changing from write to read



	TWCR = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));

	while(! (TWCR & (1 << TWINT)) );

	if ( (TWSR & 0xf8) != 0x10){ 

		return 0; 
	}



	// send slave address + read bit

	TWDR = 0xD7;	// slave address + read (connected to Vcc) (00110001)

	TWCR = ((1 << TWEN) | (1 << TWINT)); //  enable and clear the flag  

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x40){ // comfirms that slave has received address and sent ACK

		return 0; 
	}


	// slave sends data


	TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  (also sends NACK) ( FOr ACK, add TWEA);


	while(! (TWCR & (1 << TWINT)) ); // wait for data transmitted


	if ( (TWSR & 0xf8) != 0x58){ // confirms that slave has understood that data has been recived and NACK was sent out

		return 0;
	}

	*data = TWDR;


	// send stop condition

	TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 



	// TWI is finished. Data has been collected



return 1;


}


ISR(TWI_vect){


		if (read_0_write_1 == 1){
	
	
	
	
			static bool 0x28_was_called = false;
	
			switch (TWSR & 0xf8){
	
	
				case 0x08 :
	
	
	
						TWDR = reg; // ( only one byte to be read)
	
						TWCR = ( (1 << TWINT) | (1 << TWEN) );
	
						break;
	
	
				case 0x18 :
	
	
						TWDR = reg;  // ( only one byte to be read)
	
						TWCR = ( (1 << TWINT) | (1 << TWEN) );
	
						break;
	
	
				case 0x28 :
	
						if (0x28_was_called == false){
	
	
							TWDR = data;
	
							TWCR = ((1 << TWINT) | (1 << TWEN));
	
							0x28_was_called = true;
						}
	
						else{
	
							TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) );
	
							0x28_was_called = false;
						}
	
						break;
	
			}
		}



		else{






		}
	
	
	
	
	
}















