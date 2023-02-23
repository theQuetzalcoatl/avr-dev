#ifndef SONGS_H
#define SONGS_H

#include "kernel.h"

extern register_t song_stack[CONFIG_MIN_STACK_SIZE + 20];
extern void imperial_march(void);
extern void gta_sa_theme(void);
extern void tetris(void);

#endif /* SONGS_H */