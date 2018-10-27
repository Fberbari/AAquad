#include "init.h"



void initialize::interrupts(){

	// external int 0
	EIMSK |= (1 << INT0); // enable the int0 interrupt												
	EICRA |= (1 << ISC00);	// will fire at any logical change

	// external int 1
	EIMSK |= (1 << INT1); // enable the int0 interrupt
	EICRA |= (1 << ISC10);	// will fire at any logical change

	// pcint 1 (for pin C3)
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT11);

	//pcint 2 (for pin B1)
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT17);
	
	

}

void initialize::timers(){

	// Timer 1 is set to just count and start from 0 when overflow
	// It is used to provide time stamps to the pwm pilot input
	TCCR1B |= (1 << CS10);	// timer will run with no prescaler

	// Timer 0 is set to raise an interrupt roughly every 0.1 seconds
	// it will call the pid compute function
	TCCR0A |= (1 << WGM01);	// CTC mode, timer resets everytime it hits the value set in OCR0A
	TCCR0B |= ((1 << CS02) | (CS01));	// clock divide by 8
	TIMSK0 |= (1 << OCIE0A);	// interrupt set to fire when timer matches with the OCROA register
	OCR0A = 98;
}