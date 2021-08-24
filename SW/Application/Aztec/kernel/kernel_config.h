#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include "../utils.h"

    /* HW */
#define F_CPU (16000000UL) /* Hz */
#define AVAILABLE_SRAM (4096u) /* bytes */

    /* THREADS */
#define CONFIG_MAX_THREADS (255u)
#define CONFIG_NUM_OF_THREADS (4u)
#define CONFIG_THREADS_QUERY_STATE TRUE /* TRUE - every thread costs 2 additional bytes of RAM, but enables querying another thread's state */

    /* STACKS */
#define MAX_STACK_SIZE ((AVAILABLE_SRAM*7)/10) /* ~70% of all internal RAM */
#define MIN_STACK_SIZE (2u + 32u + 1u + 2u) /* required for context switching -> PC + R0->R31 + SREG + 2bytes stack overflow detection */ /* bytes */

    /* OTHER */
#define CONFIG_SYSTEM_TICK_IN_US (650u)



#endif /* KERNEL_CONFIG_H */