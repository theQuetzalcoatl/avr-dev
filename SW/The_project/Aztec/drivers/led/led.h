#ifndef LED_H
#define LED_H

#include "../../kernel/kernel.h"

#define LED1 PINC2
#define LED2 PINC3
#define LED3 PINC4
#define LED4 PINC5

/* board specific naming */
#define LED_D4 LED1
#define LED_D5 LED2
#define LED_D6 LED3
#define LED_D7 LED4

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
