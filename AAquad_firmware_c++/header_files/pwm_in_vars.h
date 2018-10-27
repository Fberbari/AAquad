#include "avr_compiler.h"


/************************************
* This file contains the declarations of the global variables that the interrupts use to store raw pilot data
*Currently, this file is only included in main.cpp and pilot_instructions.h
************************************/


#ifndef PWM_IN_VARS_H
#define PWM_IN_VARS_H


extern volatile uint16_t requested_aileron_pos;
extern volatile uint16_t requested_elevator_pos;
extern volatile uint16_t requested_rudder_pos;
extern volatile uint16_t requested_throttle_pos;

#endif
