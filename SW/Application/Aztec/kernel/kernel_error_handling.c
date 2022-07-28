#include "../kernel/kernel.h"

extern void disable_systick(void);

static void signal_morse_sos_forever();

void kernel_panic(void) __attribute__ ((weak));
void kernel_panic(void)
{
    disable_systick();
    signal_morse_sos_forever();
}

static void signal_morse_sos_forever()
{
    DDRC |= 1<<LED4;
    while(1){
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(100);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(300);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(100);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        _delay_ms(500);
    }
}

/*
 * error_handling.c
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