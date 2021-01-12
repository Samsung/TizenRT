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

MEMTOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
BASE_PATH=`echo ${MEMTOOL_PATH%/*/*}`

usage()
{
	echo "Usages: $0 [-s SIZE] [-e ELF] [-o OUTPUT_FILENAME]"
	echo "This script parses the global variable which size is greater than [SIZE]"
	echo -e "\tIf no input params are specified, below is assumed"
	echo -e "\tSIZE : 256"
	echo -e "\tELF : ../../build/output/bin/tinyara"
	echo -e "\tOUTPUT_FILENAME : var_list_over_SIZEbytes.txt"
	echo "Output file info"
	echo -e "\tSIZE : variable size(bytes)"
	echo -e "\tTYPE : you can refer to this site <https://sourceware.org/binutils/docs/binutils/nm.html>"
}

while getopts ":hs:e:o:" arg; do
	case ${arg} in
		s)
			REF_SIZE=`echo "${OPTARG}" | grep -E ^\-?[0-9]+$`
			if [[ -z ${REF_SIZE} || ${REF_SIZE} -lt 1 ]]; then
				echo "${OPTARG} is a wrong SIZE"
				usage
				exit 1
			fi
			;;
		e)
			if [ -f ${OPTARG} ]; then	
				ELF=${OPTARG}
			else
				echo "${OPTARG} is a wrong ELF"
				usage
				exit 1
			fi
			;;
		o)
			OUTPUT_FILE=${OPTARG}
			;;
		h)
			usage
			exit 0
			;;
		*)
			usage
			exit 1
			;;
	esac
done

shift $(($OPTIND - 1))
if [ ! -z "$*" ]; then
	echo "$* are not supported"
	usage
	exit 1
fi

if [ -z ${REF_SIZE} ]; then
	REF_SIZE=256
fi

if [ -z ${ELF} ]; then
	ELF="${MEMTOOL_PATH}/../../build/output/bin/tinyara"
	if [ ! -f ${ELF} ]; then
		echo "Unavailable Default ELF \"${ELF}\""
		exit 1
	fi
fi

if [ -z ${OUTPUT_FILE} ]; then
	OUTPUT_FILE="var_list_over_${REF_SIZE}bytes.txt"
fi

temp_output_file="temp_var_list.txt"

echo "Size   : ${REF_SIZE}"
echo "ELF    : ${ELF}"
echo "Output : ${OUTPUT_FILE}"

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

echo "${OUTPUT_FILE} is generated."
