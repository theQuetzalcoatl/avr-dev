#!/bin/bash

#######################################################
## This script checks if found header files have 
## *proper* headerguard or not.
## proper = <HEADERFILE_NAME>_H (can be modified)
#######################################################

echo -e "--- HEADER FILE GUARD START ---\n\n";

header_files="";
if [ $# -eq 1 ]; then # abs or relative path is given
	header_files=$(find "$1" -type f -name "*.h");
elif [ $# -eq 0 ]; then # no path given, using relative path
	header_files=$(find . -type f -name "*.h");
else
	echo -e "Wrong number of arguments supplied.\nUsage: <script_name> <starting folder>(optional)"
	exit 1
fi

IFS=$'\n'; # changing the internal field seprator from '<space>\t\n' to only '\n' to catch the outputs from now on as lines

found_bad_files=0;
echo -e "Uncompliant files:"
for header_file in ${header_files}; do
  found_header=$(grep -P "[[:space:]]*#[[:space:]]*(ifndef|define)[[:space:]]+.*" "${header_file}");
  found_header=$(echo -e ${found_header} | xargs --null); # trimming leading, trailing and extra whitespaces for simpler checking later
  
  calculated_header=$(expr "${header_file}" : '.*\/\(.*\)'); # regex: file_path/(file_name)
  calculated_header=${calculated_header/%.h/_h}; # from .h to _h
  calculated_header=${calculated_header^^}; # all uppercase

  if [[ "${found_header}" != *"define ${calculated_header}"* || "${found_header}" != *"ifndef ${calculated_header}"* ]]; then 
    echo "*   ${header_file}";
    found_bad_files=$(( ++found_bad_files ));
  fi
done

echo -e "\n--- HEADER FILE GUARD END ---\n\n";

if [ $found_bad_files -eq 0 ]; then
  exit 0;
else 
  exit 1;
fi