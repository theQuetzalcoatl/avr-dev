#include "./button.h"

/**** DEIVCE ID ****/
static uint8_t leased = FALSE;

/* Device initialization */

static void set_gpio_to_input(void);
static void activate_pullup_resistors();
void button_init_device(void)
{
    set_gpio_to_input();
    activate_pullup_resistors();
}

static void set_gpio_to_input(void)
{
    DDRC &= ~(1<<BUTTON_2 | 1<<BUTTON_3);
}

static void activate_pullup_resistors()
{
    PORTC |= (1<<BUTTON_2 | 1<<BUTTON_3);
}


/* Device functionalities */

device_id_t button_lease(void)
{
    if(leased == FALSE){
        KERNEL_ENTER_ATOMIC();
        leased = TRUE;
        KERNEL_EXIT_ATOMIC();
        return BUTTON_ID;
    }
    else return INVALID_ID;
}

void button_release(void)
{
    leased = FALSE;
}

uint8_t button_get_state(uint8_t button)
{
    if(button == BUTTON_2 || button == BUTTON_3) return (PINC & (uint8_t)((1<<button))>>button);
    else return ERR_INVALID_BUTTON;
}

static void deactivate_pullup_resistors(void);
static void set_gpio_to_output_low(void);
k_error_t button_deactivate_buttons(device_id_t id)
{
    if(BUTTON_ID == id && leased == TRUE){
        deactivate_pullup_resistors();
        set_gpio_to_output_low();
        return NO_ERROR;
    }
    else K_ERR_INVALID_DEVICE_ACCESS;
}

static void deactivate_pullup_resistors(void)
{
    PORTC &= ~(1<<BUTTON_2 | 1<<BUTTON_3);
}

static void set_gpio_to_output_low(void)
{
    DDRC |= (1<<BUTTON_2 | 1<<BUTTON_3);
    PORTC &= ~(1<<BUTTON_2 | 1<<BUTTON_3);
}

k_error_t button_activate_buttons(device_id_t id)
{
    if(BUTTON_ID == id && leased == TRUE){
        set_gpio_to_input();
        activate_pullup_resistors();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

/*
 * button.c
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