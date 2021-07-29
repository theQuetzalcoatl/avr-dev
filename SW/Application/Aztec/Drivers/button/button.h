#ifndef BUTTON_H
#define BUTTON_H

#include "../../kernel/kernel.h"

#define BUTTON_2 PINC0
#define BUTTON_3 PINC1

#define PRESSED PIN_LOW
#define NOT_PRESSED PIN_HIGH
#define ERR_INVALID_BUTTON (0xffu)

extern void button_init_device(void);
extern uint8_t button_get_button_state(uint8_t button);
extern void button_deactive_buttons(void);
extern void button_activate_buttons(void);

#endif /* BUTTON_H */