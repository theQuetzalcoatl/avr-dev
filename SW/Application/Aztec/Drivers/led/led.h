#ifndef LED_H
#define LED_H

#include "../../kernel/kernel.h"

#define LED1 PINC2
#define LED2 PINC3
#define LED3 PINC4
#define LED4 PINC5

extern void led_init_device(void);
extern void turn_led_on(uint8_t led);
extern void turn_led_off(uint8_t led);
extern void toggle_led(uint8_t led);

#endif /* LED_H */