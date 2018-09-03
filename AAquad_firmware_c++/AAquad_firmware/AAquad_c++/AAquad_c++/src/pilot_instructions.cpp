#include "pilot_instructions.h"



void pilot_instructions::set_max_angle(uint8_t max_angle){

	this-> max_angle = max_angle;
}

void pilot_instructions::set_max_yaw_rate(uint8_t max_yaw_rate){

	this-> max_yaw_rate = max_yaw_rate;
}


float pilot_instructions::get_bank_angle() const{

	return bank_angle;
}


float pilot_instructions::get_pitch_angle() const{

	return pitch_angle;
}

float pilot_instructions::get_yaw_rate() const{

	return yaw_rate;
}


uint8_t pilot_instructions::get_throttle_power() const{

	return throttle_power;
}



void pilot_instructions::compute(void){


	extern volatile uint16_t requested_aileron_pos;
	extern volatile uint16_t requested_elevator_pos;
	extern volatile uint16_t requested_rudder_pos;
	extern volatile uint16_t requested_throttle_pos;


	// 50% of the time the value read is actually the valley of the pwm, that is corrected here 

	if (requested_aileron_pos > 0x7000 ){
			
		requested_aileron_pos = 0xffff - requested_aileron_pos;
	}

	if (requested_elevator_pos > 0x7000 ){
			
		requested_elevator_pos = 0xffff - requested_elevator_pos;
	}

	if (requested_rudder_pos > 0x7000 ){
			
		requested_rudder_pos = 0xffff - requested_rudder_pos;
	}

	if (requested_throttle_pos > 0x7000 ){
			
		requested_throttle_pos = 0xffff - requested_throttle_pos;
	}




	//bank_angle = ( requested_aileron_pos / 6553 ) * max_angle;	// computes the percentage of the max the pilot wants
	//pitch_angle = (requested_elevator_pos / 6553 ) * max_angle;	// computes the percentage of the max the pilot wants
	//yaw_rate = ( requested_rudder_pos / 6553 ) * max_yaw_rate;	// computes the percentage of the max the pilot wants
	throttle_power = ((requested_throttle_pos - 1122) / 8) ;	// computes the percentage of the max the pilot wants


}