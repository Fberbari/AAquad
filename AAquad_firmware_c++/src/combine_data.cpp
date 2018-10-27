#include "combine_data.h"


int* combine(float bank_number, float pitch_number, float throttle_percentage){

	// the top one is temporary for processing floats whilethe bottom stores the ints
	// bottom one is the one that returns
	float motor[4];
	static int ret_motor[4];

	//inex of motor heavily depends on how the accelerometer and gyro are pointed and how the motors are plugged in.
	motor[0] = (bank_number + pitch_number)  / 2 ;
	motor[1] = (bank_number - pitch_number)  / 2 ;
	motor[2] = (-bank_number + pitch_number) / 2 ;
	motor[3] = (-bank_number - pitch_number) / 2 ;
	
	
	// numbers were in the range [-50, 50], this effectively turns them into a percentage
	// that percentage is the percentage of full power that each motor should turn at.
	for (int i = 0; i < 4; i++){
		
		motor[i] += 50;
	}

	
	// now, that themotors are working correctly relative to each other, multiply by throttle percentage to get the absolute values.
	for (int i = 0; i < 4; i++){
		
		motor[i] *= (throttle_percentage / 80);
	}

	
	// these floats will be cast to integers, so round before hand
	for (int i = 0; i < 4; i++){
		
		ret_motor[i] = round(motor[i]);
	}
	
	
	
	return  ret_motor;

}

