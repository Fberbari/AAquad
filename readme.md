This is an implementation of a quadcopter filght computer running baremetal on an atmega 328pb chip that takes input a standard hobby receiver and outputs pwm to esc's.



These directories contain the full firmware and the pcb design + gerber files.

As of right now, the system produces desirable results and has passed some basic functionality tests. 
I am currently waiting on a mechanical engineering student to finish designing the frame of the aircraft before further testing.





WORKING PRINCIPLE

	As of right now, the system reads input from the receiver via an interrupt based (asynchronous approach). This, because of the 50Hz pwm encoding that the receiver emits is by far the slowest process.

	Apart from this step, the rest of the processis synchronous and the loop goesas follows:

		1. The processor gathers data from the 2 on board sensors (accelerometer and gyroscope) via I2C.
		2. Using a modified version of the Kallman filter, the processor uses both pieces of data to determine the absolute position of the aircraft (with reference to gravity)
		3. A function gathers all the newest available data from the pilot and computes the pilots intention. (How should the aircraft bank and pitch and how strong should 		motor power be)
		4. A PID algorythim then uses the processed sensor data aswell, the processed pilot data and the previous cycle's data to compute 4 numbers that represent the 				percentage of full power that each motor should run at to best achieve the pilot's wishes
		5. Those numbers are encodedinto values that the pwm chip can understand and sent to it via I2C.
		6. The process repeats



PURPOSE

	The purose of this project was to familiarize myself with the basics of avr architecture, firmware design flow, communications between chips (I2C), pcb design flow etc.
	It was also a way to enter the world of embedded design in aviation. 






I (Anthony Berbari) AM SOLELY RESPONSIBLE FOR ALL THE FIRMWARE AND HARDWARE DESIGN DONE ON THIS PROJECT.