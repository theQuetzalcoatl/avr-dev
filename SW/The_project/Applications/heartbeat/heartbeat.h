#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include "kernel.h"

extern register_t heartbeat_stack[CONFIG_MIN_STACK_SIZE+10];
extern void main_heartbeat(void);

#endif /* HEARTBEAT_H */