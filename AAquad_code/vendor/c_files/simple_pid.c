

#include "../header_files/simple_pid.h"


// this file behaves as a class (or struct) as these variables cannot be accessed outside this file
// currently, no integral support, I do not bielieve this is needed right now.
extern volatile int kp;
extern volatile int kd;
extern volatile int previous_time;
extern volatile int previous_x_position;
extern volatile int previous_y_position;




void pid_set_vars(int _kp, int _kd){

	kp =_kp;
	kd = _kd;	// how important each of the constants should be

}


int pid_compute_x(int current_x_position, int desired_x_position){


	int proportional;

	int deriv;


	proportional = desired_x_position - current_x_position;


	if ( TCNT1H < previous_x_position){	// timer overflow

			deriv = (current_x_position - previous_x_position) / 0xff- previous_x_position + TCNT1H;
		} 

		else{
			
			deriv = (current_x_position - previous_x_position) / (TCNT1H - previous_time);
		}

	proportional *= kp;
	deriv *= kd;

	return proportional - deriv ;


}


int pid_compute_y(int current_y_position, int desired_y_position){


	int proportional;

	int deriv;


	proportional = desired_y_position - current_y_position;


	if ( TCNT1H < previous_y_position){	// timer overflow

			deriv = (current_y_position - previous_y_position) / 0xff- previous_y_position + TCNT1H;
		} 

		else{
			
			deriv = (current_y_position - previous_y_position) / (TCNT1H - previous_time);
		}

	proportional *= kp;
	deriv *= kd;

	return proportional - deriv ;

}