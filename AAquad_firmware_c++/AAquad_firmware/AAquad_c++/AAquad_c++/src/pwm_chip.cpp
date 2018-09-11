#include "pwm_chip.h"



pwm_chip::pwm_chip(I2C_328pb i2c, uint8_t prescaler){
	
	
	DDRB |= (1 << 2);	// set OE to 0;
	
	PORTC |= ( (1 << 4) | (1 << 5) );

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 

	//TWBR0 = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ


	i2c.start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0x0); // mode register 1
	i2c.send(0x21); //clock on, autoincrement enable
	i2c.repeat_start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0xFE);	//pre scale register
	i2c.send(prescaler);	//prescaler
	i2c.stop();

}




void pwm_chip::pass(I2C_328pb i2c, int* motors){

// this function will communicate over I2C to the pwmchip for final controll of the motors

	uint8_t instruction[2];
	
	encode_motors(0, motors, instruction);

	i2c.start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0x06);		//LED0_ON_L
	i2c.send(0);
	i2c.send(0);
	i2c.send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	i2c.send(instruction[1]);
	i2c.stop();


	encode_motors(1, motors, instruction);
	

	i2c.start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0x16);		//LED4_ON_L
	i2c.send(0);
	i2c.send(0);
	i2c.send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	i2c.send(instruction[1]);
	i2c.stop();

	
	encode_motors(2, motors, instruction);



	i2c.start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0x2E);		//LED8_ON_L
	i2c.send(0);
	i2c.send(0);
	i2c.send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	i2c.send(instruction[1]);
	i2c.stop();



	encode_motors(3, motors, instruction);


	i2c.start();
	i2c.send_slave(0x9E);
	i2c.send_reg(0x42);		//LED8_ON_L
	i2c.send(0);
	i2c.send(0);
	i2c.send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	i2c.send(instruction[1]);
	i2c.stop();


}

void pwm_chip::encode_motors(uint8_t motor, int* motors, uint8_t* instruction){
	
	uint16_t temp = motors[motor]*2;	// the actualslope of this curve is 2.05
	
	temp += 205;	// 205 is the value corresponding to 0 for the esc
	
	instruction[0] = ( temp & 0xff );	// conserves only the low byte
	
	instruction[1] = ( temp >> 8);	// conserves only the high half-byte
	
}