
extern volatile int kp;
extern volatile int kd;
extern volatile int previous_time;
extern volatile int previous_horitz_position;
extern volatile int previous_vert_position;




void pid_set_vars(int _kp, int _kd, int _ki){

	kp =_kp;
	kd = _kd;

}


int pid_compute_horitz(int current_horitz_position, int desired_horitz_position){


	int proportional;

	int deriv;


	proportional = desired_position - current_horitz_position;


	if ( TCNT1H < previous_horitz_position){	// timer overflow

			deriv = (current_horitz_position - previous_horitz_position) / 0xff- previous_horitz_position + TCNT1H;
		} 

		else{
			
			deriv = (current_horitz_position - previous_horitz_position) / (TCNT1H - previous_time);
		}

	proportional *= kp;
	deriv *= kd;

	return proportional - deriv ;


}


int pid_compute_horitz(int current_vert_position, int desired_vert_position){


	int proportional;

	int deriv;


	proportional = desired_position - current_horitz_position;


	if ( TCNT1H < previous_vert_position){	// timer overflow

			deriv = (current_vert_position - previous_vert_position) / 0xff- previous_vert_position + TCNT1H;
		} 

		else{
			
			deriv = (current_vert_position - previous_vert_position) / (TCNT1H - previous_time);
		}

	proportional *= kp;
	deriv *= kd;

	return proportional - deriv ;

}