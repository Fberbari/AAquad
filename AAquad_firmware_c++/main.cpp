/*
 * Firmware Developer : Anthony Berbari
 */ 

#include "avr_compiler.h"



#include "pwm_in_vars.h"	// global external variables



#include "init.h"
#include "I2C_328pb.h"
#include "pilot_instructions.h"
#include "pwm_chip.h"
#include "sensors.h"
#include "bp_PID.h"
#include "combine_data.h"


	
	// These global vars hold raw pilot data. 
	// Requested... holds the time between a rise and fall (or fall and rise). unit used is arbitrary, so long as all is consistent
	// these variables are declared in pwm_in_vars.h and that file is include in main.cpp and in the pilot_instructions.h class
	volatile uint16_t requested_aileron_pos = 0;
	volatile uint16_t requested_elevator_pos = 0;
	volatile uint16_t requested_rudder_pos = 0;
	volatile uint16_t requested_throttle_pos = 0;

	// these store the time stamp of the previous time the associated ISR triggered
	// substracting the time stamp of the previous edge from that of the current edge allows the algorythim to obtain a measure of the pwm's duty cycle.
	// that measure is stored in the above requested.... variables
	volatile uint16_t temp_timer_aileron = 0;
	volatile uint16_t temp_timer_elevator = 0; 
	volatile uint16_t temp_timer_rudder = 0;
	volatile uint16_t temp_timer_throttle = 0; 

	// these are flags that are set true right after the ISR's gather new pilot data
	// they ensure the algorythim that processes it is infact dealing with fresh data
	volatile bool new_aileron_data_available = false;
	volatile bool new_elevator_data_available = false;
	volatile bool new_rudder_data_available = false;
	volatile bool new_throttle_data_available = false;


	// pid objects declaired here to be visible to the ISR
	 pid bank_pid;
	 pid pitch_pid;

	// these global vars are used to exchange data between the ISR that contains the pid algorythim and the main while(1) loop
	volatile float bank_number = 0;
	volatile float pitch_number = 0;
	volatile float throttle_percentage = 0;
	volatile bool  pid_called = false;
	
	


int main(void){
	
	// points to the array that stores the percentage of full each motor should run at
	volatile int* volatile motor_percentages;
	
	
	initialize::timers();
	initialize::interrupts();
	
	// gathers and processes the pilot's wishes
	pilot_instructions pilot;
	pilot.set_max_angle(30);
	pilot.set_max_yaw_rate(45);
	
	//handles all sensor data gathering and processing
	sensors sense;
	
	// initialisation of the chip responsible for talking directly to the esc's
	pwm_chip pwm(121);


	// processes current position and requested position into a number proportional to motor strength
	// This is only for the bank (tilted left or right) angle
	bank_pid.setWeights(1,1,0.5);
	bank_pid.setOutputLimits(-50, 50);

	//Same as on top but for the pitch (tilted forward or back) angle
	pitch_pid.setWeights(1,1,0.5);
	pitch_pid.setOutputLimits(-50, 50);
	


	

	
	// Initialisation finished. Interupts enabled, ready to begin
	sei();
	

	
	while(1){
		
		// sensor data gathered
		sense.read_acc();
		sense.read_gyro();
			
		// all sensor data received and processed
		sense.compute_position();
	
		// all pilot data received and processed
		pilot.compute();	
	
		// pilot commands and sensor data passed to PID object
		pitch_pid.setPoints(pilot.get_pitch_angle(), sense.get_pitch());		
		bank_pid.setPoints(pilot.get_bank_angle(), sense.get_roll());

		//computiations are done
		// the contents of bank_number, pitch_number and throttle power are representative of the pitch and bank required to achieve pilot's wishes
		// these vars are ready to be used by the combine_data method, once the ISR calls it.
		throttle_percentage = pilot.get_throttle_power();
			
		// data_available is only true after a call of the pid loop from the interrupt
		// it is then immediately set to false in wait of the next interrupt	
		if (pid_called){
			
			// results of pid are combined and translated into percentages
			// motor points to an array that contains the percentages that each motor should be run at.
			motor_percentages = combine(bank_number, pitch_number, throttle_percentage);
			
			// data encoded into PWM chip language and sent to the esc's
			pwm.pass( (int*) motor_percentages);	
			pid_called = false;
		}

	}



return 0;

}


