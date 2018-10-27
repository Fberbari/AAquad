#include "pwm_in_vars.h"
#include "avr_compiler.h"


/****************************************************
*	This class is responsible for converting the raw data gathered by the external inteerupts into angles that represent the pilot's intention of the quad's orientation with regards to gravity
*	It also provides a percentage a full power that the pilot demands with his throttle lever
*	
*	
****************************************************/




#ifndef PILOT_INSTRUCTIONS
#define PILOT_INSTRUCTIONS


class pilot_instructions{

	public:
		

		// These methods associate this angle with the extreme stick positions on the  
		void set_max_angle(uint8_t max_angle);
		void set_max_yaw_rate(uint8_t max_yaw_rate);

		// converts the data captured by the interrupts into usable angles 
		// that are stored in the in the *_angle variables
		void compute(void);

		//output
		int get_bank_angle() const;
		int get_pitch_angle() const;
		int get_yaw_rate() const;
		uint8_t get_throttle_power() const;


	private:
		
		// defines the maximum angle the quadcopter can be ordered to
		uint8_t max_angle;	
		
		// defines themaximum angular speed the quad should spin with
		uint8_t max_yaw_rate;

		int bank_angle;	// expressed in degrees
		int pitch_angle;// degrees
		int yaw_rate;	// expressed in degrees per second
		uint8_t throttle_power; //expressed as a percentage
		
		
		// these arays store the previous values
		float pitch[5];
		float bank[5];
		float yaw[5];
		float throttle[5];


};

#endif