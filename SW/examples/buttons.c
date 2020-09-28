#include "avr/io.h"

/**************************************************
 * description:
 * 
 * Activates the two push buttons, SW2 and SW3.
 * Activates leftmost led(D4).
 * If either or both of them are pushed the led 
 * should light up.
 **************************************************/

int main()
{
	DDRC |= 1<<PINC2;
	DDRC &= ~(1<<PINC0 | 1<<PINC1);
	
	/* enabling pullups */
	PORTC |= (1<<PINC0 | 1<<PINC1);
	
	if( (PINC & (1<<PINC0)) == 0 || (PINC & (1<<PINC1)) == 0) PORTC |= 1<<PINC2;
	else PORTC &= ~(1<<PINC2);
	
	return 0;
}

/*
 * buttons.c
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


