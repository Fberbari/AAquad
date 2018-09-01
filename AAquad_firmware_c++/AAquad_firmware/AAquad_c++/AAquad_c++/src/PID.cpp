#include "PID.h"



PID::PID(){

    last_error = 0;
	current_error = 0;
    last_output = 0;
    integral = 0;
}
        


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

		
	last_error = current_error;
		
	current_error = set_point - feedback_input;	

	last_output = output;	


	// the derivative is computed in this line
	// the integral, however, was computed one cycle ago
	output = Kp*current_error + Ki*integral + Kd* (current_error-last_error)/time_elapsed;	// calculate the new output
		
		


	if ( (output > output_upper_limit) || (output < output_lower_limit)) {

			
		if (fabs(Ki*integral) > fabs(current_error)) {	// integral wind up, do not update the integral further

				
			if (output > output_upper_limit){

				return output_upper_limit;	
			}


			else if (output < output_lower_limit){

				return output_lower_limit;
			}
				
		}
	}


	// otherwise, update the integral
	integral += (current_error + last_error) * time_elapsed /2.f;
			
			
	return output;
}




void PID::update_time(){

	if (TCNT1 < this-> previous_clock){

		time_elapsed = (float)(0xffff - previous_clock + TCNT1) / 1000.f; 

	}

	else{

		time_elapsed = (float)(TCNT1 - previous_clock) / 1000.f ; 

	}

	this-> previous_clock = TCNT1;

}

