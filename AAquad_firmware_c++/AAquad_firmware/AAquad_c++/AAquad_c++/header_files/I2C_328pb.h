#include <avr\io.h>


#ifndef I2C_328PB 
#define I2C_328PB 

class I2C_328pb{

	friend class pwm_chip;	// this allows these classes to call the methods of the I2C class
	friend class sensors;

	public:
		I2C_328pb(int bit_rate);
		int start();
		int repeat_start();
		int send_slave(int address);
		int send_reg(int reg);
		int send(int data); 
		void stop();


}



#endif




