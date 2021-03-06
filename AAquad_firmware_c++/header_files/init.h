#include "avr_compiler.h"


/********************************************************************
* This class contains the methods that initialise various peripherals
********************************************************************/

#ifndef INIT
#define INIT


class initialize{

	public:

		// initilises all the registers required for the interrupts to function except sei();
		static void interrupts();

		// This 16 bit timer is always running free. The interrupt handlers of the external interrupts(or anything else) may use it to keep track of time
		// With the current prescaler, this timer overuns every 80 ms or so on an 8MHz clock. As the pwm is guaranteed to have a period of < 20 ms, reading the timer effectively produces 16 bits resolution
		// To read this timer, store the contents of TCNT1 into a 16 bit variable
		static void timers();

};


#endif