#include "uart.h"

typedef struct uart_t
{
    uint8_t mode;
    uint8_t data_bits;
    uint8_t parity;
    uint8_t stop_bits;
    uint32_t baud_rate;
}uart_t;

static uart_t uart;

#define NO_ONGOING_TX() (UCSR0A & (1<<UDRE))
#define NO_ONGOING_RX() !(UCSR0A & (1<<RXC0))

#define MAX_BAUDRATE_REG_VAL (832u)
#define MAX_BAUDREATE (2000000UL)
#define MIN_BAUDRATE (2400u)


/* Device initialization */

void uart_init_device(void)
{  
    UBRR0L = 103; // baud rate 9600

    UCSR0A &= ~(1<<U2X0); // normal speed

    UCSR0C &= ~(1<<UMSEL0); // Asynchronous mode
    UCSR0C &= ~(1<<UPM00 | 1<<UPM01); // no parity
    UCSR0C &= ~(1<<USBS0); // 1 bit STOP signal
    UCSR0C &= ~(1<<UCSZ02); UCSR0C |= (1<<UCSZ00 | 1<<UCSZ01); // data is 8 bits

    UCSR0B |= 1<<TXEN | 1<<RXEN; // enable transmission and receiving

    uart = (uart_t){.baud_rate=9600, .data_bits=BIT_8, .mode=UART_NORMAL_SPEED_MODE, .parity=UART_NO_PARITY, .stop_bits=ONE_STOP_BIT};
}


/* Device functionalities */

static void wait_till_changing_is_safe(void);

k_error_t uart_set_baud_rate(const uint32_t baud_rate)
{
    if(check_device_ownership(DEV_UART) == SAME_OWNER){
        if(baud_rate > MIN_BAUDRATE && baud_rate < MAX_BAUDREATE){
            wait_till_changing_is_safe();

            uint16_t temp = (F_CPU*10)/(16*baud_rate);

            temp += 5; // +0.5 rounding
            temp /= 10;
            if((temp == 0) || (temp-1 > MAX_BAUDRATE_REG_VAL)) return ERR_UART_OUT_OF_LIMIT_BAUD_RATE;
            temp -= 1;

            UBRR0L = temp;
            UBRR0H = temp>>8;
            uart.baud_rate = baud_rate;
        }
        else return ERR_UART_OUT_OF_LIMIT_BAUD_RATE;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;

    return NO_ERROR;
}

k_error_t uart_set_speed_mode(const uint8_t mode)
{
    if( check_device_ownership(DEV_UART) == SAME_OWNER ){
        if(mode != uart.mode){
            wait_till_changing_is_safe();

            if(mode == UART_NORMAL_SPEED_MODE) UCSR0A &= ~(1<<U2X0);
            else if(mode == UART_DOUBLE_SPEED_MODE) UCSR0A |= (1<<U2X0);
            else return ERR_UART_INVALID_MODE;
            uart.mode = mode;
        }
        else return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;

    return NO_ERROR;
}

k_error_t uart_set_parity(const uint8_t parity)
{
    if( check_device_ownership(DEV_UART) == SAME_OWNER ){
        switch(parity)
        {
            case UART_NO_PARITY: UCSR0C &= ~(1<<UPM00 | 1<<UPM01);
                break;
            case UART_EVEN_PARITY: UCSR0C &= ~(1<<UPM00); UCSR0C |= 1<<UPM01;
                break;
            case UART_ODD_PARITY: UCSR0C |= (1<<UPM00 | 1<<UPM01);
                break;
            default:
                return ERR_UART_INVALID_PARITY;
        }
        uart.parity = parity;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;

    return HAL_OK;
}

k_error_t uart_set_stop_bits(const uint8_t stop_bits)
{
    if( check_device_ownership(DEV_UART) == SAME_OWNER ){
        wait_till_changing_is_safe();
        if(stop_bits == ONE_STOP_BIT) UCSR0C &= ~(1<<USBS0);
        else if(stop_bits == TWO_STOP_BITS) UCSR0C |= 1<<USBS0;
        else return ERR_UART_INVALID_STOP_BITS;
        uart.stop_bits = stop_bits;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;

    return HAL_OK;
}

k_error_t uart_set_data_bits(uint8_t data_b)
{
    if( check_device_ownership(DEV_UART) == SAME_OWNER ){
        wait_till_changing_is_safe();
        switch(data_b)
        {
            case BIT_5:
                UCSR0C &= ~(1<<UCSZ00 | 1<<UCSZ01 | 1<<UCSZ02);
                break;

            case BIT_6:
                UCSR0C &= ~(1<<UCSZ01 | 1<<UCSZ02);
                UCSR0C |= 1<<UCSZ00;
                break;

            case BIT_7:
                UCSR0C &= ~(1<<UCSZ00 | 1<<UCSZ02);
                UCSR0C |= 1<<UCSZ01;
                break;

            case BIT_8:
                UCSR0C &= ~(1<<UCSZ02);
                UCSR0C |= 1<<UCSZ00 | 1<<UCSZ01;
                break;

            case BIT_9:
                UCSR0C |= 1<<UCSZ00 | 1<<UCSZ01 | 1<<UCSZ02;
                break;

            default:
                return ERR_UART_INVALID_BITS;
        }
        uart.data_bits = data_b;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;

    return NO_ERROR;
}


k_error_t uart_putc(const char c)
{
    if( check_device_ownership(DEV_UART) == SAME_OWNER ){
        while ( !( UCSR0A & (1<<UDRE0)) );
        UDR0 = c;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

/*
char uart_getc(void)
{
    // TODO
}*/

k_error_t uart_puts(const char *s)
{
    if(check_device_ownership(DEV_UART) == SAME_OWNER){
        while(*s){
            uart_putc(*s);
            ++s;
        }
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

static void wait_till_changing_is_safe(void)
{
    // UDRE, RXC datasheet USART Initialization
    while( NO_ONGOING_RX() && NO_ONGOING_TX() );
}



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