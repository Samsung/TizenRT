#!/bin/bash
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
# File 	 : g_var_profiler.sh
# Description : parse the global variable which size is greater than <SIZE>

ref_size=$1
path=$2
output_file=$3
WD=`pwd`
prefix=`echo ${PWD%/*/*}`

if [ "$1" == "--help" ]
then
	echo "Usage: $0 [SIZE] [ELF_PATH] [OUTPUT_FILENAME]"
	echo "This script parses the global variable which size is greater than [SIZE]"
	echo -e "\tIf no input params are specified, below is assumed"
	echo -e "\tSIZE : 256"
	echo -e "\tELF_PATH : ../../build/output/bin/tinyara"
	echo -e "\tOUTPUT_FILENAME : var_list_over_SIZEbytes.txt"
	echo "Output file info"
	echo -e "\tSIZE : variable size(bytes)"
	echo -e "\tTYPE : you can refer to this site <https://sourceware.org/binutils/docs/binutils/nm.html>"
	exit
fi

if [ -z ${ref_size} ]
then
	ref_size=256
fi

if [ -z ${path} ]
then
	path="${WD}/../../build/output/bin/tinyara"
fi

if [ -z ${output_file} ]
then
	output_file="var_list_over_${ref_size}bytes.txt"
fi

temp_output_file="temp_var_list.txt"

## find over-size global variables through nm
nm -S --size-sort -l $path > $temp_output_file

rm $output_file
echo "SIZE(bytes)	TYPE	VARIABLE		PATH" >> $output_file
echo "------------------------------------------------------------" >> $output_file
while read line 
do
	IFS=' ' read -a array <<< $line
	var_size=`echo $((16#${array[1]}))`
	#Parse only type B, b, d, D, G, g, S, s
	if [ ${array[2]} == "B" ] || [ ${array[2]} == "b" ] || [ ${array[2]} == "d" ] || [ ${array[2]} == "D" ] || [ ${array[2]} == "G" ] || [ ${array[2]} == "g" ] || [ ${array[2]} == "s" ] || [ ${array[2]} == "S" ] && [ $ref_size -le $var_size ]; then 
		echo -e $var_size"\t\t"${array[2]}"\t"${array[3]}"\t\t"${array[4]#$prefix} >> $output_file
	fi

done < $temp_output_file
rm $temp_output_file

echo "Output file(${output_file}) is generated."
