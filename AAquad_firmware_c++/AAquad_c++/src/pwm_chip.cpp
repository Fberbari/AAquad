#include "pwm_chip.h"



pwm_chip::pwm_chip( uint8_t prescaler): I2C_328pb(0x10){
	
	
	DDRB |= (1 << 2);	// set OE to 0;

	//This function sets the auto increment, the prescaler and any other necessary feature required for the pwm chip to run 

	//TWBR0 = (1 << 1); // I'll run the cpu at 1 MHz, this divides the value by 2 for 50 KHZ


	start();
	send_slave(0x9E);
	send_reg(0xFE);	//pre scale register
	send(prescaler);	//prescaler
	repeat_start();
	send_slave(0x9E);
	send_reg(0x0); // mode register 1
	send(0x21); //clock on, autoincrement enable
	stop();

}




void pwm_chip::pass( int* motors){

// this function will communicate over I2C to the pwmchip for final controll of the motors

	uint8_t instruction[2];
	
	encode_motors(0, motors, instruction);

	start();
	send_slave(0x9E);
  	send_reg(0x06);		//LED0_ON_L
	send(0);
	send(0);
	send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	send(instruction[1]);
	stop();


	encode_motors(1, motors, instruction);
	

	start();
	send_slave(0x9E);
	send_reg(0x16);		//LED4_ON_L
	send(0);
	send(0);
	send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	send(instruction[1]);
	stop();

	
	encode_motors(2, motors, instruction);



	start();
	send_slave(0x9E);
	send_reg(0x2E);		//LED8_ON_L
	send(0);
	send(0);
	send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	send(instruction[1]);
	stop();



	encode_motors(3, motors, instruction);


	start();
	send_slave(0x9E);
	send_reg(0x42);		//LED8_ON_L
	send(0);
	send(0);
	send(instruction[0]); //ON_L, ON_H, OFF_L, OFF_H
	send(instruction[1]);
	stop();


}

void pwm_chip::encode_motors(uint8_t motor, int* motors, uint8_t* instruction){
	
	uint16_t temp = motors[motor]*2;	// the actualslope of this curve is 2.05
	
	temp += 205;	// 205 is the value corresponding to 0 for the esc
	
	instruction[0] = ( temp & 0xff );	// conserves only the low byte
	
	instruction[1] = ( temp >> 8);	// conserves only the high half-byte
	
}