

int init_mux_timer(){

// uses the 8 bit timer for this job


	PRR &= ~(1 << PRTIM0);	// ensures a clock is provided to the timer module

	TCCR0B |= (1 << CS00); // the timer will run on the internal clock (currently with no prescaler)

	TCCR0A |= (1 << WGM01);	// CTC mode (clear timeron compare match)

	TIMSK0 |= (1 << OCIEA);	// Enable output compare match interrupt

	OCR0A = 0xFF;	// defines the TOP value the timer counts to.

}