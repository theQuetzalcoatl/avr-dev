#ifndef LED_H
#define LED_H

#include "../../kernel/kernel.h"

#define LED1 PINC2
#define LED2 PINC3
#define LED3 PINC4
#define LED4 PINC5


extern void led_init_device(void);
extern k_error_t led_1_on(void);
extern k_error_t led_2_on(void);
extern k_error_t led_3_on(void);
extern k_error_t led_4_on(void);
extern k_error_t led_1_off(void);
extern k_error_t led_2_off(void);
extern k_error_t led_3_off(void);
extern k_error_t led_4_off(void);
extern k_error_t led_1_toggle(void);
extern k_error_t led_2_toggle(void);
extern k_error_t led_3_toggle(void);
extern k_error_t led_4_toggle(void);

#endif /* LED_H */