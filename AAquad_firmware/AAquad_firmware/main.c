/*
 * Developper : Anthony Berbari
 */ 

#include <avr/io.h>

#include <stdint.h>

#include "avr_compiler.h"

#include "lis3dh_func.h"
#include "init_mux_timer.h"


#include "macros/acc_defines.h"

#define F_CPU 1000000UL

int main(void){
	
	sei();

DDRC |= (1 << 3);
DDRB |= (1 << 0);
//init_mux_timer();

DDRC = 0;
PORTC = 0xFF;
while(1){

}



return 0;

}

ISR((TIMER0_COMPA_vect)){

	PORTC ^= (1 << 3);
	PORTB ^= (1 << 0);
}