#include "avr_compiler.h"


#ifndef SENSOR_CLASS

#define SENSOR_CLASS



class sensor

{

public:

	sensor();		// default constructor 50 khz clock

	sensor(int bit_rate); // constructor (max SCL is 100 khz in normal mode and 400 khz in fast)

	void set_slave(int address);

	void set_write_reg(int reg);

	void set_read_reg(int reg);

	bool write(int data_in);

	bool read(int* data_out);




private:


	int slave_address;

	int write_reg;

	int read_reg;

};



#endif