
#include "avr_compiler.h"

#include "header_files/l3gd20h_func.h"

#include "header_files/lis3dh_func.h"

#define 	F_CPU   1000000UL

#include <util/delay.h>

#include <avr/interrupt.h>



						// This code should transmit accelerometer data to some led's.

void Error(void);


int var = 0;


int main(){

/*
sei(); // enable global interupts

PRR &= ~(1 << PRADC); // provide power to the ADC's

ADCSRA |= (1 << ADEN);	// enable the ADC's

ADMUX |= (1 << REFS0);// comparison voltage

// ADC 0 is the name of the game



ADMUX |= (1 << ADLAR);// left adjust

ADCSRA |= (1 << ADIE); // enable ADC Interrupt

ADCSRA |= (1 << ADPS2);	// run at 62 KHz

ADCSRA |= (1 << ADSC); // start conversion

*/




DDRB = 0xff;



TCCR1B |= ( (1 << WGM13) | (1 << WGM12) | (1 << CS10) );		// selecting fast PWM mode (counts up till selected value then resets)

TCCR1A |= ((1 << WGM11)| (1<<COM1A1) | (1<<COM1A0));	// set inverting mode fast pwm


ICR1 = 19999;

OCR1A = 16000;


	while (1){ /*

		while (OCR1A > 17600){

		OCR1A -= 10;

		_delay_ms(100);

		}

		while (OCR1A < 19500){

		OCR1A += 10;

		_delay_ms(100);

		}*/
	

	}

		return 0;

}



void Error(void){



		PORTB |= (1 << 1);

		while(1);


	// this function should only be called if uC has lost contact with the gyroscope or accelerometer

	// it should deploy the parashute.



return;


}

ISR(ADC_vect){

	var = (100 * ADCH);

	ADCSRA |= (1 << ADSC); // start conversion


}


