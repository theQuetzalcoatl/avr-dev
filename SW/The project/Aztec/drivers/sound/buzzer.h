#ifndef BUZZER_H
#define BUZZER_H

#include "../../kernel/kernel.h"

#define BUZZER_ERR_INVALID_REGISTER_VAL (1u)
#define BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST (2u)

extern void buzzer_init_device(void);
extern k_error_t buzzer_buzz(uint16_t frequency_Hz);
extern k_error_t buzzer_off(void);

#endif /* BUZZER_H */


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