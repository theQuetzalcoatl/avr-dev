#!/bin/bash

output_bin="example.bin";

avr-gcc -mmcu=atmega128 -Wall -O2 -o $output_bin $1 && \
avr-size --format=avr --mcu=atmega128 example.bin && \
avr-objcopy --only-section=.text --only-section=.data --output-target=ihex $output_bin example.hex && \
avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino -U flash:w:example.hex:i;
