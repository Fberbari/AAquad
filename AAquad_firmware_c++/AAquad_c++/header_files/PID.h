#include "avr_compiler.h"
#include <math.h>


/************************************************
*	This class gathers the pilots intention and the current position of the quad and perfoms all the PID algorythims.
*	It then stores integers that represent percentages in the "motor array"
*
*	MANY OF THE METHODS HERE HAVE BEEN CREATED FROM A SKELETON, credits to
*	Gregory Frizon Gusberti
*	youtube.com/alleletronicgr
************************************************/






#ifndef PID_H
#define PID_H

#define TIMER_BIT_RATE 1000000	// LSB per second

class PID {

        public:

            PID();

            /// Set the P, I, D terms respectively. Use it AFTER setRefreshInterval or setRefreshRate
            void setWeights(const float &Kp, const float &Ki, const float &Kd);

            /// Set the lower limit of the output. Output will be clamped to this value.
            /// If clamped, the integral will not be computed to prevent integral windup.
            /// To set the upper limit use setOutputUpperLimit
            /// YOU NEED TO SET IT!
            void setOutputLowerLimit(const float &output_lower_limit);

            /// Set the upper limit of the output. Output will be clamped to this value.
            /// If clamped, the integral will not be computed to prevent integral windup.
            /// To set the lower limit use setOutputLowerLimit
            /// YOU NEED TO SET IT!
            void setOutputUpperLimit(const float &output_upper_limit);

            /// Set the desired point of the output.
            /// This is the output the PID loop will try to reach.
            /// error will be computed subtracting the feedback input from this value
            void setDesiredPoint(const float &desired_point);

            /// The refresh funcion does all the PID computation.
            /// The feedback_input is the value from the feedback.
            /// The returned value is the output value of the filter.
            float refresh(const float &feedback_input);

            // this function should be called with bank_pid.refresh and pitch_pid.refresh as arguments.
            // Call it like PID::combine_data
            // currently does nothing with yaw_rate
            static void combine_data(float bank_percentage, float pitch_percentage, float throttle_percentage);
            
			
			// holds the percentage of full power that each individual motor needs to be at. This needs to be read for full output
            // this array will be read by the pilot for info
            static int motor[4];  
        private:


            // updates the time since the last computation
            void update_time();


			float current_error;
            float last_error;
            float output;
			float last_output;
            float set_point;
            float Kp, Ki, Kd;
            float integral;
			float derivative;
            float interval;
            float output_upper_limit;
            float output_lower_limit;


            

            float time_elapsed;
            uint16_t time_of_previous_scan;


};


#endif