#include "led.h"
#include "../../kernel/kernel.h"

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
    PORTC &= ~(1<<LED1 | 1<<LED2 | 1<<LED3 | 1<<LED4);
}

/* Device functionalities */

k_error_t led_1_on(void)
{
    if(check_device_ownership(DEV_LED1) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED1;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_on(void)
{
    if(check_device_ownership(DEV_LED2) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED2;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_on(void)
{
    if(check_device_ownership(DEV_LED3) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED3;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_on(void)
{
    if(check_device_ownership(DEV_LED4) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED4;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_1_off(void)
{
    if(check_device_ownership(DEV_LED1) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED1);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_off(void)
{
    if(check_device_ownership(DEV_LED2) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED2);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_off(void)
{
    if(check_device_ownership(DEV_LED3) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED3);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_off(void)
{
    if(check_device_ownership(DEV_LED4) == SAME_OWNER){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED4);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_1_toggle(void)
{
    if(check_device_ownership(DEV_LED1) == SAME_OWNER){
        PORTC ^= 1<<LED1;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_toggle(void)
{
    if(check_device_ownership(DEV_LED2) == SAME_OWNER){
        PORTC ^= 1<<LED2;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_toggle(void)
{
    if(check_device_ownership(DEV_LED3) == SAME_OWNER){
        PORTC ^= 1<<LED3;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_toggle(void)
{
    if(check_device_ownership(DEV_LED4) == SAME_OWNER){
        PORTC ^= 1<<LED4;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}