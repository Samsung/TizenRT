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

REF_SIZE=$1
ELF=$2
OUTPUT_FILE=$3
MEMTOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
BASE_PATH=`echo ${MEMTOOL_PATH%/*/*}`

if [ "$1" == "--help" ]
then
	echo "Usage: $0 [SIZE] [ELF] [OUTPUT_FILENAME]"
	echo "This script parses the global variable which size is greater than [SIZE]"
	echo -e "\tIf no input params are specified, below is assumed"
	echo -e "\tSIZE : 256"
	echo -e "\tELF : ../../build/output/bin/tinyara"
	echo -e "\tOUTPUT_FILENAME : var_list_over_SIZEbytes.txt"
	echo "Output file info"
	echo -e "\tSIZE : variable size(bytes)"
	echo -e "\tTYPE : you can refer to this site <https://sourceware.org/binutils/docs/binutils/nm.html>"

	exit
fi

if [ -z ${REF_SIZE} ]; then
	REF_SIZE=256
fi

if [ -z ${ELF} ]; then
	ELF="${MEMTOOL_PATH}/../../build/output/bin/tinyara"
fi

if [ -z ${OUTPUT_FILE} ]; then
	OUTPUT_FILE="var_list_over_${REF_SIZE}bytes.txt"
fi

temp_output_file="temp_var_list.txt"

## find over-size global variables through nm
nm -S --size-sort -l $ELF > $temp_output_file

if [ -f ${OUTPUT_FILE} ]
then
	rm $OUTPUT_FILE
fi

echo "SIZE(bytes)	TYPE	VARIABLE		PATH" >> $OUTPUT_FILE
echo "------------------------------------------------------------" >> $OUTPUT_FILE
while read line 
do
	IFS=' ' read -a array <<< $line
	var_size=`echo $((16#${array[1]}))`
	#Parse only type B, b, d, D, G, g, S, s
	if [ ${array[2]} == "B" ] || [ ${array[2]} == "b" ] || [ ${array[2]} == "d" ] || [ ${array[2]} == "D" ] || [ ${array[2]} == "G" ] || [ ${array[2]} == "g" ] || [ ${array[2]} == "s" ] || [ ${array[2]} == "S" ] && [ $REF_SIZE -le $var_size ]; then 
		echo -e $var_size"\t\t"${array[2]}"\t"${array[3]}"\t\t"${array[4]#$BASE_PATH} >> $OUTPUT_FILE
	fi

done < $temp_output_file
rm $temp_output_file

echo "Output file(${OUTPUT_FILE}) is generated."
