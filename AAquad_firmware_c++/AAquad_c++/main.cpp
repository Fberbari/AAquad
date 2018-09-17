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


	
	//These global vars hold raw pilot data. Requested... holds the time between a rise and fall (or fall and rise). temp_timer is just a var used by the ISR's
	// these variables are declared in pwm_in_vars.h and that file is include here and in the pilot_instruction class
	volatile uint16_t requested_aileron_pos = 0;
	volatile uint16_t temp_timer_aileron = 0; 

	volatile uint16_t requested_elevator_pos = 0;
	volatile uint16_t temp_timer_elevator = 0; 

	volatile uint16_t requested_rudder_pos = 0;
	volatile uint16_t temp_timer_rudder = 0;

	volatile uint16_t requested_throttle_pos = 0;
	volatile uint16_t temp_timer_throttle = 0; 


int main(void){

	initialize::timers();
	initialize::interrupts();
	
	// this class will handleall the pilot's wishes
	pilot_instructions pilot;
	pilot.set_max_angle(30);
	pilot.set_max_yaw_rate(45);

	
	// I2C object created just for use in the sensor object
	//handles all sensor data processing
	I2C_328pb sensor_I2C(0x02);	
	sensors sense(sensor_I2C);

	// processes current position and rqueste position into a number proportional to motor strength
	// This is only for the bank (tilted left or right) angle
	PID bank_pid;
	bank_pid.setWeights(2,0.08,0.05);
	bank_pid.setOutputLowerLimit(-50);
	bank_pid.setOutputUpperLimit(50);

	//Same as on top but for the pitch (tilted forward or back) angle
	PID pitch_pid;
	pitch_pid.setWeights(2,0.08,0.05);
	pitch_pid.setOutputLowerLimit(-50);
	pitch_pid.setOutputUpperLimit(50);
	

	// initialisation of the chip responsible for talking directly to the esc's
	I2C_328pb pwm_chip_I2c(0x02);
	pwm_chip pwm(pwm_chip_I2c, 10);
	

	
	// Initialisation finished. Interupts enabled, ready to begin
	sei();
	
	
	

	
	while(1){
		
		// sensor data gathered
		sense.read_acc(sensor_I2C);
		sense.read_gyro(sensor_I2C);
			
		// all sensor data received and processed
		sense.compute_position();
		
	
	
	
		// all pilot data received and processed
		pilot.compute();	
		
	
	
		// pilot commands passed to PID object
		bank_pid.setDesiredPoint(pilot.get_bank_angle());	
		pitch_pid.setDesiredPoint(pilot.get_pitch_angle());
	
		// Combination of pilot and sensor data into individual percentages of full pwer for each motor
		PID::combine_data(bank_pid.refresh(sense.get_roll()), pitch_pid.refresh(sense.get_pitch()), pilot.get_throttle_power());

		// data encoded into PWM chip language and sent to the esc's
		pwm.pass(pwm_chip_I2c, PID::motor);	

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
		// this is true with all interrupts below
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

