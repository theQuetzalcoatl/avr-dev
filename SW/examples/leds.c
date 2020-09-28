#include "avr/io.h"

#define 	F_CPU   8000000UL
#include "avr/delay.h"

/**************************************************
 * description:
 * 
 * Initializes all the leds(D4-D7).
 * You should see this:
 *
 * #
 * ##
 * ###
 * ####
 * ###
 * ## 
 * #
 *
 * *repeat*
 **************************************************/

int main()
{
	DDRC |= 1<<PINC2 | 1<<PINC3 | 1<<PINC4 | 1<<PINC5;

	char leds = 0;
	leds |= 1<<PINC1;

	while(1)
	{
		for(char i = 0; i <= 3; ++i)
		{
			leds = leds<<1;
			PORTC |= leds;
			_delay_ms(250);
		}
		for(char i = 0; i <= 3; ++i)
		{
		
			PORTC &= ~leds;
			_delay_ms(250);
			leds = leds>>1;
		}
	}
	return 0;
}

/*
 * leds.c
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
 * along with examples. If not, see <http://www.gnu.org/licenses/>.
 */


