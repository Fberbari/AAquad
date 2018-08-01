#include "init_mux_timer.h"

void init_mux_timer(){

// this timer is responsible for providing the pwm that periodically flips the muxes back and forth
// uses the 8 bit timer 0 for this job


	TCCR0B |= ((1 << CS01) | (1 << CS00)); // the timer will run on the internal clock (currently with 64x prescaler)

	//TCCR0A |= (1 << WGM01);	// CTC mode (clear timer on compare match)

	//TIMSK0 |= (1 << OCIE0A);	//  Enable output compare match interrupt

	//OCR0A = 0xFF;	// defines the TOP value the timer counts to.

}
