/*
 * Firmware Developer : Anthony Berbari
 */ 

#include "avr_compiler.h"


#include "pwm_in_vars.h"	// global external variables




#include "init.h"
#include "I2C_328pb.h"
#include "pilot_instructions.h"
#include "sensors.h"
#include "PID.h"

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

	initialize::interrupts();
	initialize::timers();
	
	I2C_328pb i2c(2000); // 4Khz I2C clock
	
	pilot_instructions pilot;
	pilot.set_max_angle(30);
	pilot.set_max_yaw_rate(45);
	
	
	
	I2C_328pb sensor_I2C(2000);	// object created just for use in the sensor object
	sensors sense(sensor_I2C);
	
	PID bank_pid;
	bank_pid.setWeights(0.5,0.5,0.5);
	bank_pid.setOutputLowerLimit(-30);
	bank_pid.setOutputUpperLimit(30);
	
	PID pitch_pid;
	pitch_pid.setWeights(0.5,0.5,0.5);
	pitch_pid.setOutputLowerLimit(-30);
	pitch_pid.setOutputUpperLimit(30);
	
	


	while(1){
		
		sense.read_acc(sensor_I2C);
		sense.read_gyro(sensor_I2C);	// all sensor data processed
		sense.compute_position();
		
		pilot.compute();	// all pilot data processed
		
		bank_pid.setDesiredPoint(pilot.get_bank_angle());
		pitch_pid.setDesiredPoint(pilot.get_pitch_angle());
		
		
		
		

	}



return 0;

}



ISR(INT1_vect){
	
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

ISR(INT0_vect){
	
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




ISR(PCINT0_vect){

	uint16_t temp = TCNT1;
		

	if ( temp < temp_timer_rudder){	// timer overflow

		requested_rudder_pos = (0xffff - temp_timer_rudder) + temp ;
	}

	else {	// regular case

		requested_rudder_pos = temp - temp_timer_rudder;
		
	}

	
	temp_timer_rudder = temp;
	
	

}

ISR(PCINT1_vect){

	uint16_t temp = TCNT1;
	

	if ( temp < temp_timer_elevator){	// timer overflow

		requested_elevator_pos = (0xffff - temp_timer_elevator) + temp ;
	}

	else {	// regular case

		requested_elevator_pos = temp - temp_timer_elevator;
		
	}

	
	temp_timer_elevator = temp;
	
	

}

