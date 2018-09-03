/*
 * Firmware Developer : Anthony Berbari
 */ 

#include "avr_compiler.h"


#include <util/delay.h>

	
	volatile uint16_t pwm_width = 0;
	volatile uint16_t temp0 = 0;
	volatile uint16_t temp1 = 0; 
	
int main(void){

	TCCR1B |= (1 << CS10);	// timer starts

	PORTD |= (1 << 2);	// pin D2 set as an input 
	
	while(1){
		
		
		if (PIND == (PIND | (1 << 2)) ){	// rising edge detected     
			
			temp0 = TCNT1;
			
			while (PIND == (PIND | (1 << 2) ));
			
			temp1= TCNT1;
			
			if ( temp1 < temp0 ) {
				
				pwm_width = 0xffff - temp0 + temp1;
			}
			
			else{
				
				pwm_width = temp1 - temp0 ;
			}
			
		}
		
		
	}



return 0;

}


