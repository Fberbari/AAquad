#include "sensors.h"



sensors::sensors(I2C_328pb i2c){

	i2c.start(); 
	i2c.send_slave(0x32);	// acc slave + w
	i2c.send_reg(0x20);	// Control register 1
	i2c.send(0x67);	// all axis enable data refresh rate is 200Hz
	stop();
// CTRL reg 4 should be set to properly controll measurement range and sensitivity

////////////////////////////// accelerometer init above and guro init below

	i2c.start();
	i2c.send_slave(0xD6); // gyro slave + w
	i2c.send_reg(0x20);	// Control register 1
	i2c.send(0x15);	// all axis enable data refresh rate is 100Hz (a lot of other params also affected)
	stop();

			// default sensitivity is 8.75 mdps/digit

}



int sensors::read_acc(I2C_328pb i2c){

	i2c.start(); 
	i2c.send_slave(0x32);	// acc slave + w
	i2c.send(0xA8);	// X_low +auto increment
	i2c.repeat_start();
	i2c.send_slave(0x33);	// acc slave + r

	i2c.get_next_byte();
	acc_x_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	acc_x_data |= (TWDR0 << 8);	// read high byte
	i2c.get_next_byte();


	acc_y_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	acc_y_data |= (TWDR0 << 8);	// read high byte
	i2c.get_next_byte();


	acc_z_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	acc_z_data |= (TWDR0 << 8);	// read high byte


	i2c.end_data_read();
	i2c.stop();
}

int sensors::read_gyro(I2C_328pb i2c){

	i2c.start(); 
	i2c.send_slave(0xD6);	// gyro slave + w
	i2c.send(0xA8);	// X_low +auto increment
	i2c.repeat_start();
	i2c.send_slave(0xD7);	// gyro slave + r

	i2c.get_next_byte();
	gyro_x_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	gyro_x_data |= (TWDR0 << 8);	// read high byte
	i2c.get_next_byte();


	gyro_y_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	gyro_y_data |= (TWDR0 << 8);	// read high byte
	i2c.get_next_byte();

  
	gyro_z_data = TWDR0;	// read low byte
	i2c.get_next_byte();
	gyro_z_data |= (TWDR0 << 8);	// read high byte

	i2c.end_data_read();
	i2c.stop();
}




void sensors::compute_position(){

	const int R = sqrt( square(acc_x_data) + square(acc_y_data) + square(acc_z_data) );


	acc_x_angle_absolute = asin( acc_x_data / R);	// accounts for +- sign 0 is the level value
	acc_y_angle_absolute = asin( acc_y_data / R);	// accounts for +- sign 0 is the level value
	acc_z_angle_absolute = acos( acc_z_data / R);	// angle will never be negative. Anything above 90 degrees means the quad is upside down.

	// Till here, the accelerometer has given its best estimate of angle

	gyro_x_angle_relative = (gyro_x_data * GYRO_SENSITIVITY * time_between measurements); //angle between the projection of the new y or z vector onto the previous yz plane and the previous y or z vector
	gyro_y_angle_relative = (gyro_y_data * GYRO_SENSITIVITY * time_between_measurements); //angle between the projection of the new z or x vector onto the previous zx plane and the previous z or x vector
	gyro_z_angle_relative = (gyro_z_data * GYRO_SENSITIVITY * time_between_measurements); //angle between the projection of the new x or y vector onto the previous xy plane and the previous x or y vector

	/* so I could tell you that new x axis has moved (gyro_y_angle_relative) in the y direction and (gyro_z_direction_relative in the z direction) */

}

void sensors::produce_vector(){

	
}