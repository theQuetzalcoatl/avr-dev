#include "adc.h"


/*
 * Good to knows about the peripheral:
 * ---------------------------------------
 * 10 bit, SAR conversion
 * adc clock: 125 kHz
 */


static int16_t amp_offset = 0;


/************** Device initialization **************/
static void set_adc_freq_to_125khz(void);
static adc_t acquire_amplifier_offset(int64_t *offset);

void adc_init_device(void)
{
    DDRF &= ~(1<<PINF0 | 1<<PINF1 | 1<<PINF2); // ADC gpios are inputs

    set_adc_freq_to_125khz();
    ADCSRA &= ~(1<<ADFR);   // single conversation mode
    ADMUX |= 1<<REFS0; ADMUX &= ~(1<<REFS1); // uC voltage supply as reference
    ADMUX &= ~(1<<ADLAR); // ADC result is left adjusted
    ADMUX &= ~(1<<MUX4 | 1<<MUX3 | 1<<MUX2 | 1<<MUX1 | 1<<MUX0); // adc0 channel is selected by default

    ADCSRA |= 1<<ADEN;      // enable adc
    acquire_amplifier_offset(&amp_offset);
}

static void set_adc_freq_to_125khz(void)
{
#if F_CPU == 16000000UL
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;
#else // assuming 8 MHz here...
    ADCSRA |= 1<<ADPS2 | 1<<ADPS1;
    ADCSRA &= ~(1<<ADPS0);
#endif
}

static adc_t acquire_amplifier_offset(int64_t *offset)
{
    adc_t tmp= 0;
    adc_change_channels(ADC1_ADC1_1x);
    _delay_us(500);
    adc_convert(&tmp);

    if(tmp & (1<<9)){
        tmp = ~tmp + 1; // two's complement 
        *offset = -1*tmp;
    }
    else{
        *offset = tmp;
    }
}


/************** Device functionalities **************/

k_error_t adc_convert(adc_t * const result)
{
    if(check_device_ownership(DEV_ADC) == SAME_OWNER){
        ADCSRA |= 1<<ADSC; // if 0, we start a conversion, if already 1 we soon get one
        while(ADCSRA & (1<<ADSC)){;}
        *result = (adc_t)ADCL;
        *result += (adc_t)ADCH<<8;
        *result += amp_offset; // NOTE: buggy, because in single channel it is irrelevant --> create channel type variable
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}

k_error_t adc_change_channels(uint8_t channel_conf)
{
    if(check_device_ownership(DEV_ADC) == SAME_OWNER){
        ADMUX &= 0b11100000;
        ADMUX |= channel_conf;
        return NO_ERROR;
    }
    else return K_ERR_INVALID_DEVICE_ACCESS;
}



/*
 * adc.c
 * This file is part of Aztec
 *
 * Copyright (C) 2021 - theQuetzalcoatl
 *
 * Aztec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Aztec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aztec. If not, see <http://www.gnu.org/licenses/>.
 */