#include "buzzer.h"

/* Device initialization */

static void set_gpio_to_output(void);
void buzzer_init_device(void)
{
    set_gpio_to_output();
    PORTB &= ~(1<<PINB4); /* turn buzzer off */
}

static void set_gpio_to_output(void)
{
    DDRB |= 1<<PINB4;
}

/* Device functionalities */

void device_buzzer_buzz(uint16_t frequency, uint16_t on_time_ms)
{
    /* DISABLE INTERRUPTS */
    



    /* ENABLE INTERRUPTS */
}