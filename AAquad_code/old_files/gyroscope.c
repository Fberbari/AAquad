
#include "avr_compiler.h"

#define 	F_CPU   1000000UL

#include <util/delay.h>



						// This code should transmit gyro data to some led's.

void Error(void);



int main(){

	int read_data;

	DDRB = ~0;

	DDRD = ~0;

	PORTD = 0;

	DDRC = ~0;

	PORTC = 0;


	_delay_ms(200);

	//setup

	PRR &= ~(1 <<  7); // ensures a clock is provided to the TWI (instead of power saver) 

	TWCR &= ~(1 << TWIE); // no need for interrupts just yet

	TWBR = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ

	TWSR &= ~( (1 << 0) | (1 << 1) );	// no prescaler (clock division)



	while (1){


	//  send Start condition


	TWCR |= (1 << TWEN); // The TWI process takes controll of the I/O pins

	TWCR |= ( (1 << TWSTA ) | (1 << TWINT) ); // writes the start condition on the line  and Hardware will clear this bit when ready


	PORTB |= (1 << 1);


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x08){ // comfirms that status is infact start condition has gone through

		Error(); 
	}



	PORTB |= (1 << 2);


	// send slave address + write bit

	TWDR = 0xD6;	// slave address + write (connected to VCC) (11010110)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );


	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x18){ // comfirms that slave has received address and sent ACK

		Error(); 
	}



	// send (bit that determines wether read once or multiple times (1 for multiple)) + address of register to be read

	TWDR = 0x29; // register OUT_X_H in this case ( only one byte to be read)

	TWCR = ( (1 << TWINT) | (1 << TWEN) );

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go


	if ( ((TWSR & 0xf8) != 0x28) ){ // comfirms that slave has received address of register and sent ACK

		Error(); 
	}



	PORTB |= (1 << 4);


	// send repeat start condition // because we are changing from write to read



	TWCR = ((1 << TWEN) | (1 << TWSTA) | (1 << TWINT));

	while(! (TWCR & (1 << TWINT)) );

	if ( (TWSR & 0xf8) != 0x10){ 

		Error(); 
	}


	PORTB |= (1 << 5);

	// send slave address + read bit

	TWDR = 0x33;	// slave address + read (connected to ground) (00110001)

	TWCR = ((1 << TWEN) | (1 << TWINT)); //  enable and clear the flag  

	while(! (TWCR & (1 << TWINT)) ); // Hardware will write this to 0 when ready to go

	if ( (TWSR & 0xf8) != 0x40){ // comfirms that slave has received address and sent ACK

		Error(); 
	}


	// slave sends data


	TWCR |= (1 << TWEN) | (1 << TWINT); //  enable and clear the flag  (also sends NACK) ( FOr ACK, add TWEA);

	while(! (TWCR & (1 << TWINT)) ); // wait for data transmitted

	if ( (TWSR & 0xf8) != 0x58){ // confirms that slave has understood that data has been recived and NACK was sent out

		Error(); 
	}

	read_data = TWDR;


	// send stop condition

	TWCR |= ( (1 << TWEN) | (1 << TWINT) | (1 << TWSTO) ); 



	// TWI is finished. Data has been collected



	PORTD = read_data;


}


return 0;

}



void Error(void){



		PORTB |= (1 << 0);

		while(1);


	// this function should only be called if uC has lost contact with the gyroscope or accelerometer

	// it should deploy the parashute.



return;


}

