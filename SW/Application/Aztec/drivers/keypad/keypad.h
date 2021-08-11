#ifndef KEYPAD_H
#define KEYPAD_H

#include "../../kernel/kernel.h"


#define KEYPAD_0        (0u)
#define KEYPAD_1        (1u)
#define KEYPAD_2        (2u)
#define KEYPAD_3        (3u)
#define KEYPAD_4        (4u)
#define KEYPAD_5        (5u)
#define KEYPAD_6        (6u)
#define KEYPAD_7        (7u)
#define KEYPAD_8        (8u)
#define KEYPAD_9        (9u)
#define KEYPAD_ASTERISK (10u)
#define KEYPAD_POUND    (11u)

extern void keypad_init_device(void);
extern char keypad_get_pressed_key(void);

#endif /* KEYPAD_H */