#ifndef KERNEL_ERROR_HANDLING_H
#define KERNEL_ERROR_HANDLING_H

#include <stdint.h>

typedef uint8_t k_error_t;

#define K_ERR_FAILED_INIT_SYSTICK       (2u | 0x80u)
#define K_ERR_STACK_OVERFLOW            (3u | 0x80u)
#define K_ERR_INVALID_STACKSIZE         (4u | 0x80u)
#define K_ERR_THREAD_NUM_OUT_OF_BOUNDS  (5u | 0x80u)
#define K_ERR_THREAD_NOT_FOUND          (6u | 0x80u)
#define K_ERR_ALREADY_REGISTERED_STACK  (7u | 0x80u)

#endif /* KERNEL_ERROR_HANDLING_H */

/*
 * error_handling.h
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