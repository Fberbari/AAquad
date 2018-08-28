#include "PID.h"



    PID::PID(){

        this-> last_error = 0f;
        this-> last_output = 0f;
        this-> integral = 0f;
        this-> error_threshold = 0f;
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
	void PID::setErrorThreshold(const float &error_threshold) {
		this->error_threshold = error_threshold;
	}

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

		this->update_time();	// get the time for integral and derivative purposes

		
		last_error = error;
		
		error = set_point - feedback_input;	

		last_output = output;	


		// the derivative is computed in this line
		// the integral, however, was computed one cycle ago
		output = Kp*error + Ki*integral + Kd* (error-last_error)/time_elapsed;	// calculate the new output
		
		


		if ( (output > output_upper_limit) || (output < output_lower_limit)) {

			
			if (abs(Ki*integral) > abs(error)) {	// integral wind up, do not update the integral further

				
				if (output > output_upper_limit){

					return output_upper_limit;	
				}


				else if (output < output_lower_limit){

					return output_lower_limit;
				}
				
			}
		}


		// otherwise, update the integral
		integral += (error+last_error) * time_elapsed /2f;
			
			
		return output;
	}




	void PID::update_time(){

		if (TCNT1 < this-> previous_clock){

			time_elapsed = (float)(0xffff - previous_clock + TCNT1) / 1000f; 

		}

		else{

			time_elapsed = (float)(TCNT1 - previous_clock) / 1000f ; 

		}

		this-> previous_clock = TCNT1;

	}

