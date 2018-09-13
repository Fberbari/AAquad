
#ifndef CALC_H
#define CALC_H

// This class should hold any functions that would do side calculations at any step.

#include <math.h>
#include <stdint.h>


class calc{

	public:

		int moving_average(int* motors);
		
		
		static int filtered_motors[4];

	private:



};

#endif




