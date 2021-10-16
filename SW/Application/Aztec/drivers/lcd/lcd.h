#ifndef LCD_H
#define LCD_H

#include "../../kernel/kernel.h"

    /* COMMANDS */
#define LCD_CLEAR       (0b00000001u)

extern device_id_t lcd_lease(void);
extern void lcd_release(void);
extern void lcd_init_device(void);
extern k_error_t lcd_send_command(device_id_t id, uint8_t command);
extern k_error_t lcd_write(device_id_t id, uint8_t data);
extern k_error_t lcd_turn_backligh_off(device_id_t id);
extern k_error_t lcd_turn_backligh_on(device_id_t id);

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