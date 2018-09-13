#include "avr_compiler.h"

#ifndef I2C_328PB 
#define I2C_328PB 

class I2C_328pb{

	friend class pwm_chip;	// this allows these classes to call the methods of the I2C class
	friend class sensors;

	public:
		I2C_328pb(int bit_rate);
		void start();
		void repeat_start();
		void send_slave(int address);
		void send_reg(int reg);
		void send(int data); 
		void get_next_byte();	// this one waits the appropriate ammount of time before allowing the data register to be read again.
		void end_data_read();	// sends the master Nak to tell the slave we are done
		void stop();


};



#endif




