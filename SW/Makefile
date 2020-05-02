 ###########
## General ##
 ###########
HEX_FILE = Upload/Application.hex
ELF_FILE = Upload/Application.elf

START_TIME := $(shell date)
CURRENT_TIME = $(shell date)

 #########################
## AVRDude configuration ##
 #########################
BAUD_RATE = 19200
PROGRAMMER = -c arduino
PORT = -P /dev/ttyUSB0 # TODO: try to automatize it
FLASH = -U flash:w:$(HEX_FILE):i


 #############################
## avr-objcopy configuration ##
 #############################
MEMORY_SECTIONS = --only-section=.text --only-section=.data

#############################################################################
#############################################################################

 ##################
## compile the SW ##
 ##################
.PHONY: all

all: $(ELF_FILE)
$(ELF_FILE): $(OBJECT_FILES)
	# here goes something
	avr-size --format=avr --mcu=atmega128 $@

 ####################
## flash the binary ##
 ####################
.PHONY: upload

upload: $(HEX_FILE)
	avrdude -p m128 $(BAUD_RATE) $(PORT) $(PROGRAMMER) $(FLASH)
	
$(HEX_FILE): $(ELF_FILE)
	avr-objcopy $(MEMORY_SECTIONS) --output-target=ihex $< $@

 ########################################
## Cleaning up the whole SW environment ##
 ########################################
.PHONY: clean

clean:
 -rm $(OBJECT_FILES)# herecomes the system cleaning 
 # strategy for object files -> .INTERMEDIATE is a special target which prerequesits are files which are deleted after make exits.
