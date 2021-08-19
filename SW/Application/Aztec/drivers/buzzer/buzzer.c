#include "buzzer.h"

#define BUZZER_PIN (PINB4)

static uint16_t prescaler = 8;

/* Device initialization */

static void init_timer(void);
void buzzer_init_device(void)
{
    init_timer();
}

static void init_timer(void)
{
    /* Compare Match Output Mode */
    TCCR0 |= 1<<WGM01;
    TCCR0 &= ~(1<<WGM00);

    /* devide timer peripheral's clock by 256 */
    prescaler = 256u;
    TCCR0 |= 1<<CS01 | 1<<CS02;
    TCCR0 &= ~(1<<CS00);

    /* init GPIO */
    DDRB |= 1<<BUZZER_PIN;
    PORTB &= ~(1<<BUZZER_PIN); // when the buzzer is turned off it will pull the pin low
}

/* Device functionalities */

static uint8_t check_frequency_boundaries(const uint16_t frequency_Hz);
uint8_t buzzer_buzz(const uint16_t frequency_Hz)
{   
    uint8_t ret = check_frequency_boundaries(frequency_Hz);
    if(ret == BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST) return ret;

    /* set timer treshold */

    /* Fout = F_CPU/(2*PRESCALER(treshold_reg + 1)) */
    uint32_t val = 0;
    val = F_CPU/2;
    val /= prescaler;
    val /= frequency_Hz;
    val -= 1;
    if(val <= 255) OCR0 = val;
    else return BUZZER_ERR_INVALID_REGISTER_VAL;

    /* output toggle on Compare Match Output */
    TCCR0 |= 1<<COM00;
    TCCR0 &= ~(1<<COM01);

    return NO_ERROR;
}

static uint8_t check_frequency_boundaries(const uint16_t frequency_Hz)
{
    /* Fout = F_CPU/(2*PRESCALER(treshold_reg + 1)) */
    uint32_t upper_boundary = ~0;
    uint32_t lower_boundary = 0;

    upper_boundary = F_CPU/2;
    upper_boundary /= prescaler;

    lower_boundary = upper_boundary / 256u;

    if(frequency_Hz > upper_boundary || frequency_Hz < lower_boundary) return BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST;
    else return NO_ERROR;
}

void buzzer_off(void)
{
    TCCR0 &= ~(1<<COM01 | 1<<COM00);
}
