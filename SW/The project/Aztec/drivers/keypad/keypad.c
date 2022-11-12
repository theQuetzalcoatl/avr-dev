#include "keypad.h"

#define COL_2 PIND0 
#define ROW_1 PIND1
#define COL_1 PIND2
#define ROW_4 PIND3
#define COL_3 PIND4
#define ROW_3 PIND5
#define ROW_2 PIND6

#define SWITCH_TO(column) PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3; PORTD &= ~(1<<column)
#define IS_ROW_ACTIVE(row) !(PIND & 1<<row)

/* Device initialization */

static void init_rows_gpio(void);
static void init_columns_gpio(void);
void keypad_init_device(void)
{
    init_columns_gpio();
    init_rows_gpio();
}

static void init_rows_gpio(void)
{
    DDRD &= ~(1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2);
	PORTD |= 1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2; // activating pullups
}

static void init_columns_gpio(void)
{
    DDRD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
	PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
}


/* Device functionalities */

static void switch_to_column(uint8_t column);
static uint8_t is_row_active(uint8_t row);
char keypad_get_pressed_key(void)
{
    while(1){    
        switch_to_column(COL_1);
        _delay_us(5); // without delay, first row is not recognised due to parasitic capacitances and inductance which slows down the column switching
        if(is_row_active(ROW_1)) return KEYPAD_1;
        else if(is_row_active(ROW_2)) return KEYPAD_4;
        else if(is_row_active(ROW_3)) return KEYPAD_7;
        else if(is_row_active(ROW_4)) return KEYPAD_ASTERISK;
        
        _delay_us(100);
        
        switch_to_column(COL_2);
        _delay_us(5);
        if(is_row_active(ROW_1)) return KEYPAD_2;
        else if(is_row_active(ROW_2)) return KEYPAD_5;
        else if(is_row_active(ROW_3)) return KEYPAD_8;
        else if(is_row_active(ROW_4)) return KEYPAD_0;
        
        _delay_us(100);
        
        switch_to_column(COL_3);
        _delay_us(5);
        if(is_row_active(ROW_1)) return KEYPAD_3;
        else if(is_row_active(ROW_2)) return KEYPAD_6;
        else if(is_row_active(ROW_3)) return KEYPAD_9;
        else if(is_row_active(ROW_4)) return KEYPAD_POUND;

        _delay_us(100);
    }
}


char keypad_get_pressed_key_nonblocking(void)
{
        switch_to_column(COL_1);
        _delay_us(5); // without delay, first row is not recognised due to parasitic capacitances and inductance which slows down the column switching
        if(is_row_active(ROW_1)) return KEYPAD_1;
        else if(is_row_active(ROW_2)) return KEYPAD_4;
        else if(is_row_active(ROW_3)) return KEYPAD_7;
        else if(is_row_active(ROW_4)) return KEYPAD_ASTERISK;
        
        _delay_us(100);
        
        switch_to_column(COL_2);
        _delay_us(5);
        if(is_row_active(ROW_1)) return KEYPAD_2;
        else if(is_row_active(ROW_2)) return KEYPAD_5;
        else if(is_row_active(ROW_3)) return KEYPAD_8;
        else if(is_row_active(ROW_4)) return KEYPAD_0;
        
        _delay_us(100);
        
        switch_to_column(COL_3);
        _delay_us(5);
        if(is_row_active(ROW_1)) return KEYPAD_3;
        else if(is_row_active(ROW_2)) return KEYPAD_6;
        else if(is_row_active(ROW_3)) return KEYPAD_9;
        else if(is_row_active(ROW_4)) return KEYPAD_POUND;

        _delay_us(25);

        return KEYPAD_NO_NUM;
}


static void switch_to_column(uint8_t column)
{
    PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
    PORTD &= ~(1<<column);
}


static uint8_t is_row_active(uint8_t row)
{
    return !(PIND & (1<<row));
}


/*
 * keypad.c
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