#!/bin/bash

function main()
{
	local output_bin="application.bin";
	local optimization="-O3";
	local debug="-g1";
	local uC="atmega128";
	local warnings="-Wall -Wextra"

	local did_compile=0;

	local c_files=$(find . -type f -name "*.c");

	for c_file in ${c_files}; do
		avr-gcc -mmcu=${uC} ${warnings} -I /usr/lib/avr/include/ -I "${kernel_include_path}" ${optimization} ${used_standard} ${debug} -c -o ${c_file/%.c/.o} ${c_file} || did_compile=$?;
	done

    exit ${did_compile};
}

main