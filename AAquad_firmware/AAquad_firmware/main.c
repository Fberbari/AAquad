/*
 * Developper : Anthony Berbari
 */ 

#include <avr/io.h>

#include <stdint.h>

#include "avr_compiler.h"

#include "lis3dh_func.h"
#include "init_mux_timer.h"
#include "init_free_timer.h"


#include "macros/acc_defines.h"

#define F_CPU 1000000UL

	volatile uint8_t requested_aileron_pos = 0; 
	volatile uint8_t temp_timer_aileron = 0; 


int main(void){
	
	sei();

DDRC |= (1 << 3);
DDRB |= (1 << 0);
init_mux_timer();
init_free_timer();

DDRC = 0;
PORTC = 0xFF;


DDRD = 0xff;
TCCR4B |= ( (1 << WGM13) | (1 << WGM12) | (1 << CS10) );		// selecting fast PWM mode (counts up till selected value then resets)

TCCR4A |= ((1 << WGM11)| (1<<COM1A1) | (1<<COM1A0));	// set inverting mode fast pwm


ICR4 = 19999;

OCR4A = 16000;

PORTB |= (1 << 0);

while(1){

	OCR4A = 19999 - (requested_aileron_pos * 100);	// try setting requested aileron to a constant

}



return 0;

}
ISR(INT1_vect){


		if ( TCNT1H < temp_timer_aileron){	// timer overflow

			requested_aileron_pos = 0xff- temp_timer_aileron + TCNT1H ;
		}

		else{
			
			requested_aileron_pos = TCNT1H - temp_timer_aileron;
		}
		
		PORTB |= (1 << 0);
		
		temp_timer_aileron = TCNT1H;

}