#include "../header_files/init_extern_ints.h"

void init_extern_ints(){

	// int 0

	EIMSK |= (1 << INT0); // enable the int0 interrupt

	EICRA |= (1 << ISC00);	// will fire at any logical change

	// int 1

	EIMSK |= (1 << INT1); // enable the int0 interrupt

	EICRA |= (1 << ISC10);	// will fire at any logical change

}