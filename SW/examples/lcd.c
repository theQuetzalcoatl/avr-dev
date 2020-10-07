#include "avr/io.h"

#define 	F_CPU   8000000UL
#include "avr/delay.h"

#define LCD_D7		PINA7
#define LCD_D6		PINA6
#define LCD_D5		PINA5
#define LCD_D4		PINA4

#define RS         PINA1
#define EN         PINA2
#define BACK_LIGHT PINA0
#define LCD_PORT   PORTA

#define COMMAND_MODE()	LCD_PORT &= ~(1<<RS)
#define DATA_MODE()	    LCD_PORT |= (1<<RS)

static void LCD_flash(void);
static void LCD_command(unsigned char command);
static void LCD_write(unsigned char data);

int main()
{

 /* Initialization */
	DDRA = 0xff;
	
	LCD_PORT |= 1<<BACK_LIGHT;
	
	COMMAND_MODE();
	LCD_PORT |= (1<<LCD_D5) | (1<<LCD_D4);
	LCD_PORT &= ~((1<<LCD_D6) | (1<<LCD_D7));	// 0b0011xxxx
	// 1)
	_delay_ms(20);
	LCD_flash();
	// 2)
	_delay_ms(5);
	LCD_flash();
	// 3)
	_delay_ms(1);
	LCD_flash();
	// 4)
	LCD_PORT &= ~(1<<LCD_D4);	// 0b0010xxxx 
	_delay_ms(5);
	LCD_flash();
	// 5)
	_delay_ms(5);
	LCD_command(0b00101000);	// N=1(2 rows), F=0 (5*8 matrix)
	LCD_command(0b00001000);	// display off, cursor&blinking off
	LCD_command(0b00000001);	// clear screen, return cursor home
	LCD_command(0b00000110);	// inc cursor right when writing, don't shift screen
	
	LCD_command(0b00001100);	// display back on
	
	/* Writing */
	
	LCD_write('S');
	LCD_write('u');
	LCD_write('c');
	LCD_write('c');
	LCD_write('e');
	LCD_write('s');
	LCD_write('s');
	LCD_write('!');
	
	while(4){;}
	
	return 0;
}

static void LCD_flash(void)
{
	LCD_PORT |= (1<<EN);
	asm("nop");
	asm("nop");
	asm("nop");		
	LCD_PORT &= ~(1<<EN);
}
	
static void LCD_command(unsigned char command)
{
	COMMAND_MODE();
	LCD_PORT &= 0x0f;
	LCD_PORT |= (command & 0xf0);
	LCD_flash();
	
	LCD_PORT &= 0x0f;
	LCD_PORT |= ((command<<4) & 0xf0);
	LCD_flash();
	
	_delay_ms(2);
}

static void LCD_write(unsigned char data)
	{
		DATA_MODE();
		LCD_PORT &= 0x0f;
		LCD_PORT |= (data & 0xf0);
		LCD_flash();
		
		LCD_PORT &= 0x0f;
		LCD_PORT |= ((data<<4) & 0xf0);
		LCD_flash();
		
		_delay_us(50);
	}

/*
 * lcd.c
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


