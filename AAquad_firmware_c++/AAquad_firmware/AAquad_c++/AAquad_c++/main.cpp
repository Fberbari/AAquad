/*
 * Firmware Developper : Anthony Berbari
 */ 

#include <avr/io.h>

#include <stdint.h>

#include "avr_compiler.h"
#include "acc_init.h"
#include "gyro_init.h"

#include "lis3dh_func.h"
#include "init_free_timer.h"
#include "init_ints.h"
#include "pwm_chip_init.h"
#include "pass_to_pwm_chip.h"
#include "I2C_init.h"
#include <util/delay.h>

#define F_CPU 1000000UL


	volatile uint16_t requested_aileron_pos = 0;
	volatile float processed_aileron_pos = 0.0; 
	volatile uint16_t temp_timer_aileron = 0; 
	volatile uint16_t temp0 = 0;	// used in ISR(ext_int_o)
	volatile bool new_aileron_data_available = false;

int main(void){

PORTC |= ( (1 << 5) | (1 << 4) );	// pulup resistors fix this in hardware!!!!!!

init_free_timer();
init_extern_ints();	
I2C_init();


if (! pwm_chip_init() ){

	_delay_ms(5);

	if (! pwm_chip_init() ){

		// decaire error code on led's
	}
}




if (! gyro_init() ){

	_delay_ms(5);

	if (! gyro_init() ){

		// decaire error code on led's
	}
}




if (! acc_init() ){

	_delay_ms(5);

	if (! acc_init() ){

		// decaire error code on led's
	}
}


uint8_t motors[5] = {0};	// carries data transferred to the motors



sei();

while(1){
	
	if (new_aileron_data_available){

		if (requested_aileron_pos > 0x7000 ){
			
			processed_aileron_pos = 0xffff - requested_aileron_pos;
		}
		
		else{
			
			processed_aileron_pos = requested_aileron_pos;
			
		}
		
		
		processed_aileron_pos *= 100;
		processed_aileron_pos /= 6553;	// 10% of the total value of the 16bit register

		
		new_aileron_data_available = false;
	}


	

}



return 0;

}
ISR(INT1_vect){
	
		temp0 = TCNT1;
		

		if ( temp0 < temp_timer_aileron){	// timer overflow

			requested_aileron_pos = (0xffff - temp_timer_aileron) + temp0 ;
		}

		else {	// regular case
	
			requested_aileron_pos = temp0 - temp_timer_aileron;
			
		}
	
		
		temp_timer_aileron = temp0;
		
		
		new_aileron_data_available = true;
		
		// here, there is a chance that the value stored in requested aileron is actually (0xffff - actual requested aileron) this needs to be fixed in the while loop, it has been avoided here to kep the ISR short.
	
}

ISR(INT0_vect){/*
	
		temp0 = TCNT1;
		

		if ( temp0 < temp_timer_throttle){	// timer overflow

			requested_throttle_pos = (0xffff - temp_timer_throttle) + temp0 ;
		}

		else {	// regular case
	
			requested_throttle_pos = temp0 - temp_timer_throttle;
			
		}
	
		
		temp_timer_throttle = temp0;
		
		
		new_throttle_data_available = true;
		
		// here, there is a chance that the value stored in requested throttle is actually (0xffff - actual requested throttle) this needs to be fixed in the while loop, it has been avoided here to kep the ISR short.
	
	*/

}

