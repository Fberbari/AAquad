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

		int16_t acc_x_data;
		int16_t acc_y_data;	//raw data from acc
		int16_t acc_z_data;

		int16_t gyro_x_data;
		int16_t gyro_y_data;	// raw data from gyro
		int16_t gyro_z_data;

		float acc_x_angle_absolute;
		float acc_y_angle_absolute;	// processed angle without considering gyro
		float acc_z_angle_absolute;	// this is the angle of the i j and k vectors in the coordinate system defined by the new location of the quad with respects to the i j and k defined by the objective coordinate system of the earth.
		
		/* THESE ARE NOT THE SAME */
		
		float gyro_x_angle_relative;
		float gyro_y_angle_relative; // processed angle without considering current acc reading
		float gyro_z_angle_relative; // angle between the projection of the new x or y vector onto the previous xy plane and the previous x or y vector


		int8_t combined_x_angle;
		int8_t combined_y_angle;	// final current processed angle
		uint8_t combined_z_angle;

		
		int8_t previous_x_angle;
		int8_t previous_y_angle;	// for use in computing the new processed angle
		uint8_t previous_z_angle;

		uint16_t time_of_previous_scan;


}


#endif
