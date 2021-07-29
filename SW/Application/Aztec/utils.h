#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define INITED (0u)


/* KERNEL ERRORS */
#define NO_ERROR (0u)
#define GENERAL_ERROR (1U)
#define ERR_FAILED_INIT_SYSTICK (2u)
#define ERR_STACK_OVERFLOW (4u)
#define ERR_INVALID_STACKSIZE (8u)

#define TRUE (1u)
#define FALSE (0u)

#define PIN_HIGH (1U)
#define PIN_LOW  (0U)

#endif /* UTILS_H */