#ifndef LCD_H
#define LCD_H

#include "../../kernel/kernel.h"

    /* COMMANDS */
#define LCD_CLEAR (uint8_t)(0b00000001u)

extern void lcd_init_device(void);
extern void lcd_send_command(uint8_t command);
extern void lcd_write(uint8_t data);
extern void lcd_turn_backligh_off(void);
extern void lcd_turn_backligh_on(void);

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