#include "avr/io.h"

#define F_CPU 8000000UL	
#include "avr/delay.h"

/**************************************************
 * description:
 * -------------------------------------
 * when the user tpouches the left button, the leftmost
 * led should light up, touching the right button
 * the led next to the mentioned one should.
 * Touch sensitivity can be adjusted if necessary.
 *************************************************/
 
#define treshold_isnt_reached_for(pin) !(PING & 1<<pin)
#define treshold_not_reached() !(PING & 0b00000011)
#define SENSITIVITY (50u)

	unsigned char initial_button_val = 0u;

void process_button_press(unsigned char button_pin, unsigned char led)
{
	unsigned char captured_time_val = 0u;

	TCNT0 = 0u;
	DDRG &= ~(1<<button_pin); // release the touch button capacitance
	while(treshold_isnt_reached_for(button_pin)){;}
	captured_time_val = TCNT0;
	if(initial_button_val < captured_time_val - SENSITIVITY)	PORTC |= 1<<led;
	else PORTC &= ~(1<<led);
	DDRG |= 1<<button_pin; // short the button capacitance
}
 
int main()
{
	DDRC |= 1<<PINC2 | 1<<PINC3; // init the two leds
	TCCR0 |= 1<<CS00; // start Timer0 with no prescaling
	
	DDRG |= 1<<PING0 | 1<<PING1; //init button sensing pins as output
	PORTG &= ~(1<<PING0 | 1<<PING1);
	
	_delay_us(1);
	TCNT0 = 0u;
	DDRG &= ~(1<<PING0 | 1<<PING1); // release the touch button capacitances
	while(treshold_not_reached()){;} // assuming maximum 1 button is being touched at startup, not both
	initial_button_val = TCNT0;
	DDRG |= 1<<PING0 | 1<<PING1;
		
	while(4){
		process_button_press(PING0, PINC2);
		process_button_press(PING1, PINC3);
	}

 return 0;
}

/*
 * capacitive_buttons.c
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
 * along with <program name>. If not, see <http://www.gnu.org/licenses/>.
 */


