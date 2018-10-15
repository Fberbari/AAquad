#include "avr_compiler.h"
#include <math.h>
#include "I2C_328pb.h"




/**********************************************************
*	This class contains all the methods that gather and process the sensor data into angles that reflect the gyroscope's current position.
*	All angles are with respect to gravity (0 bank and 0 pitch being the level position)
*	Please see the hardware for the positive and negative directions.
*
*
*	The algorythim used to combine the accelerometer and gyroscope data is a simplified version of the Kalman filter
*	Outlined here: http://www.pieter-jan.com/node/11
*
*
*
*	This algorithym (with some modification) has produced very reliable results in testing.
**********************************************************/






#ifndef SENSORS
#define SENSORS


#define GYRO_SENSITIVITY 0.00875	// unit is mdps/digit
#define TIMER_BIT_RATE 1000000	// LSB per second



#define ACC_SENSITIVITY 0.000123	// ( 8md/digit is what is is supposed to be. Tests revealed 1g corresponds to 1130 or so which is consistant with the +- 4g's requested)
#define INVERSE_ACC_SENSITIVITY 8130	// this is nothing more than 1/ACC_SENSITIVITY


#define DEGREE_CONVERSION_FACTOR 57.3	// ratio of 180/pi

class sensors : private I2C_328pb{

	public:
		
		//configuration of all the sensors, things like refresh rate, sensitivity selection
		sensors();
		
		
		// grabbs the 2 Bytes of data for each axis from the acc. Raw results are stored in acc_""_data
		void read_acc();
		// grabbs the 2 Bytes of data for each axis from the gyro. Raw results are stored in gyro_""_data
		void read_gyro();
		
		
		// combines the raw data from the acc and gyro in a *smart* way and updates the pitch and roll angles. Also produces the yaw rate.
		void compute_position();
		
		
		
		float get_pitch() const;
		float get_roll() const;
		float get_yaw_rate() const;


	private:

		float get_time(); // gives us time since last scan
		
		//raw data from acc
		int16_t acc_x_data;
		int16_t acc_y_data;	
		int16_t acc_z_data;
		
		// raw data from gyro
		int16_t gyro_x_data;
		int16_t gyro_y_data;
		int16_t gyro_z_data;

		
		// holds the angles of the CURRENT position
		float pitch;
		float roll;	
		
		
		//holds the speed of the yawing motion in deg/s
		float yaw_rate;

		// variable used by the get_time method
		uint16_t time_of_previous_scan;  


};


#endif
