#include "init_free_timer.h"

// This 16 bit timer is always running free. The interrupt handlers of the external interrupts(or anything else) may use it to keep track of time
// With the current prescaler, this timer overuns every 65 ms or so. As the pwm is guaranteed to have a period of < 20 ms, reading the timer effectively produces 16 bits resolution
// To read this timer, store the contents of TCNT1 into a 16 bit variable
void init_free_timer(){


	// by default the timer is set to normal mode

	TCCR1B |= ( (1 <<CS10));	// timer will run with an no prescaler 
}