#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#define F_CPU (16000000UL) /* Hz */

#define SIMPLE_ATOMIC 0u
#define COMPLEX_ATOMIC  1u
#define WAY_TO_DO_ATOMIC SIMPLE_ATOMIC

#define MAX_THREADS (255u)
#define NUM_OF_THREADS (3u)

#define AVAILABLE_SRAM (4096u) /* bytes */
#define MAX_STACK_SIZE ((AVAILABLE_SRAM*7)/10) /* ~70% of all internal RAM */
#define MIN_STACK_SIZE (2u + 32u + 1u + 1u  + 2u) /* required for context switching -> PC + R0->R31 + RAMPZ + SREG  + 2bytes stack overflow detection */

#define SYSTEM_TICK_IN_MS (1u)

#endif /* KERNEL_CONFIG_H */