#ifndef KERNEL_H
#define KERNEL_H

/*
AVR stuff:
    32x8bit general registers
    8 bit DATA BUS
    program memory from 0x0000 - 0xffff --> 16 bit ADDRESS BUS
    every program adress is 16 bits wide -> PC[15:0]
    internal SRAM from 0x0000-0x10ff -> SRAM ADDRESS = 13 bits -> STACK as well
    the SP is pointing before the top value of the stack

    PC on stack:
        0xff - PC[7:0]
        0xfe - PC[15:8]

    array in memory goes from lower to higher
*/

#include <stdint.h>

#include "../utils.h"
#include "../kernel/kernel_config.h"
#include "kernel_error_handling.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define KERNEL_VERSION "v7.1"

#define KERNEL_ENTER_ATOMIC() cli() 
#define KERNEL_EXIT_ATOMIC() sei()

/* THREAD STATES */
#define RUNNING ('X')
#define WAITING ('W')
#define READY   ('R')
#define DELETED ('D')

typedef uint16_t stack_size_t;
typedef uint8_t register_t;
typedef void (*thread_address_t)(void);

extern k_error_t start_os(void); // Last one to call in main, after all the thread registration
extern k_error_t register_thread(const thread_address_t thread_addr, register_t * const stack_start, const stack_size_t stack_size); // Registers one thread in the kernel. Must be called before OS starts

/*************************************************
 **             SYSTEM CALLS 
 ************************************************/

extern void exit_(void);  // Threads can exit from being scheduled and run again.
extern void wait_us(const uint32_t us);     // wait functions get less precise the shorter the requested wait. The number of active threads and a longer systick worsens accuracy
extern void wait_ms(const uint16_t ms);     // If there is a huge (~1min) delay and most of the threads exit during this time, the instance of the delay can become highly inaccurate
extern k_error_t release(const uint8_t requested_device);               // changes the device's state to NO_OWNER if caller thread owns the device, else K_ERR_INVALID_DEVICE_ACCESS
extern k_error_t lease(const uint8_t requested_device);                 // makes the device to be owned by the caller thread
extern uint8_t check_device_ownership(const uint8_t requested_device);  // NO_OWNER - device is free to take, SAME_OWNER - caller leased the device, DIFFERENT_OWNER - other thread leased device
extern uint8_t get_num_of_threads(void);    // provides the number of threads which are not in the DELETED state
extern void halt_system(void);              // halts the system indefenitely
extern void reboot(void);                   // acts like a hardware reset, except it does not reset the RAM, only the .bss and .data regions
#if CONFIG_THREADS_QUERY_STATE == TRUE
extern uint8_t get_thread_state(const thread_address_t th_addr); // provides the state of the supplied thread or K_ERR_THREAD_NOT_FOUND if an invalid ID was given.
#endif


/*************************************************
 **             DRIVERS
 ************************************************/

typedef uint8_t device_id_t; // valid IDs are stored in DEVICE_IDS enum
extern k_error_t register_device(void (*driver_func) (void), uint8_t dev); // Run the initialization function of a device, also the kernel registers it as well

#define NO_OWNER ((thread_t *)0)
#define SAME_OWNER      (1u)
#define DIFFERENT_OWNER (2u)

enum DEVICE_IDS
{
    DEV_BUTTON,
    DEV_LED1,
    DEV_LED2,
    DEV_LED3,
    DEV_LED4,
    DEV_LCD,
    DEV_UART,
    DEV_BUZZER,
    DEV_ADC,
    DEVICE_COUNT
};


#include "../drivers/button/button.h"
#include "../drivers/sound/buzzer.h"
#include "../drivers/dac/dac.h"
#include "../drivers/keypad/keypad.h"
#include "../drivers/led/led.h"
#include "../drivers/uart/uart.h"
#include "../drivers/lcd/lcd.h"
#include "../drivers/adc/adc.h"

#endif /* KERNEL_H */





/*
 * kernel.h
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