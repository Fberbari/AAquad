#include "sensors.h"



sensors::sensors(I2C_328pb i2c){

		if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0x32) ){	// acc slave + w

		return 0;
	} 

	if ( ! send_reg(0x20) ){	// Control register 1

		return 0;
	}

	if ( ! send(0x67) ){	// all axis enable data refresh rate is 200Hz

		return 0;
	}


	stop();

////////////////////////////// accelerometer init above and guro init below

	if  ( ! start() ){

		return 0;
	} 

	if ( ! send_slave(0xD6) ){	// gyro slave + w

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



int sensors::read_acc(I2C_328pb i2c, char axis){


}

int sensors::read_gyro(I2C_328pb i2c, char axis){


}