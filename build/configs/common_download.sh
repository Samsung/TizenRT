#!/usr/bin/env bash
###########################################################################
#
# Copyright 2021 Samsung Electronics All Rights Reserved.
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
# common_download.sh

CURDIR=$(readlink -f "$0")
CUR_PATH=$(dirname "$CURDIR")

#When the location of this script is changed, only TOP_PATH needs to be changed.
TOP_PATH=${CUR_PATH}/../..
OS_PATH=${TOP_PATH}/os
BIN_PATH=${TOP_PATH}/build/output/bin
PARTITION_KCONFIG=${OS_PATH}/board/common/Kconfig

CONFIG=${OS_PATH}/.config
source ${CONFIG}
SMARTFS_BIN_PATH=${BIN_PATH}/${CONFIG_ARCH_BOARD}_smartfs.bin

BOARD_CONFIG=${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/board_metadata.txt
BOARD_SPECIFIC_SCRIPT=${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/${CONFIG_ARCH_BOARD}_download.sh

source ${BOARD_SPECIFIC_SCRIPT}
source ${BOARD_CONFIG}
source ${OS_PATH}/.bininfo

if [ -c "/dev/$DEFAULT_PORT" ]; then
	DEFAULT_PORT=$DEFAULT_PORT
elif [ -c "/dev/$DEFAULT_PORT2" ]; then
	DEFAULT_PORT=$DEFAULT_PORT2
fi

USBRULE_PATH=${TOP_PATH}/build/configs/usbrule.sh

## For manual port selection set any argument to port={port required}, eg. make download port=ttyUSB2 ALL or make download all port=ttyUSB3

if [ "$1" == "port" ]; then
	PORT="$2"
	shift 2
else
	PORT=${DEFAULT_PORT}
fi

if ! [ -c "/dev/$PORT" ]; then
	echo "$PORT is not available"
	exit
fi
WARNING="\n Port $PORT is selected\n\n
	############################################\n
	WARNINGS:\n
	1. Make sure the board is in DOWNLOAD MODE.\n
	2. Make sure NO other application like putty,\n
	is occupying $PORT.\n
	############################################\n"


TTYDEV="/dev/${PORT}"

##Utility function for sanity check##
function sanity_check()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	if [ ! -f ${BOARD_SPECIFIC_SCRIPT} ];then
		echo "No ${CONFIG_ARCH_BOARD}_download.sh file"
		exit 1
	fi

	if [ ! -f ${BOARD_CONFIG} ];then
		echo "No board_metadata.txt file"
		exit 1
	fi

	CHECK_BOARD=$(echo "CONFIG_ARCH_BOARD_${BOARD}")
	if [[ "${!CHECK_BOARD}" != "y" ]];then
		echo "Target is NOT ${BOARD}"
		exit 1
	fi

	if fuser -s ${TTYDEV};then
		echo "${TTYDEV} is used by another process, can't proceed"
		exit 1
	fi
}

##Utility function to match partition name to binary name##
function get_executable_name()
{
	case $1 in
		bl1)
			if [[ ! -n "${BL1}" ]];then
				echo "No Binary Match"
			else
				echo "${BL1}.bin"
			fi;;
		bl2)
			if [[ ! -n "${BL2}" ]];then
				echo "No Binary Match"
			else
				echo "${BL2}.bin"
			fi;;
		kernel|os) echo "${KERNEL_BIN_NAME}";;
		ota) echo "${OTA}.bin";;
		app1) echo "${APP1_BIN_NAME}";;
		app2) echo "${APP2_BIN_NAME}";;
		loadparam) echo "$1";;
		common) echo "${COMMON_BIN_NAME}";;
		zoneinfo) echo "zoneinfo.img";;
		ext_data) echo "${EXTERNAL}.bin";; 
		resource) echo "${RESOURCE_BIN_NAME}";;
		userfs) echo "${CONFIG_ARCH_BOARD}_smartfs.bin";;
		sssfw|wlanfw) echo "$1.bin";;
		rom) echo "romfs.img";;
		bootparam)
			if [[ ! -n "${BOOTPARAM}" ]];then
				echo "No Binary Match"
			else
				echo "${BOOTPARAM}.bin"
			fi;;
		*) echo "No Binary Match"
		exit 1
	esac
}

