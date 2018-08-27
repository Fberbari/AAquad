#include "../header_files/init_ints.h"

void init_extern_ints(){

	// int 0

	EIMSK |= (1 << INT0); // enable the int0 interrupt
																	
	EICRA |= (1 << ISC00);	// will fire at any logical change

	// int 1

	EIMSK |= (1 << INT1); // enable the int0 interrupt

	EICRA |= (1 << ISC10);	// will fire at any logical change

}

void init_pc_ints(){

	// pcint 0 (for pin C3)

	PCICR |= (1 << PCIE0);
	PCMSK1 |= (1 << PCINT11);

	//pcint 1 (for pin B1)

	PCICR |= (1 << PCIE1);
	PCMSK0 |= (1 << PCINT1);
}