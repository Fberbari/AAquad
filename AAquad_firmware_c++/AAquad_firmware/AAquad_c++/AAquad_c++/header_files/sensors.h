#include <avr\io.h>
#include <math.h>

#ifndef SENSORS
#define SENSORS


#define GYRO_SENSITIVITY 0.00875	// unit is mdps/digit


class sensors{

	public:

		sensors(I2C_328pb i2c);
		int read_acc(I2C_328pb i2c);
		int read_gyro(I2C_328pb i2c);
		void compute_position();


	private:

		float get_time(); // gives us time since last scan and updates the variable

		int16_t acc_x_data;
		int16_t acc_y_data;	//raw data from acc
		int16_t acc_z_data;

		int16_t gyro_x_data;
		int16_t gyro_y_data;	// raw data from gyro
		int16_t gyro_z_data;


		float pitch;
		float roll;	// holds the angles of the CURRENT position

		uint16_t time_of_previous_scan; // timer is updated and read every time. 


}


#endif