flash_ota=false
##Utility function to get partition index ##
function get_partition_index()
{
	for idx in ${!parts[@]}; do
		if [[ ${parts[$idx],,} == ${1,,} ]]; then
			if [[ $flash_ota == true ]]; then
				## In this case, try to find 2nd kernel partition for OTA
				flash_ota=false
				continue;
			fi
			echo $idx
			exit 1
		else
			if [[ $flash_ota == true ]]; then
				## Else check for the partition named OTA
				if [[ ${parts[$idx]} == "ota" ]]; then
					echo $idx
					exit 1
				fi
			fi
		fi
	done
	echo -1
}

##Help utility##
function dwld_help()
{
	cat <<EOF
	HELP:
		make download ALL or [PARTITION(S)]
		make download ERASE ALL or erase [PARTITION(S)]
		make download ALL port=ttyUSB1
		make download port=ttyUSB1 ALL

	PARTITION(S):
		[${uniq_parts[@]}]  NOTE:case sensitive

	For examples:
		make download ALL
		make download ERASE ALL
		make download kernel
		make download erase kernel
		make download ota
		make download erase ota
		make download kernel userfs
		make download erase userfs
		make download erase port=ttyUSB1 userfs
EOF
}

##Utility function to read partitions from .config or Kconfig##
function get_configured_partitions()
{
	local configured_parts
	# Read Partitions
	if [[ -z ${CONFIG_FLASH_PART_NAME} ]];then

		configured_parts=`grep -A 2 'config FLASH_PART_NAME' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	else
		configured_parts=${CONFIG_FLASH_PART_NAME}
	fi
	
	echo $configured_parts
}

function get_configured_second_partitions()
{
	local configured_parts
	# Read Partitions
	if [[ -z ${CONFIG_SECOND_FLASH_PART_NAME} ]];then

		configured_parts=`grep -A 2 'config SECOND_FLASH_PART_NAME' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	else
		configured_parts=${CONFIG_SECOND_FLASH_PART_NAME}
	fi
	
	echo $configured_parts
}

##Utility function to get the partition sizes from .config or Kconfig
function get_partition_sizes()
{
	local sizes_str
	#Read Partition Sizes
	if [[ -z ${CONFIG_FLASH_PART_SIZE} ]]
	then
		sizes_str=`grep -A 2 'config FLASH_PART_SIZE' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	else
		sizes_str=${CONFIG_FLASH_PART_SIZE}
	fi
	
	echo $sizes_str
}

function get_partition_second_sizes()
{
	local sizes_str
	#Read Partition Sizes
	if [[ -z ${CONFIG_SECOND_FLASH_PART_SIZE} ]]
	then
		sizes_str=`grep -A 2 'config SECOND_FLASH_PART_SIZE' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	else
		sizes_str=${CONFIG_SECOND_FLASH_PART_SIZE}
	fi
	
	echo $sizes_str
}

download_specific_partition()
{
	TARGET=$1
	if [[ ${TARGET} == "ota" || ${TARGET} == "OTA" ]];then
		TARGET="kernel"
		flash_ota=true
	fi
	partidx=$(get_partition_index ${TARGET})
	if [[ "${partidx}" < 0 ]];then
		echo "partition ${TARGET} is not supported"
		dwld_help
		exit 1
	fi

	# Get a filename and Download a file
	if [[ $TTYDEV == *"USB"* ]]; then
		echo ""
		echo "============================="
		if [[ $1 == "ota" || $1 == "OTA" ]];then
			echo "Downloading Kernel OTA binary"
		else
			echo "Downloading ${parts[$partidx]} binary"
		fi
		echo "============================="
	fi
	
	exe_name=$(get_executable_name ${parts[$partidx]})
	if [[ "No Binary Match" = "${exe_name}" ]];then
		echo "No corresponding binary for the partition ${parts[$partidx]}"
		echo "Download $exe_name FAILED!"
	else
		board_download $TTYDEV ${offsets[$partidx]} ${exe_name} ${sizes[partidx]} ${parts[$partidx]} $TARGET
		echo ""
		echo "Download $exe_name COMPLETE!"
	fi
}

