#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H


    /* HW */
#define F_CPU (16000000UL) /* Hz */
#define T0 (0u)
#define T1 (1u)
#define T2 (2u)
#define T3 (3u)
#define TIMER_USED T2 /* NOTE: only T0 and T1 is implemented yet */

    /* THREADS */
#define CONFIG_MAX_THREADS (25u)
#define CONFIG_NUM_OF_THREADS (7u)

    /* STACKS */
#define CONFIG_MIN_STACK_SIZE (2u + 32u + 1u + 2u) /* required for context switching -> PC + R0->R31 + SREG + 2bytes stack overflow detection */ /* bytes */

    /* OTHER */
#define CONFIG_SYSTEM_TICK_IN_US (3500u)
#define CONFIG_K_LOG_ENTRY_NUM   (5u)
#define CONFIG_K_LOG_ENTRY_LENGTH_BYTES   (40u)


#endif /* KERNEL_CONFIG_H */

/*
 * kernel_config.h
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