ISR(TIMER0_COMPA_vect){

	// PID output recomputed every 0.1s	

		// get the numbers proportional to how strongly we must attempt to bank and pitch to achieve th pilot's wishes 
		bank_number = bank_pid.refresh();
		pitch_number = pitch_pid.refresh();

		// set the flag 
		pid_called = true;
}



// comments for the 4 ISR's to come are the same, so they are only included on this one
// All the following ISR's trigger on any edge
ISR(INT0_vect){
	
		// store the timestamp of the time the edge was detected
		uint16_t temp = TCNT1;
		
		// obviously, the previous edge must have happened before the current edge
		// but because the 16 bit timer may have overflowed and started the count back from 0, we need to check for this
		if ( temp < temp_timer_aileron){

			// the math inside the if condition accounts for a timer overflow
			// the comparison ensures that we only take in new data if that data is reasonable, so  as to not corrupt down the stream algorythyms
			// the values 1110 and 2000 are the lower and upper bounds on what the receiver can output
			if ( ((0xffff - temp_timer_aileron + temp) < 2000) && ((0xffff - temp_timer_aileron + temp) > 1110)){
				
				requested_aileron_pos = (0xffff - temp_timer_aileron) + temp ;

				// since the latest requested aileron position has just been updated, set the flag to true
				new_aileron_data_available = true;
			}
		}

		// this else statement catches the case were no timer overflow has occured
		else {
			
			// the comparisons ensure that we only take in new data if that data is reasonable, so  as to not corrupt down the stream algorythyms
			// the values 1110 and 2000 are the lower and upper bounds on what the receiver can output
			if ( ( (temp - temp_timer_aileron) < 2000) && ( (temp - temp_timer_aileron) > 1110) ) {
	
				requested_aileron_pos = temp - temp_timer_aileron;

				// since the latest requested aileron position has just been updated, set the flag to true
				new_aileron_data_available = true;
			
			}
		}
	
		// no matter what, the current timestamp becomes the previous timestamp
		temp_timer_aileron = temp;

}

ISR(INT1_vect){
	
		uint16_t temp = TCNT1;
		

		if ( temp < temp_timer_throttle){


			if ( ((0xffff - temp_timer_throttle + temp) < 2000) && ((0xffff - temp_timer_throttle + temp) > 1015)){
						
				requested_throttle_pos = (0xffff - temp_timer_throttle) + temp ;
				new_throttle_data_available = true;
			}
		}

		else {
	

			if ( ( (temp - temp_timer_throttle) < 2000) && ( (temp - temp_timer_throttle) > 1015) ) {
						
				requested_throttle_pos = temp - temp_timer_throttle;
				new_throttle_data_available = true;
							
			}
			
		}
	
		
		temp_timer_throttle = temp;
		
		
		
}



ISR(PCINT1_vect){

	uint16_t temp = TCNT1;
		

	if ( temp < temp_timer_rudder){	


		if ( ((0xffff - temp_timer_rudder + temp) < 2000) && ((0xffff - temp_timer_rudder + temp) > 1110)){
						
			requested_rudder_pos = (0xffff - temp_timer_rudder) + temp;
			new_rudder_data_available = true;
		}
	}

	else {	


		if ( ( (temp - temp_timer_rudder) < 2000) && ( (temp - temp_timer_rudder) > 1110) ){
						
			requested_rudder_pos = temp - temp_timer_rudder;
			new_rudder_data_available = true;
						
		}
		
	}

	
	temp_timer_rudder = temp;
	
	

}

ISR(PCINT2_vect){

	uint16_t temp = TCNT1;
	

	if ( temp < temp_timer_elevator){


		if ( ((0xffff - temp_timer_elevator + temp) < 2000) && ((0xffff - temp_timer_elevator + temp) > 1110)){
						
			requested_elevator_pos = (0xffff - temp_timer_elevator) + temp;
			new_elevator_data_available = true;
		}
	}

	else {

		if ( ( (temp - temp_timer_elevator) < 2000) && ( (temp - temp_timer_elevator) > 1110) ) {
						
			requested_elevator_pos = temp - temp_timer_elevator;
			new_elevator_data_available = true;
						
		}
		
	}

	
	temp_timer_elevator = temp;
	
	

}