download_all()
{
	echo "Starting Download..."
	found_kernel=false
	found_app1=false
	found_app2=false
	found_common=false
	found_resource=false
	
	for partidx in ${!parts[@]}; do

		if [[ "${CONFIG_APP_BINARY_SEPARATION}" != "y" ]];then
			if [[ "${parts[$partidx]}" == "userfs" ]];then
				continue
			fi
		fi

		if [[ "${parts[$partidx]}" == "kernel" ]];then
			if [[ $found_kernel == true ]];then
				continue
			fi
			found_kernel=true
		fi

		if [[ "${parts[$partidx]}" == "app1" ]];then
			if [[ $found_app1 == true ]];then
				continue
			fi
			found_app1=true
		fi

		if [[ "${parts[$partidx]}" == "app2" ]];then
			if [[ $found_app2 == true ]];then
				continue
			fi
			found_app2=true
		fi

		if [[ "${parts[$partidx]}" == "common" ]];then
			if [[ $found_common == true ]];then
				continue
			fi
			found_common=true
		fi

		if [[ "${parts[$partidx]}" == "ftl" ]];then
			continue
		fi

		if [[ "${parts[$partidx]}" == "config" ]];then
			continue
		fi
		if [[ "${parts[$partidx]}" == "resource" ]];then
			if [[ $found_resource == true ]];then
				continue
			fi
			found_resource=true
		fi
		exe_name=$(get_executable_name ${parts[$partidx]})
		if [[ "No Binary Match" = "${exe_name}" ]];then
			continue
		fi
		if [[ $TTYDEV == *"USB"* ]]; then
			echo ""
			echo "=========================="
			echo "Downloading ${parts[$partidx]} binary"
			echo "=========================="
		fi
		board_download $TTYDEV ${offsets[$partidx]} ${exe_name} ${sizes[partidx]} ${parts[$partidx]} "ALL"
	done
	echo ""
	echo "Download COMPLETE!"
}

erase()
{
	echo "Starting Erase..."
	if [[ $2 == "all" || $2 == "ALL" ]];then
		echo ""
		echo "=========================="
		echo "      Erasing All"
		echo "=========================="
		for partidx in ${!parts[@]}; do
			if [[ "${parts[$partidx]}" == "ss" ]];then
				continue
			else
				if [[ $TTYDEV == *"USB"* ]]; then
					echo ""
					echo "=========================="
					echo "Erasing ${parts[$partidx]} partition"
					echo "=========================="
				fi
			fi

			board_erase $TTYDEV ${offsets[$partidx]} ${sizes[partidx]} ${parts[$partidx]} $2
		done
	else
		found_kernel=false
		for partidx in ${!parts[@]}; do
			if [[ $2 == "kernel" || $2 == "KERNEL" ]];then
				if [[ "${parts[$partidx]}" != "kernel" ]];then
					continue
				elif [[ $found_kernel == false ]];then
					found_kernel=true
				else
					continue
				fi
			elif [[ $2 == "ota" || $2 == "OTA" ]];then
				if [[ "${parts[$partidx]}" == "ota" ]];then
					flash_ota=false
				elif [[ "${parts[$partidx]}" != "kernel" ]];then
					continue
				elif [[ $found_kernel == false ]];then
					found_kernel=true
					continue
				else
					flash_ota=false
				fi
			elif [[ $2 == "ss" || $2 == "SS" ]];then
				if [[ "${parts[$partidx]}" != "ss" ]];then
					continue
				fi
			elif [[ $2 == "userfs" || $2 == "USERFS" ]];then
				if [[ "${parts[$partidx]}" != "userfs" ]];then
					continue
				fi
			else
				printf "\n## Invalid Option ##\n\n"
				dwld_help
				break
			fi

			if [[ $TTYDEV == *"USB"* ]]; then
				echo ""
				echo "=========================="
				echo "Erasing ${parts[$partidx]} partition"
				echo "=========================="
			fi
			board_erase $TTYDEV ${offsets[$partidx]} ${sizes[partidx]} ${parts[$partidx]} $2
		done
	fi
	echo ""
	echo "Erase COMPLETE!"
}

