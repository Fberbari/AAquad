#include <avr\io.h>
#include <math.h>

#ifndef SENSORS
#define SENSORS



class sensors{

	public:

		sensors(I2C_328pb i2c);
		int read_acc(I2C_328pb i2c);
		int read_gyro(I2C_328pb i2c);
		void compute_position();


	private:

		uint16_t acc_x_data;
		uint16_t acc_y_data;	//raw data from acc
		uint16_t acc_z_data;

		uint16_t gyro_x_data;
		uint16_t gyro_y_data;	// raw data from gyro
		uint16_t gyro_z_data;

		uint16_t acc_x_angle;
		uint16_t acc_y_angle;	// processed angle without considering gyro
		uint16_t acc_z_angle;


		uint16_t combined_x_angle;
		uint16_t combined_y_angle;	// final current processed angle
		uint16_t combined_z_angle;

		
		uint16_t previous_x_angle;
		uint16_t previous_y_angle;	// for use in computing the new processed angle
		uint16_t previous_z_angle;

		uint16_t time_between;

}


#endif
