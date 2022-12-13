#ifndef UART_H
#define UART_H

#include "../../kernel/kernel.h"


typedef struct Uart
{
    uint8_t mode;
    uint8_t num_of_bits;
    uint8_t parity;
    uint8_t num_of_stop_bits;
    uint32_t baud_rate;
}Uart;


/* MODES */
#define UART_NORMAL_SPEED_MODE   (0u)
#define UART_DOUBLE_SPEED_MODE   (1u)

#define BIT_5               (2u)
#define BIT_6               (3u)
#define BIT_7               (4u)
#define BIT_8               (5u)
#define BIT_9               (6u)

#define UART_NO_PARITY      (7u)
#define UART_EVEN_PARITY    (8u)
#define UART_ODD_PARITY     (9u)

#define ONE_STOP_BIT        (10u)
#define TWO_STOP_BITS       (11u)


/* ERROR CODES */
#define ERR_UART_INVALID_MODE           (1u | 0x80)
#define ERR_UART_OUT_OF_LIMIT_BAUD_RATE (2u | 0x80)
#define ERR_UART_INVALID_PARITY         (3U | 0x80)
#define ERR_UART_INVALID_STOP_BITS      (4u | 0x80)
#define ERR_UART_INVALID_BITS           (5u | 0x80)

extern void uart_init_device(void);
extern k_error_t uart_set_baud_rate(const uint32_t baud_rate);
extern k_error_t uart_set_speed_mode(const uint8_t mode);
extern k_error_t uart_set_stop_bits(const uint8_t stop_bits);
extern k_error_t uart_set_data_bits(const uint8_t data_b);
extern k_error_t uart_putc(const char c);
extern k_error_t uart_puts(const char *s);

#endif /* UART_H */

/*
 * uart.h
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