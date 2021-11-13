#include "buzzer.h"

#define BUZZER_PIN (PINB4)

static uint16_t prescaler = 8;

/* Device initialization */

static void init_timer(void);
void buzzer_init_device(void)
{
    init_timer();
}

static void init_timer(void)
{
    /* Compare Match Output Mode */
    TCCR0 |= 1<<WGM01;
    TCCR0 &= ~(1<<WGM00);

    /* devide timer peripheral's clock by 256 */
    prescaler = 256u;
    TCCR0 |= 1<<CS01 | 1<<CS02;
    TCCR0 &= ~(1<<CS00);

    /* init GPIO */
    DDRB |= 1<<BUZZER_PIN;
    PORTB &= ~(1<<BUZZER_PIN); // when the buzzer is turned off it will pull the pin low
}

/* Device functionalities */

static uint8_t check_frequency_boundaries(const uint16_t frequency_Hz);
k_error_t buzzer_buzz(const uint16_t frequency_Hz)
{   
    if(kernel_check_device_ownership(DEV_BUZZER) == SAME_OWNER){

        uint8_t ret = check_frequency_boundaries(frequency_Hz);
        if(ret == BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST) return ret;

        /* set timer treshold */

        /* Fout = F_CPU/(2*PRESCALER(treshold_reg + 1)) */
        uint32_t val = 0;
        val = F_CPU/2;
        val /= prescaler;
        val /= frequency_Hz;
        val -= 1;
        if(val <= 255) OCR0 = val;
        else return BUZZER_ERR_INVALID_REGISTER_VAL;

        /* output toggle on Compare Match Output */
        TCCR0 |= 1<<COM00;
        TCCR0 &= ~(1<<COM01);

        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

static uint8_t check_frequency_boundaries(const uint16_t frequency_Hz)
{
    /* Fout = F_CPU/(2*PRESCALER(treshold_reg + 1)) */
    uint32_t upper_boundary = ~0;
    uint32_t lower_boundary = 0;

    upper_boundary = F_CPU/2;
    upper_boundary /= prescaler;

    lower_boundary = upper_boundary / 256u;

    if(frequency_Hz > upper_boundary || frequency_Hz < lower_boundary) return BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST;
    else return NO_ERROR;
}

k_error_t buzzer_off(void)
{
    if(kernel_check_device_ownership(DEV_BUZZER) == SAME_OWNER){
        TCCR0 &= ~(1<<COM01 | 1<<COM00);
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}


/*
 * buzzer.c
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
