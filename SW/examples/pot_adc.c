#include "avr/io.h"

#define F_CPU 8000000UL
#include "avr/delay.h"

/**************************************************
 * description:
 * 
 * Implements crude PWM modulation of LED1 using the
 * result of the ADC.
 * Rotating the pot.meter in one direction should increase
 * its light, turning it another way should decrease it.
 * Only bad thing about integer math here is that at the
 * bottom of the result, it suddenly jumps, at the top
 * it does nothing.
 **************************************************/
 
#define MAX_RESULT_VALUE (1024u)
#define LED1 PINC2


int main()
{
	ADMUX |= 1<<REFS0; // selecting AVCC as reference voltage for the DAC
	ADMUX &= ~(1<<REFS1); // selecting AVCC as reference voltage for the DAC
	ADCSRA |= 1<<ADFR; //activate free running mode
	//ADMUX &= ~(1<<ADLAR); //Make result right adjusted
	//ADMUX &= 0b11100000; // selecting channel 0 for the input
	ADCSRA |= 0b00000111; // dividing F_CPU by 128 = 62500 Hz for the ADC
	ADCSRA |= 1<<ADEN; // enable ADC
	ADCSRA |= 1<<ADSC; // start free running
	
	DDRC |= 1<<LED1; // init output for led


	unsigned short int result = 0;
	unsigned short int period_ms = 20;
	unsigned short int on_time = 0;
	while(4){
		/* get conversion result */
		result = (unsigned short int)ADCL;
		result += (unsigned short int)ADCH<<8;
		
		/* calculate led's ON time,do some pwm */
		on_time = (period_ms*result)/MAX_RESULT_VALUE;
		PORTC |= 1<<LED1;
		for(int i = on_time; i; --i){
			_delay_ms(1);
		}
		PORTC &= ~(1<<LED1);
		for(int i = period_ms - on_time; i; --i){
			_delay_ms(1);
		}
	}
	
 return 0;
}


/*
 * pot_adc.c
 * This file is part of examples
 *
 * Copyright (C) 2020 - theQuetzalcoatl
 *
 * examples is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * examples is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with example,see <http://www.gnu.org/licenses/>.
 */

