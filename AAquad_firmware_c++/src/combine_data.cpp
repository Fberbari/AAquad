#include "combine_data.h"


int* combine(float bank_number, float pitch_number, float throttle_percentage){


	static int motor[4];

	//inex of motor heavily depends on how the accelerometer and gyro are pointed and how the motors are plugged in.
	motor[0] = (int) round((bank_number + pitch_number) * throttle_percentage / 200) ;
	motor[1] = (int) round((bank_number - pitch_number) * throttle_percentage / 200) ;
	motor[2] = (int) round((-bank_number + pitch_number) * throttle_percentage / 200);
	motor[3] = (int) round((-bank_number - pitch_number) * throttle_percentage / 200);
	
	
	// numbers were in the range [-50, 50], this effectively turns them into a percentage
	// that percentage is the percentage of full power that each motor should turn at.
	motor[0] += 50;
	motor[1] += 50;
	motor[2] += 50;
	motor[3] += 50;
	
	return motor;

}

