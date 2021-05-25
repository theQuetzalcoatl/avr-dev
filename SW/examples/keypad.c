#include "avr/io.h"
#define F_CPU 16000000UL
#include "avr/delay.h"


/**************************************************
 * description:
 * 
 *  - push a key and the binary equivalent will be displayed by the LEDs
 * 1  | 2  | 3
 * 4  | 5  | 6
 * 7  | 8  | 9
 * 10 | 11 | 12
 **************************************************/

#define COL_2 PIND0 
#define ROW_1 PIND1
#define COL_1 PIND2
#define ROW_4 PIND3
#define COL_3 PIND4
#define ROW_3 PIND5
#define ROW_2 PIND6

unsigned char mirror_bits(unsigned char value) /* this is needed because currently the pinout to the leds are backwards */ 
{/* e.g. in: 0101 out: 1010 */
	unsigned char mirrored_value = 0u;
						
	mirrored_value += (value & 1<<3)>>3; // VAR[3] -> MIR[0]
	mirrored_value += (value & 1<<2)>>1; // VAR[2] -> MIR[1]
	mirrored_value += (value & 1<<1)<<1; // VAR[1] -> MIR[2]
	mirrored_value += (value & 1<<0)<<3; // VAR[0] -> MIR[3]
	
	return mirrored_value;
}

#define switch_to(column) PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3; PORTD &= ~(1<<column)
#define is_row_active(row) !(PIND & 1<<row)
#define display(value) PORTC |= value<<2

int main()
{
	DDRD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
	PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
	
	DDRD &= ~(1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2);
	PORTD |= 1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2; // activating pullups
	
	DDRC |= 1<<PINC2 | 1<<PINC3 | 1<<PINC4 | 1<<PINC5; // init leds
	
	while(4){
		switch_to(COL_1);
		_delay_us(5); // without delay, first row is not recognised due to parasitic capacitances and inductance which slows down the column switching
		if(is_row_active(ROW_1)) display(1);
		else if(is_row_active(ROW_2)) display(4);
		else if(is_row_active(ROW_3)) display(7);
		else if(is_row_active(ROW_4)) display(10);
		
		_delay_ms(1);
		
		switch_to(COL_2);
		_delay_us(5);
		if(is_row_active(ROW_1)) display(2);
		else if(is_row_active(ROW_2)) display(5);
		else if(is_row_active(ROW_3)) display(8);
		else if(is_row_active(ROW_4)) display(11);
		
		_delay_ms(1);
		
		switch_to(COL_3);
		_delay_us(5);
		if(is_row_active(ROW_1)) display(3);
		else if(is_row_active(ROW_2)) display(6);
		else if(is_row_active(ROW_3)) display(9);
		else if(is_row_active(ROW_4)) display(12);

		_delay_ms(1);
		PORTC = 0; // turn off leds
	}
	
	return 0;
}

/*
 * keypad.c
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


