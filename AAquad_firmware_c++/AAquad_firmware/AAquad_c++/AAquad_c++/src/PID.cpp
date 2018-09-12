#include "PID.h"



PID::PID(){

    last_error = 0;
	current_error = 0;
    last_output = 0;
    integral = 0;
}
        
// initialisation of this satic array		
int PID::motor[4] = {0};


void PID::setWeights(const float &Kp, const float &Ki, const float &Kd) {
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
}
/*
void PID::setRefreshInterval(const float &refresh_interval) {
	interval = refresh_interval;
}

void PID::setRefreshRate(const float &refresh_rate) {
	interval = 1f/refresh_rate;
}
*/
/*
void PID::setErrorThreshold(const float &error_threshold) {
	this->error_threshold = error_threshold;
}
*/
void PID::setOutputLowerLimit(const float &output_lower_limit) {
	this->output_lower_limit = output_lower_limit;
}

void PID::setOutputUpperLimit(const float &output_upper_limit) {
	this->output_upper_limit = output_upper_limit;
}

void PID::setDesiredPoint(const float &desired_point) {
	set_point = desired_point;
}

float PID::refresh(const float &feedback_input) {

	update_time();	// get the time for integral and derivative purposes


	volatile float crap;

		
	last_error = current_error;
	last_output = output;		
	
	current_error = set_point - feedback_input;	
	
	
	
	// To orevet corruptig he integral with noise spikes, updates willonly be made if they are reasonable
	if (  fabs((current_error + last_error) * time_elapsed /2.f) < 1 ){
		
		integral += (current_error + last_error) * time_elapsed /2.f;
	}
	
	
	// Same with the derivative
	if ( fabs((current_error-last_error)/time_elapsed) < 20 ){
		
		derivative = (current_error-last_error)/time_elapsed;
	}
	
	
	// calculate the new output
	output = Kp*current_error; /* + Ki*integral + Kd* derivative;	*/

	crap = set_point;
	crap = feedback_input;
	crap = last_output;
	crap = output;
	crap = (output-last_output);
	crap /= last_output;
	
/*
	// in an effort to reduce noise, we can eliminate any output that is 5% different or more from the current output
	if (fabs((output-last_output)/last_output) > 0.5){
		
		output = last_output;
	}

*/
	// check wether output is maxed in either direction
	if (output < output_lower_limit){
		
		output = output_lower_limit;
	}
	
	else if (output > output_upper_limit){
		
		output = output_upper_limit;
	}
	
	crap = output;
	crap = integral;
	crap = derivative;
	crap = current_error;
		
			
	return output;
}




void PID::update_time(){

		
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

	time_elapsed = result;
	
	
}



void PID::combine_data(float bank_percentage, float pitch_percentage, float throttle_percentage){

	//heavily depends on how the accelerometer and gyro are pointed and how the motors are plugged in.
	motor[0] = (int) (bank_percentage + pitch_percentage) * throttle_percentage / 200 ;
	motor[1] = (int) (bank_percentage - pitch_percentage) * throttle_percentage / 200 ;
	motor[2] = (int) (-bank_percentage + pitch_percentage) * throttle_percentage / 200;
	motor[3] = (int) (-bank_percentage - pitch_percentage) * throttle_percentage / 200;
	
	
	// numbers were in the range [-50, 50], this effectively turns them into a percentage
	motor[0] += 50;
	motor[1] += 50;
	motor[2] += 50;
	motor[3] += 50;
	
	volatile int crap = motor[0];
	crap = motor[1];
	crap = motor[2];
	crap = motor[3];
	

}



