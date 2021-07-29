#include "led.h"

/* Device initialization */

static void set_gpio_to_output(void);
static void turn_all_leds_off(void);
void led_init_device(void)
{
    set_gpio_to_output();
    turn_all_leds_off();
}

static void set_gpio_to_output(void)
{
    DDRC |= 1<<LED1 | 1<<LED2 | 1<<LED3 | 1<<LED4;
}

static void turn_all_leds_off(void)
{
    PORTC &= ~(1<<LED1 | 1<<LED2 |1<<LED3 |1<<LED4);
}

/* Device functionalities */

void turn_led_on(uint8_t led)
{
    
    if(led == LED1 || led == LED2 || led == LED3 || led == LED4){
        cli();
        PORTC |= 1<<led;
        sei();
    } 
    
}

void turn_led_off(uint8_t led)
{
    if(led == LED1 || led == LED2 || led == LED3 || led == LED4){
        cli();
        PORTC &= ~(1<<led);  
        sei();
    } 
}

void toggle_led(uint8_t led)
{
    //cli();
    if(led == LED1 || led == LED2 || led == LED3 || led == LED4) PORTC ^= 1<<led;
    //sei();
}
