#include "lcd.h"

static uint8_t leased = FALSE;

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

static void flash(void);


device_id_t lcd_lease(void)
{
	if(leased == FALSE){
        KERNEL_ENTER_ATOMIC();
        leased = TRUE;
        KERNEL_EXIT_ATOMIC();
        return LCD_ID;
    }
    else return INVALID_ID;
}


void lcd_release(void)
{
	leased = FALSE;
}

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
	leased = TRUE;
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

	lcd_send_command(LCD_ID, 0b00101000);	// N=1(2 rows), F=0 (5*8 matrix)
	lcd_send_command(LCD_ID, 0b00001000);	// display off, cursor&blinking off
	lcd_send_command(LCD_ID, LCD_CLEAR);	// clear screen, return cursor home
	lcd_send_command(LCD_ID, 0b00000110);	// inc cursor right when writing, don't shift screen

	lcd_send_command(LCD_ID, 0b00001100);	// display back on

	leased = FALSE;
}

static void flash(void)
{
	LCD_PORT |= (1<<EN);
	asm volatile("nop \n \
                nop \n \
                nop \n");
	LCD_PORT &= ~(1<<EN);
}

k_error_t lcd_send_command(device_id_t id, uint8_t command)
{
	if(id == LCD_ID && leased == TRUE){
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

k_error_t lcd_write(device_id_t id, uint8_t data)
{
	if(id == LCD_ID && leased == TRUE){
		DATA_MODE();
		LCD_PORT &= 0x0f;
		LCD_PORT |= (data & 0xf0);
		flash();
		
		LCD_PORT &= 0x0f;
		LCD_PORT |= ((data<<4) & 0xf0);
		flash();
		
		_delay_us(50);

		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t lcd_turn_backligh_on(device_id_t id)
{
	if(id == LCD_ID && leased == TRUE){
		LCD_PORT |= 1<<BACK_LIGHT;
		return NO_ERROR;
	}
	else return K_ERR_INVALID_DEVICE_ACCESS;

}

k_error_t lcd_turn_backligh_off(device_id_t id)
{
	if(id == LCD_ID && leased == TRUE){
		LCD_PORT &= ~(1<<BACK_LIGHT);
		return NO_ERROR;
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