#include "uart.h"

/* Device initialization */

static void set_single_speed(void);
static void set_double_speed(void);
static uint8_t set_baud_rate(const Uart * const uart);
static uint8_t set_parity(const uint8_t parity);
static uint8_t set_number_of_stop_bits(const uint8_t num_of_stop_bits);
static void enable_transmission(void);
uint8_t uart_init_device(const Uart * const uart)
{  
    uint8_t ret = 0;
    if(uart->mode == NORMAL_MODE) set_single_speed();
    else if(uart->mode == DOUBLE_SPEED_MODE) set_double_speed();
    else return ERR_UART_INVALID_MODE;

    ret = set_baud_rate(uart);
    if(ret != HAL_OK) return 1;

    ret = set_parity(uart->parity);
    if(ret != HAL_OK) return 2;

    ret = set_number_of_stop_bits(uart->num_of_stop_bits);
    if(ret != HAL_OK) return 3;

    enable_transmission();

    return NO_ERROR;

}

static void set_single_speed(void)
{
    UCSR0A &= ~(1<<U2X0);
}

static void set_double_speed(void)
{
    UCSR0A |= (1<<U2X0);
}

static uint8_t set_baud_rate(const Uart * const uart)
{
    uint16_t temp = 0;
    if(uart->mode == NORMAL_MODE) temp = (F_CPU*10)/(16*uart->baud_rate);
    else if(uart->mode == DOUBLE_SPEED_MODE) temp = (F_CPU*10)/(8*uart->baud_rate);
    else return ERR_UART_INVALID_MODE;

    temp += 5;
    temp /= 10;
    if(temp == 0) return ERR_UART_OUT_OF_LIMIT_BAUD_RATE;
    temp -= 1;

    UBRR0L = temp;
    UBRR0H = temp>>8;

    return HAL_OK;
}

static uint8_t set_parity(const uint8_t parity)
{
    switch(parity)
    {
        case NO_PARITY: UCSR0C &= ~(1<<UPM00 | 1<<UPM01);
         break;
        case EVEN_PARITY: UCSR0C &= ~(1<<UPM00); UCSR0C |= 1<<UPM01;
         break;
        case ODD_PARITY: UCSR0C |= (1<<UPM00 | 1<<UPM01);
         break;
        default:
            return ERR_UART_INVALID_PARITY;
    }

    return HAL_OK;
}

static uint8_t set_number_of_stop_bits(const uint8_t num_of_stop_bits)
{
    if(num_of_stop_bits == ONE_STOP_BIT) UCSR0C &= ~(1<<USBS0);
    else if(num_of_stop_bits == TWO_STOP_BITS) UCSR0C |= 1<<USBS0;
    else return ERR_UART_INVALID_STOP_BITS;
    return HAL_OK;
}

static void enable_transmission(void)
{
    UCSR0B |= 1<<TXEN;
}


/* Device functionalities */


void uart_send(char c)
{
    while ( !( UCSR0A & (1<<UDRE)) );
    UDR0 = c;
}

// send byte
// send ascii
// send string
// change uart settings --> wait for ongoing comm to end

/*
 * uart.c
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