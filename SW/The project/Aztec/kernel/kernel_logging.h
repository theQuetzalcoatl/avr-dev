#ifndef KERNEL_LOGGING_H
#define KERNEL_LOGGING_H

#include "kernel.h"

extern void klog_init(k_error_t (*rec_func)(const char *));
extern k_error_t klog_print(void); // 
extern void klog_log(const char *log_msg);



#endif /* KERNEL_LOGGING_H */