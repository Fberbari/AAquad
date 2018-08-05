
#include "decode_motors.h"



void decode_motors(uint8_t motor, uint8_t* motors, uint8_t*instruction){
	
	uint16_t temp = motors[motor] * 205;	// the actualslope of this curve is 2.05, 100 times larger prevents the .05from falling off
	
	temp = ( temp / 100 ) + 205;	// 205 is the value corresponding to 0 for the esc
	
	instruction[0] = ( temp & 0xff );	// conserves only the low byte
	
	instruction[1] = (temp >> 8);	// conserves only the high half-byte
	
	return;
	
}
