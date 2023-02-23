#ifndef KERNEL_ERROR_HANDLING_H
#define KERNEL_ERROR_HANDLING_H

#include <stdint.h>

typedef uint8_t k_error_t;

enum KERNEL_ERRORS
{
    NO_ERROR = 0x80u,
    K_ERR_STACK_OVERFLOW,
    K_ERR_INVALID_STACKSIZE,
    K_ERR_THREAD_NUM_OUT_OF_BOUNDS,
    K_ERR_THREAD_NOT_FOUND,
    K_ERR_ALREADY_REGISTERED_STACK,
    K_ERR_INVALID_DEVICE_ACCESS,
    K_ERR_MULTIPLE_THREADS_IN_SYSTEM,
    K_ERR_NO_AVAILABLE_THREAD
};

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