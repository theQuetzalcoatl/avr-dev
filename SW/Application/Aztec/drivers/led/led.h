#ifndef LED_H
#define LED_H

#include "../../kernel/kernel.h"

#define LED1 PINC2
#define LED2 PINC3
#define LED3 PINC4
#define LED4 PINC5


extern void led_init_device(void);
extern device_id_t led_1_lease(void);
extern device_id_t led_2_lease(void);
extern device_id_t led_3_lease(void);
extern device_id_t led_4_lease(void);
extern void led_1_release(void);
extern void led_2_release(void);
extern void led_3_release(void);
extern void led_4_release(void);
extern k_error_t led_1_on(device_id_t id);
extern k_error_t led_2_on(device_id_t id);
extern k_error_t led_3_on(device_id_t id);
extern k_error_t led_4_on(device_id_t id);
extern k_error_t led_1_off(device_id_t id);
extern k_error_t led_2_off(device_id_t id);
extern k_error_t led_3_off(device_id_t id);
extern k_error_t led_4_off(device_id_t id);
extern k_error_t led_1_toggle(device_id_t id);
extern k_error_t led_2_toggle(device_id_t id);
extern k_error_t led_3_toggle(device_id_t id);
extern k_error_t led_4_toggle(device_id_t id);

#endif /* LED_H */