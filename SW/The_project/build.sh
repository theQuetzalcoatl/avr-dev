#!/bin/bash

function main()
{
	local output_bin="application.bin";
	local optimization="-O3";
	local debug="-g1";
	local uC="atmega128";
	local warnings="-Wall -Wextra"
	export kernel_include_path="$PWD/Aztec/kernel"; # for application shell scripts to use

	local did_compile=0;

	# ----- making a lib from the OS + drivers ----- #
	local archive_filename="Aztec.a";

	local c_files=$(find ./Aztec -type f -name "*.c");

	for c_file in ${c_files}; do
		avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ ${optimization} ${debug} -c -o ${c_file/%.c/.o} ${c_file} || did_compile=$?;
	done

	local object_files=$(find ./Aztec -type f -name "*.o");

	rm ./Aztec/*.a;
	avr-ar -r -s -c ./Aztec/${archive_filename} ${object_files};
	find ./Aztec -type f -name "*.o" -exec rm {} \;

	# ----- Compiling programs----- #
	cd Applications;

	for app_dir in $(ls -d */); do
		bash "${app_dir}/build.sh";
		if [[ $? != 0 ]] ; then did_compile=1; fi
	done

	cd ..;

	avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ -I "${kernel_include_path}" ${optimization} ${debug} -c -o main.o main.c || did_compile=$?;

	# ----- linking ----- #
	object_files=$(find . -type f -name "*.o");
	avr-gcc -mmcu=${uC} -o ${output_bin} ${object_files} ./Aztec/${archive_filename} || did_compile=$?;

	avr-objdump --source ${output_bin} > app_disass.s;

	avr-size --format=avr --mcu=${uC} ${output_bin} && \
	avr-objcopy --only-section=.text --only-section=.data --only-section=.bss --only-section=.bootloader --only-section=.noinit --output-target=ihex $output_bin application.hex

	# ----- invoking post processing scripts ----- #
	./ToolsAndScripts/header_guard_checker.sh ./Aztec/

	find . -type f -name "*.o" -exec rm {} \;

	if [[ "$1" == "upload" && ${did_compile} == 0 ]]; then
		avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino -U flash:w:application.hex:i;
	fi
}

main $1;
