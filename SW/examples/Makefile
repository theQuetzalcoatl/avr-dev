#AVRDude config
TARGET = 128
MCU = m$(TARGET)
BAUD_RATE = -b 19200
PROGRAMMER = -c arduino 
PORT = -P /dev/ttyUSB0
FLASH = -U flash:w:Application.hex:i

source = placeholder
 
 #################################
# this part will flash the binary #
 #################################
.PHONY: upload
upload: Application.hex
	avrdude -p $(MCU) $(BAUD_RATE) $(PORT) $(PROGRAMMER) $(FLASH)
	
Application.hex: Application.elf
	avr-objcopy --only-section=.text --only-section=.data --output-target=ihex $< $@
	
Application.elf: $(source)
	avr-gcc -mmcu=atmega$(TARGET) -O2 -o $@ $< -Wall
	avr-size --format=avr --mcu=atmega$(TARGET) $@
	
.PHONY: write_lfuse_bits
write_lfuse_bits:
	avrdude -p $(MCU) $(BAUD_RATE) $(PORT) $(PROGRAMMER) -U efuse:w:0xFF:m

 ######################################
# Cleaning up the whole SW environment #
 ######################################
 
.PHONY: clean
clean:
	-rm *.o *.elf *.hex *.i *.s

