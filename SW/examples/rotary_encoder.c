#include "avr/io.h"
#define F_CPU 16000000UL
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

#define NO_ROTATION (0u)
#define CLOCKWISE (1u)
#define ANTI_CLOCKWISE (2u)
#define HW_FAULT (3u)


int main()
{
	/* rotary pins init */
	DDRC &= ~(1<<PINC6 | 1<<PINC7);
	PORTC |= (1<<PINC6 | 1<<PINC7); /* activating pullup resistors */
	
	DDRC |= (1<<PINC2 | 1<<PINC3); 	/* init leds */
	
	unsigned char current_state = PINC & 0b11000000;
	unsigned char previous_state = PINC & 0b11000000;
	
	unsigned char direction_of_rotation = NO_ROTATION;
	
	while(1){
		current_state = PINC & 0b11000000;
		
		if(current_state == previous_state) continue;
		else{
			switch(current_state){
				case BOTH_HIGH:
					if(previous_state == ONLY_A_HIGH) direction_of_rotation = ANTI_CLOCKWISE;
					else if(previous_state == ONLY_B_HIGH) direction_of_rotation = CLOCKWISE;
					else direction_of_rotation = HW_FAULT;
					break;
				case BOTH_LOW:
					if(previous_state == ONLY_A_HIGH) direction_of_rotation = CLOCKWISE;
					else if(previous_state == ONLY_B_HIGH) direction_of_rotation = ANTI_CLOCKWISE;
					else direction_of_rotation = HW_FAULT;
					break;
				case ONLY_A_HIGH:
					if(previous_state == BOTH_LOW) direction_of_rotation = ANTI_CLOCKWISE;
					else if(previous_state == BOTH_HIGH) direction_of_rotation = CLOCKWISE;
					else direction_of_rotation = HW_FAULT;
					break;
				case ONLY_B_HIGH:
					if(previous_state == BOTH_LOW) direction_of_rotation = CLOCKWISE;
					else if(previous_state == BOTH_HIGH) direction_of_rotation = ANTI_CLOCKWISE;
					else direction_of_rotation = HW_FAULT;
					break;
				break;
			}
			
			if(direction_of_rotation == CLOCKWISE) PORTC ^= 1<<PINC2; 
			else if(direction_of_rotation == ANTI_CLOCKWISE) PORTC ^= 1<<PINC3;
			else PORTC &= ~(1<<PINC2 | 1<<PINC1);
			previous_state = current_state; 
		}
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

