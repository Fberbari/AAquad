#include "bp_PID.h"


pid::pid(): error{0}, _integral(0),_derivative(0),_proportional(0),output(0),integral_windup(false) {

}

void pid::setWeights(const float kp, const float ki, const float kd){

	this->Kp = kp;
	this->Ki = ki;
	this->Kd = kd;
}

void pid::setOutputLimits(const float output_lower_limit, const float output_upper_limit){

	this->output_lower_limit = output_lower_limit;
	this->output_upper_limit = output_upper_limit;
}


void pid::setPoints(const float desired_point, const float actual_point){

 		float new_error = desired_point - actual_point;

 		// push all old error data back by one
 		for (int i = 4; i > 0; i-- ){

 			error[i] = error[i-1];
 		}

 		error[0] = new_error;

 }




float pid::refresh() {

	// calculate the new output
	output = Kp*find_proportional() + Ki*integrate() + Kd*differentiate();


	// check wether output is maxed in either direction
	if (output < output_lower_limit){
		
		output = output_lower_limit;
	}
	
	else if (output > output_upper_limit){
		
		output = output_upper_limit;
	}

	
			
	return output;
}


float pid::integrate(){

	// since we only update the integral every 3 cycles, this var tracks how many cycles since the last integral was delivered
	static int cycles_since = 0;

	// computation is done on this var and final changes are pushed to   the _integral variable
	float local_integral;

	check_for_windup();

	if (cycles_since != 3){

		cycles_since ++;

		// return the last computed integral
		return _integral;
	}
	
	// avoids increasing the integral indefinitly
	// set by another method if it sees that the quad is not budging
	if (integral_windup == true){

		_integral /= 1.2;

		return _integral;

	}return

	// apply simpson's rule
	local_integral = (5 * time_between_calls / 6);
	local_integral *= ( error[0] + (4*error[1]) + error[2] );

	// push the added area to the actual integral variable
	_integral += local_integral;


	cycles_since = 0;

	return _integral;


}

float pid::differentiate(){

	_derivative = 3*error[0] - 4*error[1] + error[2];
	_derivative /= 2*time_between_calls;

	return _derivative; 
}

float pid::find_proportional(){

	_proportional = 0.6*error[0] + 0.4*error[1] + 0.2*error[2] - 0.2*error[4];

	return _proportional; 
}



void pid::check_for_windup(){

	if ( (_integral*Ki) > output_upper_limit || (_integral*Ki) < output_lower_limit){

		integral_windup = true;

	}

	else{

		integral_windup = false;
	}


}