#include "avr/io.h"

/**************************************************
 * description:
 * 
 * Initializes the UART peripheral.
 * Constantly send the 'U' character so it makes a
 * 4800 Hz square waveform which can be measured
 * with a DMM or an oscilloscope.
 * The example assume 8MHz system clock frequency.
 **************************************************/

int main()
{

	UBRR0H = 0u;
	UBRR0L = 51u; // 9600 baud rate
	
	UCSR0B |= 1<<TXEN;
	
	UCSR0C &= ~(1<<UMSEL0 | 1<<UPM00 | 1<<UPM01 | 1<<USBS0 );
	UCSR0C |= (1<<UCSZ00 | 1<<UCSZ01);
	UCSR0B &= ~(1<<UCSZ02);

	UDR0 = 0x55;

	while(4)
	{
		while((UCSR0A & (1<<UDRE0)))
		{
			UDR0 = 0x55;
		}
	}

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

