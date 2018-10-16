#include "avr_compiler.h"
#include <math.h>

/*********************************************
*
* This function is called repeatedly in the main while loop to integrate the data processed by the pidfunction
*
*
*********************************************/



#ifndef COMBINE_DATA
#define COMBINE_DATA

    /****************************************************************
	Arguments:
		the numbers between [-50, 50] that correspond to how hard the quad should bank and pitch
		the percentage between [0, 100] that represents the throttle the pilot commands

	Error handling:
		none. This function will only run correctly if all the classes previous have returned properly
		
	returns:
		a pointer to an array of 4 ints that contain percentages [0, 100] that represent how fast each motor should spin toachieve the pilot's demands

	Description:
		

	Notes:
		this function returns the address of a static array.

    *****************************************************************/
	int* combine(float bank_number, float pitch_number, float throttle_percentage);


#endif