#include "sensors.h"



sensors::sensors(I2C_328pb i2c){

	i2c.start(); 
	i2c.send_slave(0x32);	// acc slave + w
	i2c.send_reg(0xA0);	// Control register 1 + autoincrement
	i2c.send(0x67);	// all axis enable data refresh rate is 200Hz
	i2c.send(0x0);	// CR2 default values
	i2c.send(0x0);	// CR3 default values
	i2c.send(0x10);	// CR4  +- 4g and 8mg/digit, The value 32767 corresponds to 4 g's
	i2c.stop();
	

////////////////////////////// accelerometer init above and gyro init below

	i2c.start();
	i2c.send_slave(0xD6); // gyro slave + w
	i2c.send_reg(0x20);	// Control register 1
	i2c.send(0x0F);	// all axis enable data refresh rate is 100Hz (a lot of other params also affected)
	i2c.stop();

			// default sensitivity is 8.75 mdps/digit
			
			
	pitch = 0.f;
	roll = 0.f;
	yaw_rate = 0.f;

	time_of_previous_scan = TCNT1;

}



void sensors::read_acc(I2C_328pb i2c){

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

void sensors::read_gyro(I2C_328pb i2c){

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

	volatile float R;	// what the accelerometer thinks the total acceleration is
	volatile float trust_factor; // to what degree do I trust the accelerometer's readings
	const float time_between_measurements = get_time();
 
	
	pitch += (gyro_y_data * GYRO_SENSITIVITY * time_between_measurements);
	roll += (gyro_x_data * GYRO_SENSITIVITY * time_between_measurements);
	yaw_rate = (gyro_z_data * GYRO_SENSITIVITY );



	R = sqrt( square( acc_x_data ) + square( acc_y_data ) + square( acc_z_data ) );
	R *= ACC_SENSITIVITY;
	

	if ( R > 1.5 || R < 0.5 ){

		return;	// there is too much external factor for the accelerometer to be of any use

	}


	float acc_pitch_angle = asin(  acc_y_data / (R*8130) );	// accounts for +- sign 0 is the level value
	float acc_roll_angle = asin( acc_x_data / (R*8130) );	// accounts for +- sign 0 is the level value
	
	acc_pitch_angle *= 57.3;
	acc_roll_angle *= 57.3;	// conversion to degrees instead of radians.



	trust_factor = ( 1 - fabs(1 - R) ) / 10;


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
	
		volatile float crap_pitch;
	
	crap_pitch = roll;
	
	return pitch;
}

float sensors::get_roll() const{
	
	volatile float crap_roll;
	
	crap_roll = roll;
	
	return roll;
}

float sensors::get_yaw_rate() const{
	
	return yaw_rate;
}