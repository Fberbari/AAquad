#include <avr/io.h>
#include "I2C_328pb.h"
#include <stdint.h>

#ifndef PASS_TO_PWM_CHIP

#define PASS_TO_PWM_CHIP

int pass_to_pwm_chip(uint8_t* motors);


#endif

