#include "avr/io.h"
#define F_CPU 8000000UL
#include "avr/delay.h"


/**************************************************
 * description:
 * 
 * After initialization all the possible 
 * transitions are covered.
 * When one turns the encoder clockwise, one of the
 * ldes are flickering, when turned counterclockwise
 * the other led flickers.
 * Note: There may be some hardware fault that can
 * cause the leds to behave strangely.
 **************************************************/

#define BOTH_HIGH   (0b11000000u)
#define BOTH_LOW    (0b00000000u)
#define ONLY_A_HIGH (0b01000000u)
#define ONLY_B_HIGH (0b10000000u)

int main()
{
	/* rotary pins init */
	DDRC &= ~(1<<PINC6 | 1<<PINC7);
	PORTC |= (1<<PINC6 | 1<<PINC7); /* activating pullup resistors */
	
	/* init leds */
	DDRC |= (1<<PINC2 | 1<<PINC3);
	
	unsigned char current_state = PINC & (1<<PINC7 | 1<<PINC6);
	unsigned char previous_state = PINC & (1<<PINC7 | 1<<PINC6);
	
	while(4){
		current_state = PINC & (1<<PINC7 | 1<<PINC6); 
		
		if(previous_state == current_state){
			;/* do nothing */		
		}
		else if( (previous_state == BOTH_LOW) && (current_state == ONLY_A_HIGH)  || /* 00 -> 01 */
						 (previous_state == BOTH_HIGH) && (current_state == ONLY_B_HIGH) || /* 11 -> 10 */
						 (previous_state == ONLY_A_HIGH) && (current_state == BOTH_HIGH) || /* 01 -> 11 */
						 (previous_state == ONLY_B_HIGH) && (current_state == BOTH_LOW)){   /* 01 -> 00 */
			PORTC ^= 1<<PINC2;
			previous_state = current_state;
		}
		else if( (previous_state == ONLY_B_HIGH) && (current_state == BOTH_HIGH) || /* 10 -> 11 */
				     (previous_state == ONLY_A_HIGH) && (current_state == BOTH_LOW)  || /* 01 -> 00 */
				     (previous_state == BOTH_HIGH) && (current_state == ONLY_A_HIGH) || /* 11 -> 01 */
				     (previous_state == BOTH_LOW) && (current_state == ONLY_B_HIGH)){   /* 00 -> 10 */
			PORTC ^= 1<<PINC3;
			previous_state = current_state;
		}
		_delay_ms(1);
	}
	
	while(1){;}

 return 0;
}

/*
 * rotary_encoder.c
 * This file is part of examples
 *
 * Copyright (C) 2020 - theQuetzalcoatl
 *
 * <program name> is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * <program name> is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with <program name>. If not, see <http://www.gnu.org/licenses/>.
 */

