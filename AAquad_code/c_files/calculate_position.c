#include <stdfix.h>

#include "../header_files/calculate_position.h"


// this function needs a lot of work! But the gist of it is here :)


int calculate_position(int gyro_data, int acc_data){

	// data from gyro is in degrees / s
	// data from acc is in g's



	// for initial stuff, i will say 8.75 mdps/digit sensitivity

	// only reading top register, so a an increase by 1 represents a change of 2.2 dgps
	volatile accum dgps = 0k;
	volatile accum rps = 0k;
	volatile accum g_offset = 0k;

	dgps = (2.2 * gyro_data);

	// convert to radians per second

	rps = dgps*3.1415/365;

	// convertto g's via a = w**2 r

	g_offset = rps*rps*R / 9.8;	//!!!!!!!!!!!!!!! R needs to be defines once the mechanical construction of the quad is finished.


	// for the acc, sensitivity , I will say is 5 mg / digit

	volatile accum acc_calculated = 0k;

	acc_calculated = acc_data * 0.005;

	// now, gyro and acc data must be combined

	volatile accum true_acc = 0k;

	volatile accum true_bank = 0k;


	true_acc = acc_calculated + g_offset;


	true_bank = sine_inverse(true_acc/9.8);

	return true_bank;

}


//////// DOnt forget to account for actualacceleration of the quad upand down which could corrupt angle data.