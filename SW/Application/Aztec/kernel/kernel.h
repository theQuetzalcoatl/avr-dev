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
#include <avr/delay.h>

        /* DRIVERS */
#include "../Drivers/button/button.h"
#include "../Drivers/buzzer/buzzer.h"
#include "../Drivers/dac/dac.h"
#include "../Drivers/keypad/keypad.h"
#include "../Drivers/led/led.h"
#include "../Drivers/uart/uart.h"

#define HAL_OK (0u)
#define HAL_NOK (1u)

typedef uint16_t StackSize;
typedef uint8_t Register;
typedef void (*thread_address)(void);

typedef struct Thread
{
    Register *stack_pointer;
    Register *stack_bottom;
    uint8_t state;
    uint8_t remaining_wait_ticks;
    struct Thread *next;
}Thread;

#if WAY_TO_DO_ATOMIC == SIMPLE_ATOMIC /* For when we don't care about the previous state of the global interrupts */
    #define KERNEL_ENTER_ATOMIC() cli() 
    #define KERNEL_EXIT_ATOMIC() sei()
#elif WAY_TO_DO_ATOMIC == COMPLEX_ATOMIC /* For when we do care about the previous state of the global interrupts */ 
    #define KERNEL_ENTER_ATOMIC() Register __temp__ = SREG & 0x80; cli()
    #define KERNEL_EXIT_ATOMIC() SREG |= __temp__
#else
    #error "Some kind of atomic operation must be defined"
#endif


/* THREAD STATES */
#define RUNNING ('X')
#define WAITING ('W')
#define READY   ('R')
#define DELETED ('D')

extern uint8_t kernel_register_thread(thread_address addr, Register *stack_start, StackSize stack_size);
extern uint8_t kernel_init_os(void);


#endif /* KERNEL_H */
