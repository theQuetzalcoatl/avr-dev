#ifndef KEYPAD_H
#define KEYPAD_H

#include "../../kernel/kernel.h"


#define KEYPAD_0        (0u)
#define KEYPAD_1        (1u)
#define KEYPAD_2        (2u)
#define KEYPAD_3        (3u)
#define KEYPAD_4        (4u)
#define KEYPAD_5        (5u)
#define KEYPAD_6        (6u)
#define KEYPAD_7        (7u)
#define KEYPAD_8        (8u)
#define KEYPAD_9        (9u)
#define KEYPAD_ASTERISK (10u)
#define KEYPAD_POUND    (11u)

extern void keypad_init_device(void);
extern char keypad_get_pressed_key(void);

#endif /* KEYPAD_H */

/*
 * keypad.h
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