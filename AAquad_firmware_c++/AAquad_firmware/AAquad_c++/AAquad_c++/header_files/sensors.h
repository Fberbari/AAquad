#include <avr\io.h>

#ifndef SENSORS
#define SENSORS



class sensors{

	public:

		sensors(I2C_328pb i2c);
		int read_acc(I2C_328pb i2c, char axis);
		int read_gyro(I2C_328pb i2c, char axis);


	private:

		uint16_t* data;



}


#endif
