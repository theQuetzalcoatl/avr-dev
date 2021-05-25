#include "avr/io.h"

/**************************************************
 * description:
 * -------------------------------------
 * Covering the LDR should light up LED1,
 * uncovering it should turn it off.
 * The turnon/off treshold can be adjusted using
 * the pot. meter.
 *************************************************/
 
int main()
{
	DDRE &= ~(1<<PINE2 | 1<<PINE3);
	DDRC |= 1<<PINC2; /* LED1 ready */
	
	while(4){
		if(ACSR & 1<<ACO) PORTC &= ~(1<<PINC2);
		else PORTC |= 1<<PINC2;
	}

 return 0;
}

/*
 * comparator.c
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


