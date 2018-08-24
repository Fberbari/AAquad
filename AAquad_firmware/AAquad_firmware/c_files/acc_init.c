#include "acc_init.h"

int acc_init(){


	if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0x32) ){	// slave + w

		return 0;
	} 

	if ( ! send_reg(0x20) ){	// Control register 1

		return 0;
	}

	if ( ! send(0x67) ){	// all axis enable data refresh rate is 200Hz

		return 0;
	}


	stop();
	
	return 1;
}