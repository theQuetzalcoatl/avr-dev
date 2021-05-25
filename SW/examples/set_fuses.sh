#!/bin/bash
avrdude_command="avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino";

${avrdude_command} -U hfuse:w:0x89:m; # CKOPT programmed to reach 16 MHz
${avrdude_command} -U lfuse:w:0xEF:m; # 16 MHz, Crystal Oscillator, fast rising power, 16K CK, 4,1 ms 
${avrdude_command} -U efuse:w:0xFF:m; # deisable ATmega103 compatibility mode
