#include "lcd.h"

#define LCD_D7		PINA7
#define LCD_D6		PINA6
#define LCD_D5		PINA5
#define LCD_D4		PINA4

#define RS			PINA1
#define EN         	PINA2
#define BACK_LIGHT 	PINA0
#define LCD_PORT   	PORTA

#define COMMAND_MODE()	LCD_PORT &= ~(1<<RS)
#define DATA_MODE()	    LCD_PORT |= (1<<RS)


#define FIRST_ROW_OFFSET  (0x00)
#define SECOND_ROW_OFFSET (0x40)

#define COLUMNS (16u)
#define ROWS 	(2u)


static void flash(void);
static void lcd_internal_send_command(uint8_t command);


static void init_gpio(void);
static void execute_lcd_init_sequence(void);
void lcd_init_device(void)
{
    init_gpio();
    execute_lcd_init_sequence();
}


static void init_gpio()
{
    DDRA = 0xff;
}


static void execute_lcd_init_sequence(void)
{
    COMMAND_MODE();
	LCD_PORT |= (1<<LCD_D5) | (1<<LCD_D4);
	LCD_PORT &= ~((1<<LCD_D6) | (1<<LCD_D7));	// 0b0011xxxx
	
	// 1)
	_delay_ms(20);
	flash();
	
	// 2)
	_delay_ms(5);
	flash();
	// 3)
	_delay_ms(1);
	flash();
	// 4)
	LCD_PORT &= ~(1<<LCD_D4);	// 0b0010xxxx 
	_delay_ms(5);
	flash();
	// 5)
	_delay_ms(5);

	lcd_internal_send_command(0b00101000);	// N=1(2 rows), F=0 (5*8 matrix)
	lcd_internal_send_command(0b00001000);	// display off, cursor&blinking off
	lcd_internal_send_command(LCD_CLEAR);	// clear screen, return cursor home
	lcd_internal_send_command(0b00000110);	// inc cursor right when writing, don't shift screen
	lcd_internal_send_command(0b00001100);	// display back on
}


static void flash(void)
{
	LCD_PORT |= (1<<EN);
	asm volatile("nop \n \
                nop \n \
                nop \n");
	LCD_PORT &= ~(1<<EN);
}


static void lcd_internal_send_command(uint8_t command)
{
	COMMAND_MODE();
	LCD_PORT &= 0x0f;
	LCD_PORT |= (command & 0xf0);
	flash();
	
	LCD_PORT &= 0x0f;
	LCD_PORT |= ((command<<4) & 0xf0);
	flash();
	
	_delay_ms(1);
}


k_error_t lcd_send_command(uint8_t command)
{
	if(check_device_ownership(DEV_LCD) == SAME_OWNER){
		COMMAND_MODE();
		LCD_PORT &= 0x0f;
		LCD_PORT |= (command & 0xf0);
		flash();
		
		LCD_PORT &= 0x0f;
		LCD_PORT |= ((command<<4) & 0xf0);
		flash();
		
		_delay_ms(1);

		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}


k_error_t lcd_turn_backligh_on(void)
{
	if(check_device_ownership(DEV_LCD) == SAME_OWNER){
		LCD_PORT |= 1<<BACK_LIGHT;
		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}


k_error_t lcd_turn_backligh_off(void)
{
	if(check_device_ownership(DEV_LCD) == SAME_OWNER){
		LCD_PORT &= ~(1<<BACK_LIGHT);
		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}


static void lcd_internal_write(uint8_t data)
{
	DATA_MODE();
	LCD_PORT &= 0x0f;
	LCD_PORT |= (data & 0xf0);
	flash();
	
	LCD_PORT &= 0x0f;
	LCD_PORT |= ((data<<4) & 0xf0);
	flash();
	
	_delay_us(50);
}


k_error_t lcd_print(const char *string)
{
	if(check_device_ownership(DEV_LCD) == SAME_OWNER){
		while(*string){
			lcd_internal_write(*string);
			++string;
		}
		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}


k_error_t lcd_move_cursor(uint8_t x, uint8_t y)
{
	if(check_device_ownership(DEV_LCD) == SAME_OWNER){
		if(x > COLUMNS || y > ROWS) return NO_ERROR;
		else{
			lcd_internal_send_command(0x80 + SECOND_ROW_OFFSET*(y-1) + (x-1));
			return NO_ERROR;
		}
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}


/*
 * lcd.c
 * This file is part of Aztec
 *
 * Copyright (C) 2021 - theQuetzalcoatl
 *
 * Aztec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Aztec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aztec. If not, see <http://www.gnu.org/licenses/>.
 */