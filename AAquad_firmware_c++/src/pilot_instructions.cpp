#include "pilot_instructions.h"



void pilot_instructions::set_max_angle(uint8_t max_angle){

	this-> max_angle = max_angle;
}

void pilot_instructions::set_max_yaw_rate(uint8_t max_yaw_rate){

	this-> max_yaw_rate = max_yaw_rate;
}


int pilot_instructions::get_bank_angle() const{

	return bank_angle;
}


int pilot_instructions::get_pitch_angle() const{

	return pitch_angle;
}

int pilot_instructions::get_yaw_rate() const{

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

	// so the math doesent get fucked up, stop interrupts dring this process
	cli();


	// Some of the time the data provided by the interrupts is the low period of the pwm
	// In this case, it needs to be substracted from the total period to get the 
	// I was having some problems comparing 16 bit numbers, so until further notice, i compare only their high byte.

	if ( *((uint8_t*)&(requested_aileron_pos)+1) > 0x9){
			
		requested_aileron_pos = 0xffff - requested_aileron_pos;
	}

	if ( *((uint8_t*)&(requested_elevator_pos)+1) > 0x9 ){
			
		requested_elevator_pos = 0xffff - requested_elevator_pos;
	}

	if ( *((uint8_t*)&(requested_rudder_pos)+1) > 0x9 ){
			
		requested_rudder_pos = 0xffff - requested_rudder_pos;
	}

	if ( *((uint8_t*)&(requested_throttle_pos)+1) > 0x9){
			
		requested_throttle_pos = 0xffff - requested_throttle_pos;
	}






	throttle_power = fabs((requested_throttle_pos - 1015) / 8);	// computes the percentage of the max the pilot wants
	
	
	//Until physical testing, yaw rate controll is disabled
	//yaw_rate = ( requested_rudder_pos / 6553 ) * max_yaw_rate;	// computes the percentage of the max the pilot wants
	
	
	volatile float temp_pitch_angle = fabs(requested_elevator_pos - 1110);	// computes the percentage of the max the pilot wants
	temp_pitch_angle /= 800;
	temp_pitch_angle *= 2 * max_angle;
	temp_pitch_angle -= max_angle;
	pitch_angle = (int) temp_pitch_angle;
	
	

	volatile float temp_bank_angle = fabs(requested_aileron_pos - 1120);	// computes the percentage of the max the pilot wants
	temp_bank_angle /= 800;
	temp_bank_angle *= 2 * max_angle;
	temp_bank_angle -= max_angle;
	bank_angle = (int) temp_bank_angle + 2;
	
	sei();

}