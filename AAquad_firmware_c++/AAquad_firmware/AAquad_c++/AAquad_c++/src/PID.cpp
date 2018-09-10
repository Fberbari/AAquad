#include "PID.h"


PID::PID(){

    last_error = 0;
	current_error = 0;
    last_output = 0;
    integral = 0;
	
}
 
// initialising this static array is required      
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
	static int cnt = 0;
	cnt++;
	
	
	last_error = current_error;// this happens no matter what, every cycle
	current_error = set_point - feedback_input;
	
	
	// derivative must be refreshed every cycle
	derivative = (current_error-last_error)/time_elapsed;
	
	
	//only if the value that will be added to the integral is reasonable will it be added. Otherwise, the integral will be corrupted	
	if(  fabs((current_error + last_error) * time_elapsed /2.f) < 2){	
		
		integral += (current_error + last_error) * time_elapsed /2.f;
	}
	
	
	// output is computed with current proportional and derivative data. integral data, however, is at least 1 cycle behind
	// This output will only be realeased if l
	output = Kp*current_error + Ki*integral - Kd* (current_error-last_error)/time_elapsed;

	
	
	//something went wrong with pilot or sensor data collection, output the last know stable output
	if ( (output > output_upper_limit) || (output < output_lower_limit)) {
		
		return last_output;			
	}
	
	
	// all is normal, update the integral and compute the derivative and the proportiona term

	

	last_output = output;	


	
	crap = set_point;	
	crap = time_elapsed;
	crap = feedback_input;	
	crap = integral;
	crap = derivative;
	crap = (current_error + last_error) * time_elapsed /2.f;
	crap = current_error;
	crap = output;
			
			
	
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



void PID::combine_data(float bank_number, float pitch_number, float throttle_percentage){

	//heavily depends on how the accelerometer and gyro are pointed and how the motors are plugged in.
	
	static uint16_t cnt = 0;
	cnt ++;
	
	volatile float crap;

	motor[0] = (int) (bank_number + pitch_number) * throttle_percentage / 200 ;

	motor[1] = (int) (bank_number - pitch_number) * throttle_percentage / 200 ;

	motor[2] = (int) (-bank_number + pitch_number) * throttle_percentage / 200; // 200 because 100 is used to convert the throttle percentage

	motor[3] = (int) (-bank_number - pitch_number) * throttle_percentage / 200;

	crap = bank_number;
	crap = pitch_number;
	crap = throttle_percentage;
	crap = motor[0];
	crap = motor[1];
	crap = motor[2];
	crap = motor[3];
}

int* PID::get_motor(){ return motor; }
