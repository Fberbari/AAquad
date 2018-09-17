#include "gyro_init.h"

int gyro_init(){


	if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0xD6) ){	// slave + w

		return 0;
	} 

	if ( ! send_reg(0x20) ){	// Control register 1

		return 0;
	}

	if ( ! send(0x15) ){	// all axis enable data refresh rate is 100Hz (a lot of other params also affected)

		return 0;
	}


	stop();
	
	return 1;
}