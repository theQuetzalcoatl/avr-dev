#!/bin/bash

output_bin="example.bin";
mcu="atmega128";

compiler_flags="-mmcu=${mcu} -Wall -Wextra -O2";

avr-gcc ${compiler_flags} -o $output_bin $1 && \
avr-size --format=avr --mcu=${mcu} ${output_bin} && \
avr-objcopy --only-section=.text --only-section=.data --output-target=ihex $output_bin example.hex && \
avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino -U flash:w:example.hex:i;

