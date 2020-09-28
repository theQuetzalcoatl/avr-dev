#include "avr/io.h"

/**************************************************
 * description:
 * 
 * Initializes T1 timer and its corresponding pin.
 * The output pin's' will be half of your clock frequency.
 * At the capacitor, half of the supply voltage should exist.
 **************************************************/

int main()
{
	TCCR1A &= ~(1<<COM1A1);
	TCCR1A |= (1<<COM1A0);
	TCCR1B |= (1<<WGM12 | 1<<CS10);
	DDRB |= 1<<PINB5;

	while(4){;}

 return 0;
}

/*
 * dac.c
 * This file is part of <program name>
 *
 * Copyright (C) 2020 - Dani
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


