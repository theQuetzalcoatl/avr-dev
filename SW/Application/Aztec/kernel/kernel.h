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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

        /* DRIVERS */
#include "../drivers/button/button.h"
#include "../drivers/buzzer/buzzer.h"
#include "../drivers/dac/dac.h"
#include "../drivers/keypad/keypad.h"
#include "../drivers/led/led.h"
#include "../drivers/uart/uart.h"
#include "../drivers/lcd/lcd.h"

#define HAL_OK (0u)
#define HAL_NOK (1u)

#define KERNEL_ENTER_ATOMIC() cli() 
#define KERNEL_EXIT_ATOMIC() sei()

typedef uint16_t StackSize;
typedef uint8_t Register;
typedef void (*ThreadAddress)(void);


    /* SYSTEM CALLS */

extern uint8_t kernel_register_thread(const ThreadAddress thread_addr, Register * const stack_start, const StackSize stack_size); // Registers one thread in the kernel. Must be called before OS starts
extern uint8_t kernel_start_os(void); // Last one to call in main, after all the thread registration
extern void kernel_exit(void);  // Threads can exit from being scheduled and run again.


#endif /* KERNEL_H */
