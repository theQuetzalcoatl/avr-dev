#ifndef BUTTON_H
#define BUTTON_H

#include "../../kernel/kernel.h"

#define BUTTON_2 PINC0
#define BUTTON_3 PINC1

#define PRESSED PIN_LOW
#define NOT_PRESSED PIN_HIGH
#define ERR_INVALID_BUTTON (0xffu)

extern device_id_t button_lease(void);
extern void button_release(void);
extern void button_init_device(void);
extern uint8_t button_get_state(uint8_t button);
extern k_error_t button_activate_buttons(device_id_t id);
extern k_error_t button_deactivate_buttons(device_id_t id);

#endif /* BUTTON_H */

/*
 * button.h
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