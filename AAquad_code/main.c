
#include "header_files/calculate_position.h"

#include "header_files/encode_to_motors.h"

#include "header_files/init_extern_ints.h"

#include "header_files/init_free_timer.h"

#include "header_files/init_mux_timer.h"

#include "header_files/l3gd20h_func.h"

#include "header_files/lis3dh_func.h"


#include "header_files/pass_to_pwm_chip.h"

#include "header_files/simple_pid.h"


#include <avr/interrupt.h>

#include <stdint.h>

#define 	F_CPU   8000000UL

#include "header_files/macros/*"


	// this group of globals will hold all the info that the pilot may request.
	volatile uint8_t requested_throttle_pos = 0; 
	volatile uint8_t requested_aileron_pos = 0; 
	volatile uint8_t requested_elevator_pos = 0; 
	volatile uint8_t requested_rudder_pos = 0; 

	// this group of globals are available to store timer data on the rising edge, so as to compute dutycycle
	extern volatile uint8_t temp_timer_throttle = 0; 
	extern volatile uint8_t temp_timer_aileron = 0; 
	extern volatile uint8_t temp_timer_elevator = 0; 
	extern volatile uint8_t temp_timer_rudder = 0; 



int main(){

	uint8_t success; 
	uint8_t x_pos;
	uint8_t y_pos;
	uint8_t z_pos;
	uint8_t required_x_motion;
	uint8_t required_y_motion;
	uint8_t motors[4];

	uint8_t* gyro_out_data_x;
	uint8_t* gyro_out_data_y;
	uint8_t* gyro_out_data_z;

	uint8_t* acc_out_data_x;
	uint8_t* acc_out_data_y;
	uint8_t* acc_out_data_z;

	sei();	// enable interrupts

	init_mux_timer();

	init_free_timer();

	init_extern_ints();

	pid_set_vars(kp, kd);


	success = gyro_write(GYRO_CTRL1, 0x8f);	// all axis enabled, data refresh at 400 Hz

	if (! success){

		error();
	}

	success = acc_write(ACC_CTRL_REG1, 0x77);	// all axis enabled, data refresh at 400 Hz

	if (! success){

		error();
	}






	while(1){


		if ( ! acc_read(ACC_OUT_X_L, acc_out_data_x) ){

			continue;
		}

		if ( ! acc_read(ACC_OUT_Y_L, acc_out_data_y) ){	// accelerometer data collection

			continue;
		}

		if ( ! acc_read(ACC_OUT_Z_L, acc_out_data_z) ){

			continue;
		}



		if ( ! gyro_read(GYRO_OUT_X_H, gyro_out_data_x) ){

			continue;
		}
		if ( ! gyro_read(GYRO_OUT_Y_H, gyro_out_data_y) ){	// gyroscope data collection

			continue;
		}
		if ( ! gyro_read(GYRO_OUT_Z_H, gyro_out_data_z) ){

			continue;
		}



		x_pos = calculate_position(gyro_out_data_x, acc_out_data_x);
		y_pos = calculate_position(gyro_out_data_y, acc_out_data_y);	// raw data gets computed into a usable position
		z_pos = calculate_position(gyro_out_data_z, acc_out_data_z);	// uses fixed point math



		required_x_motion = pid_compute_x(x_pos, requested_aileron_pos);
		required_y_motion = pid_compute_y(y_pos, requested_elevator_pos);	// the pid algorythim computes the required strength of each pair of motors.


		encode_to_motors(motors, required_x_motion, required_y_motion, success);// coverts the processed data into individual motor instructions

		if (success){

			pass_to_pwm_chip(motors);// final results get passed to the chip and eventually to the esc's
		}

	}


		return 0;

}




	
// Note that there is a strong probability that INT0 and INT1 will fire at the same clock cycle. That should be fine as the routines are short and one will just be executed after the other
// This code will remain, but will not be triggered by muxes.

ISR(INT0_vect ){

			if ( TCNT1H < temp_timer_throttle){	// timer overflow

			requested_throttle_pos = 0xff- temp_timer_throttle + TCNT1H ;
		}

		else{
			
			requested_throttlr_pos = TCNT1H - temp_timer_throttle;
		}
				
		temp_timer_throttle = TCNT1H;

}


ISR(INT1_vect ){

			if ( TCNT1H < temp_timer_aileron){	// timer overflow

			requested_aileron_pos = 0xff- temp_timer_aileron + TCNT1H ;
		}

		else{
			
			requested_aileron_pos = TCNT1H - temp_timer_aileron;
		}
		
		temp_timer_aileron = TCNT1H;

}