# Start here

echo -e $WARNING

cmd_args=$(echo $@ | tr '[:upper:]' '[:lower:]')

# Checking the USB rule first
for i in ${cmd_args[@]};do
	if [[ "${i}" == "usbrule" ]];then
		${USBRULE_PATH} ${USBRULE_BOARD} ${USBRULE_IDVENDOR} ${USBRULE_IDPRODUCT} ${USBRULE_IDVENDOR2} ${USBRULE_IDPRODUCT2} || exit 1
		exit 0
	fi
done

sanity_check;

parts=$(get_configured_partitions)
IFS=',' read -ra partfirst <<< "$parts"

sizes=$(get_partition_sizes)
IFS=',' read -ra sizefirst <<< "$sizes"

#Calculate Flash Offset
num=${#sizefirst[@]}
offsets[0]=`printf "0x%X" ${CONFIG_FLASH_START_ADDR}`

for (( i=1; i<=$num-1; i++ ))
do
	val=$((sizefirst[$i-1] * 1024))
	offsets[$i]=`printf "0x%X" $((offsets[$i-1] + ${val}))`
done

if [[ -n ${CONFIG_ARCH_BOARD_HAVE_SECOND_FLASH} ]];then
	parts=$(get_configured_second_partitions)
	IFS=',' read -ra partsecond <<< "$parts"

	sizes=$(get_partition_second_sizes)
	IFS=',' read -ra sizesecond <<< "$sizes"

	#Calculate Flash Offset
	num1=${#sizesecond[@]}
	offsets[$num]=`printf "0x%X" ${CONFIG_SECOND_FLASH_START_ADDR}`

	for (( i=1; i<=$num1-1; i++ ))
	do
		val=$((sizesecond[$i-1] * 1024))
		offsets[$num+$i]=`printf "0x%X" $((offsets[$num+$i-1] + ${val}))`
	done
fi

parts=("${partfirst[@]}" "${partsecond[@]}")
sizes=("${sizefirst[@]}" "${sizesecond[@]}")

#Dump Info
echo ""
echo "================================ < Flash Partition Information > ================================"
printf "\rNAME       :"
printf '  %12s' "${parts[@]}"
printf "\r\nSIZE(in KB):"
printf '  %12s' "${sizes[@]}"
printf "\r\nAddr       :"
printf '  %12s' "${offsets[@]}"
printf "\n"
echo "================================================================================================="
echo ""

if test $# -eq 0; then
	echo "FAIL!! NO Given partition. Refer \"PARTITION NAMES\" above."
	dwld_help 1>&2
	exit 1
fi

uniq_parts=($(printf "%s\n" "${parts[@]}" | sort -u));

#Validate arguments
for i in ${cmd_args[@]};do

	if [[ "${i}" == "ALL" || "${i}" == "all" || "${i}" == "erase" || "${i}" == "ERASE" || "${i}" == "usbrule" || "${i}" == "USBrule" ]];then
		continue;
	fi

	result=no
	for j in ${uniq_parts[@]};do
		if [[ "${i}" == "${j}" ]];then
			result=yes
		fi
	done

	if [[ "$result" != "yes" ]];then
		echo "FAIL!! Given \"${i}\" partition is not available. Refer \"PARTITION NAMES\" above."
		dwld_help
		exit 1
	fi
done

# Must be defined in each board specific download file.
pre_download;

case $1 in
	all|ALL)
		download_all
		;;
	erase|ERASE)
		erase $1 $2
		;;
	kernel|KERNEL)
		download_specific_partition $1
		if [[ -n "${BOOTPARAM}" ]];then
			download_specific_partition "bootparam"
		fi
		;;
	*)
		while test $# -gt 0
		do
			download_specific_partition $1
			shift
		done
		;;
esac

# Must be defined in each board specific download file.
post_download;
