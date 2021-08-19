#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

    /* HW */
#define F_CPU (16000000UL) /* Hz */
#define AVAILABLE_SRAM (4096u) /* bytes */

    /* THREADS */
#define MAX_THREADS (255u)
#define NUM_OF_THREADS (4u)

    /* STACKS */
#define MAX_STACK_SIZE ((AVAILABLE_SRAM*7)/10) /* ~70% of all internal RAM */
#define MIN_STACK_SIZE (2u + 32u + 1u + 2u) /* required for context switching -> PC + R0->R31 + SREG + 2bytes stack overflow detection */ /* bytes */

    /* OTHER */
#define SYSTEM_TICK_IN_US (4000u)

#endif /* KERNEL_CONFIG_H */