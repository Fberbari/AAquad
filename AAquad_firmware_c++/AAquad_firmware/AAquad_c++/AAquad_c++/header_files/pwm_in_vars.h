#include "avr_compiler.h"

#ifndef PWM_IN_VARS
#define PWM_IN_VARS


extern volatile uint16_t requested_aileron_pos;
extern volatile uint16_t requested_elevator_pos;
extern volatile uint16_t requested_rudder_pos;
extern volatile uint16_t requested_throttle_pos;

#endif
