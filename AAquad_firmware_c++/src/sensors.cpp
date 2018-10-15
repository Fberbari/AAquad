#include "sensors.h"



sensors::sensors() : I2C_328pb(0x10){

	start(); 
	send_slave(0x32);	// acc slave + w
	send_reg(0xA0);	// Control register 1 + autoincrement
	send(0x67);	// all axis enable data refresh rate is 200Hz
	send(0x0);	// CR2 default values
	send(0x0);	// CR3 default values
	send(0x10);	// CR4  +- 4g and 8mg/digit, The value 32767 corresponds to 4 g's
	stop();
	

////////////////////////////// accelerometer init above and gyro init below

	start();
	send_slave(0xD6); // gyro slave + w
	send_reg(0x20);	// Control register 1
	send(0x0F);	// all axis enable data refresh rate is 100Hz (a lot of other params also affected)
	stop();

			// default sensitivity is 8.75 mdps/digit
			
			
	pitch = 0.f;
	roll = 0.f;
	yaw_rate = 0.f;

	time_of_previous_scan = TCNT1;

}



void sensors::read_acc(){

	start(); 
	send_slave(0x32);	// acc slave + w
	send(0xA8);	// X_low +auto increment
	repeat_start();
	send_slave(0x33);	// acc slave + r

	get_next_byte();
	acc_x_data = TWDR0;	// read low byte
	get_next_byte();
	acc_x_data |= (TWDR0 << 8);	// read high byte
	get_next_byte();


	acc_y_data = TWDR0;	// read low byte
	get_next_byte();
	acc_y_data |= (TWDR0 << 8);	// read high byte
	get_next_byte();


	acc_z_data = TWDR0;	// read low byte
	get_next_byte();
	acc_z_data |= (TWDR0 << 8);	// read high byte


	end_data_read();
	stop();
}

void sensors::read_gyro(){

	start(); 
	send_slave(0xD6);	// gyro slave + w
	send(0xA8);	// X_low +auto increment
	repeat_start();
	send_slave(0xD7);	// gyro slave + r

	get_next_byte();
	gyro_x_data = TWDR0;	// read low byte
	get_next_byte();
	gyro_x_data |= (TWDR0 << 8);	// read high byte
	get_next_byte();


	gyro_y_data = TWDR0;	// read low byte
	get_next_byte();
	gyro_y_data |= (TWDR0 << 8);	// read high byte
	get_next_byte();

  
	gyro_z_data = TWDR0;	// read low byte
	get_next_byte();
	gyro_z_data |= (TWDR0 << 8);	// read high byte

	end_data_read();
	stop();
}




void sensors::compute_position(){

	volatile float R;	// what the accelerometer thinks the total acceleration is
	volatile float trust_factor; // to what degree do I trust the accelerometer's readings
	const float time_between_measurements = get_time();
 
	
	// first, the gyro's data is used to get arelative position to the previous position
	pitch += (gyro_y_data * GYRO_SENSITIVITY * time_between_measurements);
	roll += (gyro_x_data * GYRO_SENSITIVITY * time_between_measurements);
	yaw_rate = (gyro_z_data * GYRO_SENSITIVITY );


	// Here, the total resulting acceleration is computed from the acc data
	R = sqrt( square( acc_x_data ) + square( acc_y_data ) + square( acc_z_data ) );
	R *= ACC_SENSITIVITY;
	
	
	// If the acc says that the resulting acceleration is more than 50% away from 1g, I have deemed that any reasults it gives are too biased to be of any use
	if ( R > 1.5 || R < 0.5 ){
		return;	
	}

	// accounts for +- sign 0 is the level value
	// these functions return radians
	float acc_pitch_angle = asin(  acc_y_data / (R*INVERSE_ACC_SENSITIVITY) );	
	float acc_roll_angle = asin( acc_x_data / (R*INVERSE_ACC_SENSITIVITY) );
	
	
	
	// conversion to degrees instead of radians
	acc_pitch_angle *= DEGREE_CONVERSION_FACTOR;
	acc_roll_angle *= DEGREE_CONVERSION_FACTOR;


	// The closer theaccelerometer is to 1g, the more influence it has in determing the angles at play
	trust_factor = ( 1 - fabs(1 - R) ) / 10;

	// readjustement of angle with input from accelerometer
	pitch = (pitch * (1 - trust_factor) + (acc_pitch_angle * trust_factor) );
	roll = (roll * (1 - trust_factor) + (acc_roll_angle * trust_factor) );

	
	return;


}


float sensors:: get_time(){

	uint16_t time = TCNT1;
	
	volatile float result;
	

	if (TCNT1 < time_of_previous_scan){

		result = 0xffff - time_of_previous_scan + time ;

	}

	else{

		result = time - time_of_previous_scan;
	}


	result /= (float)TIMER_BIT_RATE;

	time_of_previous_scan = time;

	return result;


}

float sensors::get_pitch() const{
	
	return pitch;
}

float sensors::get_roll() const{
	
	return roll;
}

float sensors::get_yaw_rate() const{
	
	return yaw_rate;
}