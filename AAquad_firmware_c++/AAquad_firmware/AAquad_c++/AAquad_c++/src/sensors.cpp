#include "sensors.h"



sensors::sensors(I2C_328pb i2c){

	i2c.start(); 
	i2c.send_slave(0x32);	// acc slave + w
	i2c.send_reg(0x20);	// Control register 1
	i2c.send(0x67);	// all axis enable data refresh rate is 200Hz
	stop();
	// CTRL reg 4 should be set to properly controll measurement range and sensitivity
	// default sensitivity is 1000 mg/digit

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

	float R;	// what the accelerometer thinks the total acceleration is
	float trust_factor; // to what degree do I trust the accelerometer's readings
	const float time_between_measurements = get_time();


	pitch += (gyro_y_data * GYRO_SENSITIVITY * time_between_measurements);
	roll += (gyro_x_data * GYRO_SENSITIVITY * time_between measurements);

	yaw_rate = (gyro_z_data / GYRO_SENSITIVITY );




	R = sqrt( square( acc_x_data / ACC_SENSITIVITY ) + square( acc_y_data / ACC_SENSITIVITY ) + square( acc_z_data / ACC_SENSITIVITY ) );


	if ( abs(R) > 1.5 || abs(R) < 0.5 ){

		return;	// there is too much external factor for the accelerometer to be of any use

	}


	acc_pitch_angle = asin( 1000 * acc_y_data / R);	// accounts for +- sign 0 is the level value
	acc_roll_angle = asin( 1000 * acc_x_data / R);	// accounts for +- sign 0 is the level value



	trust_factor = ( 0.6 - abs(1 - R) ) / 10;


	pitch = (pitch * (1 - trust_factor) + (acc_pitch_angle * trust_factor) );
	roll = (roll * (1 - trust_factor) + (acc_roll_angle * trust_factor) );

	return;


}


float sensors:: get_time(){

	uint16_t time = TCNT1;
	float result;
	

	if (TCNT1 < time_of_preious_scan){

		result = 0xffff - time_of_preious_scan + time ;

	}

	else{

		result = time - result;
	}


	result /= division_factor;

	time_of_preious_scan = time;

	return result;


}