#include <avr/io.h>

#ifndef INIT_INTS

#define _INIT_INTS

// this function just initialises the two pins connected to the muxes to be interrupt pins
void init_extern_ints();
void init_pcints();


#endif