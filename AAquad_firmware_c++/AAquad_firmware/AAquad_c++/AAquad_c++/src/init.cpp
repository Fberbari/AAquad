#include "init.h"



void initialize::interrupts(){

	// external int 0
	EIMSK |= (1 << INT0); // enable the int0 interrupt												
	EICRA |= (1 << ISC00);	// will fire at any logical change
/*
	// external int 1
	EIMSK |= (1 << INT1); // enable the int0 interrupt
	EICRA |= (1 << ISC10);	// will fire at any logical change

	// pcint 0 (for pin C3)
	PCICR |= (1 << PCIE0);
	PCMSK1 |= (1 << PCINT11);

	//pcint 1 (for pin B1)
	PCICR |= (1 << PCIE1);
	PCMSK0 |= (1 << PCINT1);
*/
}

void initialize::timers(){

	// by default the timer is set to normal mode
	TCCR1B |= ( (1 <<CS11));	// timer will run with 8x prescaler and willoverrun 15 times/second

}

//