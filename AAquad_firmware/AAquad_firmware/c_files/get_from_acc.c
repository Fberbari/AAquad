#include "get_from_acc.h"



int get_from_acc(char axis){

	int* data;

	if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0x32) ){	// slave + w

		return 0;
	} 

	if ( ! send_reg(0x29) ){	// OUT_X_H
		return 0;
	}

	if ( ! repeat_start() ){

		return 0;
	}


	if ( ! send_slave_r(0x33, data) ){	// slave + r

		return 0;
	} 

	if (! get_data(data) ){

		return 0;
	}


	stop();

	return *data;

}