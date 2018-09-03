/*
 * Firmware Developer : Anthony Berbari
 */ 

#include "avr_compiler.h"


#include <util/delay.h>

#include "init.h"

	
	volatile uint16_t pwm_width = 0;
	volatile uint16_t temp0 = 0;
	volatile uint16_t temp1 = 0; 
	
	
	volatile uint16_t temp = 0;
	volatile uint16_t temp_timer_aileron = 0;
	volatile uint16_t requested_aileron_pos = 0;
	
int main(void){
	

	TCCR1B |= (1 << CS10);	// timer starts

	PORTD |= (1 << 2);	// pin D2 set as an input 
	
	initialize::interrupts();
	
	sei();
	
	while(1){
		
	/*
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
		
		*/
	
		if (requested_aileron_pos < 10000){
			
			pwm_width = requested_aileron_pos;
		}
		else{
			
			pwm_width = 0xffff - requested_aileron_pos;
		}

	}



return 0;

}


ISR(INT0_vect){ 			

	uint16_t temp = TCNT1;
	 
	if ( temp < temp_timer_aileron){	// timer overflow 			
		
		requested_aileron_pos = (0xffff - temp_timer_aileron) + temp ; 		
	} 		
	
	else {	// regular case 				
		
		requested_aileron_pos = temp - temp_timer_aileron;
	} 					
	
	temp_timer_aileron = temp; 						
	
	
	// here, there is a chance that the value stored in requested aileron is actually (0xffff - actual requested aileron) this needs to be fixed in the while loop, it has been avoided here to kep the ISR short. 	
	
	}