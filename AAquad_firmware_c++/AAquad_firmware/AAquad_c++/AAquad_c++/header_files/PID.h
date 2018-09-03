#include "avr_compiler.h"
#include <math.h>

#ifndef PID_H
#define PID_H


class PID {

        public:

            PID();

            /// Set the P, I, D terms respectively. Use it AFTER setRefreshInterval or setRefreshRate
            void setWeights(const float &Kp, const float &Ki, const float &Kd);
/*
            /// Set the refresh interval of the controller in seconds.
            /// If you don't know the interval you can set the frequency of update using setRefreshRate
            void setRefreshInterval(const float &refresh_interval);
*/
/*
            /// Set the refresh frequency of the controller in hertz.
            /// If you don't know the refresh frequency you can set the refresh interval using setRefreshRate
            void setRefreshRate(const float &refresh_rate);
*/
/*
            /// Set the minimun error for computation of the PID loop. The default is 0.0
            /// It can be set to prevent integral windup or updates with minimal errors
            void setErrorThreshold(const float &error_threshold);
*/
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

            /// The refresh funcion that make all the PID computation.
            /// (use a timer interrupt if used on a embedded microcontroller)
            /// The feedback_input is the value from the feedback.
            /// The returned value is the output value of the filter.
            float refresh(const float &feedback_input);

            // this function should be called with bank_pid.refresh and pitch_pid.refresh as arguments.
            // Call it like PID::combine_data
            // currently does nothing with yaw_rate
            static void combine_data(float bank_percentage, float pitch_percentage, float throttle_percentage);

            // returns the pointer to the motors array for use in the Pwm class
            static int* get_motor() const;

        private:


            // updates the time since the last computation
            void update_time();


		float current_error;
            float last_error;
            float last_output;
            float output;
            float set_point;
            float Kp, Ki, Kd;
            float integral;
            float interval;
            float output_upper_limit;
            float output_lower_limit;

            // holds the percentage of full power that each individual motor needs to be at. This needs to be read for full output
            // this array will be read by the pilot for info
            static int motor[4];  
            

            float time_elapsed;
            uint16_t previous_clock;


};


#endif