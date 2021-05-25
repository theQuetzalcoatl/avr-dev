#include "avr/io.h"

#define 	F_CPU   16000000UL
#include "avr/delay.h"

/**************************************************
 * description:
 * 
 * Initializes the buzzer GPIO. 
 * Then constantly emits a 500 Hz tone.
 * Change F_CPU to your clock frequency.
 **************************************************/

int main()
{
	DDRB |= 1<<PINB4;
	
	while(4u)
	{
		PORTB ^= 1<<PINB4;
		_delay_ms(1);
	}
	
	return 0;
}

/*
 * buzzer.c
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

	
