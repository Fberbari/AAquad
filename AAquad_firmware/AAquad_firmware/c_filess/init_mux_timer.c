

void init_mux_timer(){

// this timer is responsible for providing the pwm that periodically flips the muxes back and forth
// uses the 8 bit timer 0 for this job


	PRR &= ~(1 << PRTIM0);	// ensures a clock is provided to the timer module

	TCCR0B |= (1 << 0); // (CS00)the timer will run on the internal clock (currently with no prescaler)

	TCCR0A |= (1 << 1);	//(WGM01) CTC mode (clear timer on compare match)

	TIMSK0 |= (1 << OCIEA);	// Enable output compare match interrupt

	OCR0A = 0xFF;	// defines the TOP value the timer counts to.

}


// every time the interrupt (TIMER0_OVF) fires, PC3 must toggle.