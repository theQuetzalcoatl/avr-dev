
#ifndef ADC_H
#define ADC_H

#include "../../kernel/kernel.h"

typedef uint16_t adc_t;

extern void adc_init_device(void);
extern k_error_t adc_convert(adc_t * const result);
extern k_error_t change_channels(uint8_t channel_conf);

/******** signal configuration table ********/
#define ADC0            0b00000
#define ADC1            0b00001
#define ADC2            0b00010
#define ADC3            0b00011
#define ADC4            0b00100
#define ADC5            0b00101
#define ADC6            0b00110
#define ADC7            0b00111

#define ADC0_ADC0_10x   0b01000
#define ADC1_ADC0_10x   0b01001
#define ADC0_ADC0_200x  0b01010
#define ADC1_ADC0_200x  0b01011
#define ADC2_ADC2_10x   0b01100
#define ADC3_ADC2_10x   0b01101
#define ADC2_ADC2_200x  0b01110
#define ADC3_ADC2_200x  0b01111

#define ADC0_ADC1_1x    0b10000
#define ADC1_ADC1_1x    0b10001
#define ADC2_ADC1_1x    0b10010
#define ADC3_ADC1_1x    0b10011
#define ADC4_ADC1_1x    0b10100
#define ADC5_ADC1_1x    0b10101
#define ADC6_ADC1_1x    0b10110
#define ADC7_ADC1_1x    0b10111

#define ADC0_ADC2_1x    0b11000
#define ADC1_ADC2_1x    0b11001
#define ADC2_ADC2_1x    0b11010
#define ADC3_ADC2_1x    0b11011
#define ADC4_ADC2_1x    0b11100
#define ADC5_ADC2_1x    0b11101

#define INTERNAL_REF    0b11110
#define GND             0b11111

#endif /* ADC_H */


/*
 * adc.h
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