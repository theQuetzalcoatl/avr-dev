#!/bin/bash

#set -x;

archive_filename="Aztec.a";

# ----- making a lib from the OS + drivers ----- #
function compile_OS()
{
	local c_files=$(find ./Aztec -type f -name "*.c");
  #local debug="-g1";
  local optimization="-O3";
  local uC="atmega128";
  local did_compile=0;

	for c_file in ${c_files}; do
		avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ ${optimization} ${debug} -c -o ${c_file/%.c/.o} ${c_file} || did_compile=$?;
	done

	local object_files=$(find ./Aztec -type f -name "*.o");

	rm ./Aztec/*.a;
	avr-ar -r -s -c ./Aztec/${archive_filename} ${object_files};
	find ./Aztec -type f -name "*.o" -delete;

  return ${did_compile};
}

function main()
{
	local output_bin="application.bin";
	local optimization="-O3";
	#local debug="-g1";
	local uC="atmega128";
	local warnings="-Wall -Wextra"
	export AZTEC_INCLUDE_PATH="$PWD/Aztec/kernel"; # for application shell scripts to use

	local did_compile=0;

  compile_OS || did_compile=$?;

	# ----- Compiling programs----- #

	for app_build_script in $(find ./Applications -type f -name build.sh); do
		bash ${app_build_script};
		if [[ $? != 0 ]] ; then did_compile=1; fi
	done

	avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ -I "${AZTEC_INCLUDE_PATH}" ${optimization} ${debug} -c -o main.o main.c || did_compile=$?;

	# ----- linking ----- #
	object_files=$(find . -type f -name "*.o");
	avr-gcc -mmcu=${uC} -o ${output_bin} ${object_files} ./Aztec/${archive_filename} || did_compile=$?;

	avr-objdump --source ${output_bin} > app_disass.s;

	avr-size --format=avr --mcu=${uC} ${output_bin} && \
	avr-objcopy --only-section=.text --only-section=.data --only-section=.bss --only-section=.bootloader --only-section=.noinit --output-target=ihex $output_bin application.hex

	# ----- invoking post processing scripts ----- #
	./ToolsAndScripts/header_guard_checker.sh ./Aztec/

	if [[ "$1" == "upload" && ${did_compile} == 0 ]]; then
		avrdude -p m128 -b 19200 -P /dev/ttyUSB0 -c arduino -U flash:w:application.hex:i;
	fi
}

main $1;
