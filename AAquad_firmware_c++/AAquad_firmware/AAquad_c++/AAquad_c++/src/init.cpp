#include "init.h"



void initialize::interrupts(){

	// external int 0
	EIMSK |= (1 << INT0); // enable the int0 interrupt												
	EICRA |= (1 << ISC00);	// will fire at any logical change
/*
	// external int 1
	EIMSK |= (1 << INT1); // enable the int0 interrupt
	EICRA |= (1 << ISC10);	// will fire at any logical change

	// pcint 1 (for pin C3)
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11);
*/
	//pcint 2 (for pin B1)
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT17);
	
	// setting the pullup resistors on everything (will not be necessary once all wires are connected

	DDRD &= ~(1 << 2);
	PORTD |= (1 << 2);
	
	DDRC &= ~(1 << 3);
	PORTC |= (1 << 3);
	
	DDRD &= ~(1 << 3);
	PORTD |= (1 << 3);
	
	DDRD &= ~(1 << 4);
	PORTD |= (1 << 4);
	
	

}

void initialize::timers(){

	// by default the timer is set to normal mode
	TCCR1B |= ( (1 << CS10));	// timer will run without prescaler and willoverrun 15 times/second

}