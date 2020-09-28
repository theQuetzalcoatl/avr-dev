#############################################
## General
#############################################
hex_file = Binaries/Application.hex
elf_file = Binaries/Application.elf
object_files = Binaries/main.o

start_time := $(shell date)
current_time = $(shell date)

#############################################
## compiler configuaration
#############################################
compiler = avr-gcc
target_mcu = -mmcu=atmega128
optimization = -O0
warnings = -Wall

#############################################
## AVRDude configuration 
#############################################
baud_rate = -b 19200
programmer = -c arduino
port = -P /dev/ttyUSB0 # TODO: try to automate it
flash = -U flash:w:$(hex_file):i

#############################################
## avr-objcopy configuration
#############################################
memory_sections = --only-section=.text --only-section=.data

#############################################
## compile the SW
#############################################
$(hex_file): $(elf_file)
	avr-objcopy $(memory_sections) --output-target=ihex $< $@

$(elf_file): $(object_files)
	$(compiler) $(object_files) -o $@ 
	# here definitely should go some 'find' command magic
	avr-size --format=avr --mcu=atmega128 $@
	
Binaries/main.o: main.c
	$(compiler) $(warnings) $(optimization) $(target_mcu) -o $@ -c $^

#############################################
## flash the binary 
#############################################
.PHONY: upload
upload: $(hex_file)
	avrdude -p m128 $(baud_rate) $(port) $(programmer) $(flash)

#############################################
## Cleaning up the whole SW environment
#############################################
.PHONY: clean
clean:
 -rm $(OBJECT_FILES)# herecomes the system cleaning 
 # strategy for object files -> .INTERMEDIATE is a special target which prerequesits are files which are deleted after make exits.
