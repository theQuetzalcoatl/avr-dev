#!/bin/bash

function remove_binaries()
{
	for binary_file in $(find . -type f -name "*.o"); do
		rm $binary_file;
	done
}

output_bin="application.bin";
optimization="-O3";
debug="-g1";
uC="atmega128";
warnings="-Wall -Wextra"

did_compile=0;

# ----- making a lib from the OS ----- #
archive_filename="Aztec.a";

c_files=$(find ./Aztec -type f -name "*.c");
h_files=$(find ./Aztec -type f -name "*.h");

for c_file in ${c_files}; do
	avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ ${optimization} ${used_standard} ${debug} -c -o ${c_file/%.c/.o} ${c_file} || did_compile=$?;
done

object_files=$(find ./Aztec -type f -name "*.o");

rm ./Aztec/*.a;
avr-ar -r -s "./Aztec/${archive_filename}" ${object_files};

# ----- Application compilation ----- #
avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ ${optimization} ${used_standard} ${debug} -c -o main.o main.c || did_compile=$?;

# ----- linking ----- #
avr-gcc -mmcu=${uC} -o ${output_bin} main.o ./Aztec/${archive_filename} || did_compile=$?;

remove_binaries;

avr-objdump --source ${output_bin} > app_disass.s;

avr-size --format=avr --mcu=${uC} ${output_bin} && \
avr-objcopy --only-section=.text --only-section=.data --only-section=.bss --only-section=.bootloader --only-section=.noinit --output-target=ihex $output_bin application.hex


# ----- invoking post processing scripts ----- #
./ToolsAndScripts/header_guard_checker.sh ./Aztec/

if [[ "$1" == "upload" && ${did_compile} == 0 ]]; then
	avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino -U flash:w:application.hex:i;
fi