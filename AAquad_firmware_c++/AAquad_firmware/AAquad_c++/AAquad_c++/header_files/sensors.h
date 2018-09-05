#include "avr_compiler.h"
#include <math.h>
#include "I2C_328pb.h"

#ifndef SENSORS
#define SENSORS


#define GYRO_SENSITIVITY 0.00875	// unit is mdps/digit
#define TIMER_BIT_RATE 1000000	// bits per second
#define ACC_SENSITIVITY 0.000123	// ( 8md/digit is what is is supposed to be. Tests revealed 1g corresponds to 1130 or so which is consistant with the +- 4g's requested)

class sensors{

	public:

		sensors(I2C_328pb i2c);
		void read_acc(I2C_328pb i2c);
		void read_gyro(I2C_328pb i2c);
		void compute_position();
		
		float get_pitch() const;
		float get_roll() const;
		float get_yaw_rate() const;


	private:

		float get_time(); // gives us time since last scan and updates the variable

		int16_t acc_x_data;
		int16_t acc_y_data;	//raw data from acc
		int16_t acc_z_data;

		int16_t gyro_x_data;
		int16_t gyro_y_data;// raw data from gyro
		int16_t gyro_z_data;


		float pitch;
		float roll;	// holds the angles of the CURRENT position
		float yaw_rate;//holds the speed of the yawing motion in deg/s

		uint16_t time_of_previous_scan; // timer is updated and read every time. 


};


#endif
