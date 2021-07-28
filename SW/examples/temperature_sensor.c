#include "avr/io.h"

#define 	F_CPU   16000000UL
#include "avr/delay.h"
#include <stdlib.h>
#include <stdint.h>

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

#define CURSOR_TO_HOME (0b00000010u)

static void LCD_flash(void);
static void LCD_command(unsigned char command);
static void LCD_write(unsigned char data);


/**************************************************
 * description:
 * -----------------------------------------------
 * The LCD should display a value between 210-240
 * depending on the ambient temperature.
 **************************************************/

int main()
{
    /* INIT ADC */
	ADMUX |= 1<<REFS0; ADMUX &= ~(1<<REFS1); // selecting AVCC as reference voltage for the DAC
	ADCSRA |= 1<<ADFR; //activate free running mode
	ADMUX |= 1<<0; // selecting channel 0 for the input
	ADCSRA |= 0b00000111; // dividing F_CPU(16 MHz by 128 = 125000 Hz for the ADC
	ADCSRA |= 1<<ADEN; // enable ADC
	ADCSRA |= 1<<ADSC; // start free running

    /* INIT LCD */
	DDRA = 0xff;
	
	LCD_PORT |= 1<<BACK_LIGHT;
	
	COMMAND_MODE();
	LCD_PORT |= (1<<LCD_D5) | (1<<LCD_D4);
	LCD_PORT &= ~((1<<LCD_D6) | (1<<LCD_D7));	// 0b0011xxxx
	
	// 1)
	_delay_ms(20);// itt világít
	LCD_flash(); // ezután már nem
	
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

    char buffer[5] = {0};
    uint16_t result = 0;
    
	while(4){

    result = (unsigned short int)ADCL;
    result += (unsigned short int)ADCH<<8;

    itoa(result, buffer, 10);

    LCD_write(buffer[0]);
    LCD_write(buffer[1]);
    LCD_write(buffer[2]);
    _delay_ms(250);
    LCD_command(CURSOR_TO_HOME);
   }


	return 0;
}


static void LCD_flash(void)
{
	LCD_PORT |= (1<<EN);
	asm("nop \n \
       nop \n \
       nop \n \
       nop \n \
       nop \n \
       nop \n \
       nop \n \
       nop \n");
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
 * temperature_sensor.c
 * This file is part of examples
 *
 * Copyright (C) 2021 - theQuetzalcoatl
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


