#include "avr_compiler.h"


/*************************************************
*	This class provides a library of methods I built specifically for the Atmega328pb
*	For this project, I've commented out the part of the methods that checks the success of the transfer and turned the into voids instead
*	^ That decision might be reversed once I implement emegency handling functions into the quad.
*	I also implemented all these methods as polling. A watchdog timer should eventually be implemented in future versions and these should eventually be interrupt based
*	Objects may be created and passed to any function or class that rwuires I2C communication
************************************************/

#ifndef I2C_328PB 
#define I2C_328PB 

class I2C_328pb{

	public:
		I2C_328pb(int bit_rate);
		void start();
		void repeat_start();
		void send_slave(int address);
		void send_reg(int reg);	
		void send(int data);	// sends a databyte to the slave
		void get_next_byte();	// waits the appropriate ammount of time before allowing the data register to be read again.
		void end_data_read();	// sends the master Nak to tell the slave we are done
		void stop();


};



#endif




