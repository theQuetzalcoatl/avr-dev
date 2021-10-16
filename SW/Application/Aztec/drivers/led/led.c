#include "led.h"
#include "../../kernel/kernel.h"

/**** DEIVCE ID ****/
static uint8_t leased_1 = FALSE;
static uint8_t leased_2 = FALSE;
static uint8_t leased_3 = FALSE;
static uint8_t leased_4 = FALSE;

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

device_id_t led_1_lease(void)
{
    if(leased_1 == FALSE){
        KERNEL_ENTER_ATOMIC();
        leased_1 = TRUE;
        KERNEL_EXIT_ATOMIC();
        return LED1_ID;
    }
    else return INVALID_ID;
}

void led_1_release(void)
{
    leased_1 = FALSE;
}

device_id_t led_2_lease(void)
{
    if(leased_2 == FALSE){
        leased_2 = TRUE;
        return LED2_ID;
    }
    else return INVALID_ID;
}

void led_2_release(void)
{
    leased_2 = FALSE;
}

device_id_t led_3_lease(void)
{
    if(leased_3 == FALSE){
        leased_3 = TRUE;
        return LED3_ID;
    }
    else return INVALID_ID;
}

void led_3_release(void)
{
    leased_3 = FALSE;
}

device_id_t led_4_lease(void)
{
    if(leased_4 == FALSE){
        leased_4 = TRUE;
        return LED4_ID;
    }
    else return INVALID_ID;
}

void led_4_release(void)
{
    leased_4 = FALSE;
}


k_error_t led_1_on(device_id_t id)
{
    if(id == LED1_ID && leased_1 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED1;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_on(device_id_t id)
{
    if(id == LED2_ID && leased_2 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED2;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_on(device_id_t id)
{
    if(id == LED3_ID && leased_3 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED3;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_on(device_id_t id)
{
    if(id == LED4_ID && leased_4 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC |= 1<<LED4;
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_1_off(device_id_t id)
{
    if(id == LED1_ID && leased_1 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED1);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_off(device_id_t id)
{
    if(id == LED2_ID && leased_2 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED2);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_off(device_id_t id)
{
    if(id == LED3_ID && leased_3 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED3);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_off(device_id_t id)
{
    if(id == LED4_ID && leased_4 == TRUE){
        KERNEL_ENTER_ATOMIC();
        PORTC &= ~(1<<LED4);  
        KERNEL_EXIT_ATOMIC();
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_1_toggle(device_id_t id)
{
    if(id == LED1_ID && leased_1 == TRUE){
        PORTC ^= 1<<LED1;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_2_toggle(device_id_t id)
{
    if(id == LED2_ID && leased_2 == TRUE){
        PORTC ^= 1<<LED2;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_3_toggle(device_id_t id)
{
    if(id == LED3_ID && leased_3 == TRUE){
        PORTC ^= 1<<LED3;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t led_4_toggle(device_id_t id)
{
    if(id == LED4_ID && leased_4 == TRUE){
        PORTC ^= 1<<LED4;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}