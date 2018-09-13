#include "pwm_in_vars.h"
#include "avr_compiler.h"


#ifndef PILOT_INSTRUCTIONS
#define PILOT_INSTRUCTIONS


class pilot_instructions{

	public:

		void set_max_angle(uint8_t max_angle);
		void set_max_yaw_rate(uint8_t max_yaw_rate);

		void compute(void);


		int get_bank_angle() const;
		int get_pitch_angle() const;
		int get_yaw_rate() const;
		uint8_t get_throttle_power() const;


	private:

		uint8_t max_angle;	// defines the maximum angle the quadcopter should be ordered to
		uint8_t max_yaw_rate;

		int bank_angle;
		int pitch_angle;
		int yaw_rate;	// expressed in degrees per second
		uint8_t throttle_power; //expressed as a percentage



};

#endif