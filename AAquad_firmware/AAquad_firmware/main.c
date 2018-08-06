/*
 * Developper : Anthony Berbari
 */ 

#include <avr/io.h>

#include <stdint.h>

#include "avr_compiler.h"

#include "lis3dh_func.h"
#include "init_mux_timer.h"
#include "init_free_timer.h"
#include "init_ints.h"
#include "pwm_chip_init.h"
#include "pass_to_pwm_chip.h"
#include <util/delay.h>


#include "macros/acc_defines.h"

#define F_CPU 1000000UL

	volatile uint16_t requested_aileron_pos = 0; 
	volatile uint16_t temp_timer_aileron = 0; 
	volatile uint16_t temp = 0;

int main(void){

PORTC |= ( (1 << 5) | (1 << 4) );

sei();

uint8_t motors[5] = {0};

pwm_chip_init();
pass_to_pwm_chip(motors);


DDRB |= (1 << 1);
PORTB |= (1 << 0);

init_mux_timer();	// a lot of this function is disconnected
init_free_timer();
init_extern_ints();	

uint16_t garbage = 0;

while(1){

motors[0] = garbage;
pass_to_pwm_chip(motors);
garbage ++;

_delay_ms(4);


}



return 0;

}
ISR(INT1_vect){
	
		temp = TCNT1;
		
/*
		if ( temp < temp_timer_aileron){	// timer overflow

			requested_aileron_pos = 0xff- temp_timer_aileron + temp ;
		}
*/
		if (temp > temp_timer_aileron){
	
			requested_aileron_pos = temp - temp_timer_aileron;
			
		}
	
		
		temp_timer_aileron = temp;
		

		
		
			OCR4A = (requested_aileron_pos >> 8);


}

ISR(INT0_vect){
	
	DDRB |= (1 << 7);

}

