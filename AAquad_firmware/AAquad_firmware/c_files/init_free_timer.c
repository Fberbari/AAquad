#include "init_free_timer.h"

// This 16 bit timer is always running free. The interrupt handlers of the external interrupts(or anything else) may use it to keep track of time
// With the current prescaler, this timer overuns every 60 ms or so. As the pwm is guaranteed to have a period of < 20 ms, reading the timer effectively produces 16 bits resolution
// To read this timer, store the contents of TCNT1H and TCNT1L into a 16bit variable
void init_free_timer(){


	// by default the timer is set to normal mode

	TCNT1H |= (1 << CS11);	// timer will run with an x8 prescaler 
}