#ifndef KERNEL_ERROR_HANDLING_H
#define KERNEL_ERROR_HANDLING_H

#define K_ERR_FAILED_INIT_SYSTICK   (2u)
#define K_ERR_STACK_OVERFLOW        (3u)
#define K_ERR_INVALID_STACKSIZE     (4u)
#define K_ERR_THREAD_NUM_OUT_OF_BOUNDS   (5u)
#define K_ERR_THREAD_NOT_FOUND      (6u)

extern void kernel_panic(void);

#endif /* KERNEL_ERROR_HANDLING_H */