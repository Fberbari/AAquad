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
#include "PID.h"
/*
#include "PID.h"
*/
#include <util/delay.h>


	volatile uint16_t requested_aileron_pos = 0;
	volatile uint16_t temp_timer_aileron = 0; // holds the time signature of the previous edge in the PWM capture

	volatile uint16_t requested_elevator_pos = 0;
	volatile uint16_t temp_timer_elevator = 0; 

	volatile uint16_t requested_rudder_pos = 0;
	volatile uint16_t temp_timer_rudder = 0;

	volatile uint16_t requested_throttle_pos = 0;
	volatile uint16_t temp_timer_throttle = 0; 


int main(void){

	initialize::timers();
	initialize::interrupts();
	
	
	pilot_instructions pilot;
	pilot.set_max_angle(30);
	pilot.set_max_yaw_rate(45);	// this retrieves and holds the pilot's wishes

	

	I2C_328pb sensor_I2C(0xAA);	// I2C object created just for use in the sensor object
	sensors sense(sensor_I2C);

	PID bank_pid;
	bank_pid.setWeights(1.5,0.05,0.05);
	bank_pid.setOutputLowerLimit(-50);
	bank_pid.setOutputUpperLimit(50);

	PID pitch_pid;
	pitch_pid.setWeights(1.5,0.05,0.5);
	pitch_pid.setOutputLowerLimit(-50);
	pitch_pid.setOutputUpperLimit(50);
	


	I2C_328pb pwm_chip_I2c(0xAA);
	pwm_chip pwm(pwm_chip_I2c, 100);

	sei();
	
	// Initialisation finished. Interupts enabled.
	

	
	while(1){
		
			
		sense.read_acc(sensor_I2C);
		sense.read_gyro(sensor_I2C);	
		sense.compute_position();// all sensor data received and processed by now
		
	
		pilot.compute();	// all pilot data received and processed
		
	
		bank_pid.setDesiredPoint(pilot.get_bank_angle());	// pilot commands passed to PID object
		pitch_pid.setDesiredPoint(pilot.get_pitch_angle());
	
	
		PID::combine_data(bank_pid.refresh(sense.get_roll()), pitch_pid.refresh(sense.get_pitch()), pilot.get_throttle_power());	// all data processed into individual motor percentages
	

		pwm.pass(pwm_chip_I2c, PID::motor);	// data encoded into PWM chip language and sent to the esc's

	}



return 0;

}



ISR(INT0_vect){
	
		uint16_t temp = TCNT1;
		

		if ( temp < temp_timer_aileron){	// timer overflow

			requested_aileron_pos = (0xffff - temp_timer_aileron) + temp ;
		}

		else {	// regular case
	
			requested_aileron_pos = temp - temp_timer_aileron;
			
		}
	
		
		temp_timer_aileron = temp;
		
		
		// here, there is a chance that the value stored in requested aileron is actually (0xffff - actual requested aileron) this needs to be fixed in the while loop, it has been avoided here to kep the ISR short.
	
}

ISR(INT1_vect){
	
		uint16_t temp = TCNT1;
		

		if ( temp < temp_timer_throttle){	// timer overflow

			requested_throttle_pos = (0xffff - temp_timer_throttle) + temp ;
		}

		else {	// regular case
	
			requested_throttle_pos = temp - temp_timer_throttle;
			
		}
	
		
		temp_timer_throttle = temp;
		
		
		// here, there is a chance that the value stored in requested throttle is actually (0xffff - actual requested throttle) this needs to be fixed in the while loop, it has been avoided here to kep the ISR short.
}




ISR(PCINT1_vect){

	uint16_t temp = TCNT1;
		

	if ( temp < temp_timer_rudder){	// timer overflow

		requested_rudder_pos = (0xffff - temp_timer_rudder) + temp ;
	}

	else {	// regular case

		requested_rudder_pos = temp - temp_timer_rudder;
		
	}

	
	temp_timer_rudder = temp;
	
	

}

ISR(PCINT2_vect){

	uint16_t temp = TCNT1;
	

	if ( temp < temp_timer_elevator){	// timer overflow

		requested_elevator_pos = (0xffff - temp_timer_elevator) + temp ;
	}

	else {	// regular case

		requested_elevator_pos = temp - temp_timer_elevator;
		
	}

	
	temp_timer_elevator = temp;
	
	

}

