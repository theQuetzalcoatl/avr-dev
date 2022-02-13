#ifndef LCD_H
#define LCD_H

#include "../../kernel/kernel.h"

    /* COMMANDS */
#define LCD_CLEAR       (0b00000001u)

extern void lcd_init_device(void);
extern k_error_t lcd_send_command(uint8_t command);
extern k_error_t lcd_turn_backligh_off(void);
extern k_error_t lcd_turn_backligh_on(void);
extern k_error_t lcd_print(const char *string);
extern k_error_t lcd_move_cursor(uint8_t x, uint8_t y); // first row is 1, so is first column

#endif  /* LCD_H */

/*
 * lcd.h
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