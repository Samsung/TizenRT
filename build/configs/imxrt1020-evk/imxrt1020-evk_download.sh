#!/usr/bin/env bash
###########################################################################
#
# Copyright 2019 NXP Semiconductors All Rights Reserved.
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
# imxrt1020-evk_download.sh

WARNING="\n########################################\n
	WARNING:Make sure NO other application like \n
	putty etc, is accessing ttyACM0. If download \n
	fails, reset the board before next try!!\n
	#################################################\n"

echo -e $WARNING

CURDIR=$(readlink -f "$0")
CURDIR_PATH=$(dirname "$CURDIR")

# When location of this script is changed, only TOP_PATH should be changed together!!!
TOP_PATH=${CURDIR_PATH}/../../..

USBRULE_PATH=${CURDIR_PATH}/../usbrule.sh
USBRULE_BOARD="imxrt"
USBRULE_IDVENDOR="0d28"
USBRULE_IDPRODUCT="0204"
USBRULE_IDVENDOR2="0403"
USBRULE_IDPRODUCT2="6010"

OS_PATH=${TOP_PATH}/os
OUTBIN_PATH=${TOP_PATH}/build/output/bin
TTYDEV="/dev/ttyACM0"
TINYARA_BIN=${OUTBIN_PATH}/tinyara.bin
CONFIG=${OS_PATH}/.config
ZONEINFO=${OUTBIN_PATH}/zoneinfo.img
FLASH_START_ADDR=0x60000000

##Utility function for sanity check##
function imxrt1020_sanity_check()
{
	if [ ! -f ${CONFIG} ];then
		echo "No .config file"
		exit 1
	fi

	source ${CONFIG}
	if [[ "${CONFIG_ARCH_BOARD_IMXRT1020_EVK}" != "y" ]];then
		echo "Target is NOT IMXRT1020_EVK"
		exit 1
	fi

	if [ ! -f ${TINYARA_BIN} ]; then
		echo "missing file ${TINYARA_BIN}"
		exit 1
	fi

	if fuser -s ${TTYDEV};then
		echo "${TTYDEV} is used by another process, can't proceed"
		exit 1
	fi
}

##Utility Function to Bootstrap and configure memory##
function imxrt1020_bootstrap()
{
	source ${CURDIR_PATH}/bootstrap.sh
	${BLHOST} -p $TTYDEV -- fill-memory 0x2000 0x04 0xc0000006
	${BLHOST} -p $TTYDEV -- configure-memory 0x09 0x2000
}

##Utility function to erase a part of flash##
#Input: Address and length
function flash_erase()
{
	echo -e "\nFLASH_ERASE: ADDR:$1 LENGTH:$2 KB"
	size_in_bytes=$(($2 * 1024))
	${BLHOST} -p ${TTYDEV} -- flash-erase-region $1 $size_in_bytes
}

##Utility function to write a binary on a flash partition##
#Input: Address and filepath
function flash_write()
{
	echo -e "\nFLASH_WRITE ADDR:$1 \nFILEPATH:$2 "
	${BLHOST} -p ${TTYDEV} -- write-memory $1 $2
	sleep 2
}

##Utility function to match partition name to binary name##
function get_executable_name()
{
	case $1 in
		kernel) echo "tinyara.bin";;
		micom) echo "micom";;
		wifi) echo "wifi";;
		zoneinfo) echo "zoneinfo.img";;
		userfs) echo "imxrt1020-evk_smartfs.bin";;
		*) echo "No Binary Match"
		exit 1
	esac
}

##Utility function to get partition index ##
function get_partition_index()
{
	case $1 in
		kernel) echo "0";;
		micom) echo "1";;
		wifi) echo "3";;
		zoneinfo)
		for i in "${!parts[@]}"
		do
		   if [[ "${parts[$i]}" = "zoneinfo" ]]; then
			echo $i
		   fi
		done
		;;
		userfs)
		for i in "${!parts[@]}"
		do
		   if [[ "${parts[$i]}" = "userfs" ]]; then
			echo $i
		   fi
		done
		;;
		*) echo "No Matching Partition"
		exit 1
	esac
}

##Help utility##
function imxrt1020_dwld_help()
{
        cat <<EOF
	HELP:
		make download ERASE [PARTITION(S)]
		make download ALL or [PARTITION(S)]
	PARTITION(S):
		 [${uniq_parts[@]}]  NOTE:case sensitive

	For examples:
		make download ALL
	        make download kernel
		make download ERASE kernel
EOF
}

##Utility function to read paritions from .config or Kconfig##
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

# Start here

cmd_args=$(echo $@ | tr '[:upper:]' '[:lower:]')

# Treat adding the USB rule first
for i in ${cmd_args[@]};do
	if [[ "${i}" == "usbrule" ]];then
		${USBRULE_PATH} ${USBRULE_BOARD} ${USBRULE_IDVENDOR} ${USBRULE_IDPRODUCT} ${USBRULE_IDVENDOR2} ${USBRULE_IDPRODUCT2} || exit 1
		exit 0
	fi
done

imxrt1020_sanity_check;

parts=$(get_configured_partitions)
IFS=',' read -ra parts <<< "$parts"

sizes=$(get_partition_sizes)
IFS=',' read -ra sizes <<< "$sizes"

#Calculate Flash Offset
num=${#sizes[@]}
offsets[0]=`printf "0x%X" ${FLASH_START_ADDR}`

for (( i=1; i<=$num-1; i++ ))
do
	val=$((sizes[$i-1] * 1024))
	offsets[$i]=`printf "0x%X" $((offsets[$i-1] + ${val}))`
done

#Dump Info
echo "PARTIION OFFSETS: ${offsets[@]}"
echo "PARTITION SIZES: ${sizes[@]}"
echo "PARTIION NAMES: ${parts[@]}"

if test $# -eq 0; then
	echo "FAIL!! NO Given partition. Refer \"PARTITION NAMES\" above."
	imxrt1020_dwld_help 1>&2
	exit 1
fi

uniq_parts=($(printf "%s\n" "${parts[@]}" | sort -u));

#Validate arguments
for i in ${cmd_args[@]};do

	if [[ "${i}" == "erase" || "${i}" == "all" ]];then
		continue;
	fi

	for j in ${uniq_parts[@]};do
		if [[ "${i}" == "${j}" ]];then
			result=yes
		fi
	done

	if [[ "$result" != "yes" ]];then
		echo "FAIL!! Given \"${i}\" partition is not available. Refer \"PARTITION NAMES\" above."
		imxrt1020_dwld_help
		exit 1
	fi
	result=no
done

imxrt1020_bootstrap;

case ${1,,} in
#Download ALL option
all)
	for part in ${uniq_parts[@]}; do
		if [[ "${CONFIG_SUPPORT_COMMON_BINARY}" != "y" ]];then
			if [[ "$part" == "userfs" ]];then
				continue
			fi
		fi
		if [[ "$part" == "ftl" ]];then
			continue
		fi
		if [[ "$part" == "config" ]];then
			continue
		fi
		gidx=$(get_partition_index $part)
		flash_erase ${offsets[$gidx]} ${sizes[$gidx]}
		exe_name=$(get_executable_name ${parts[$gidx]})
		flash_write ${offsets[$gidx]} ${OUTBIN_PATH}/${exe_name}
	done
	;;
#Download ERASE <list of partitions>
erase)
	while test $# -gt 1
	do
		chk=${2,,}
		for i in "${!parts[@]}"; do
		   if [[ "${parts[$i]}" = "${chk}" ]]; then
			flash_erase ${offsets[${i}]} ${sizes[${i}]}
		   fi
		done
		shift
	done
	;;
#Download <list of partitions>
*)
	while test $# -gt 0
	do
		chk=${1,,}
		for i in "${!uniq_parts[@]}"; do
		   if [[ "${uniq_parts[$i]}" = "${chk}" ]]; then
			gidx=$(get_partition_index ${chk})
			flash_erase ${offsets[$gidx]} ${sizes[$gidx]}
			exe_name=$(get_executable_name ${chk})
			flash_write ${offsets[$gidx]} ${OUTBIN_PATH}/${exe_name}
		   fi
		done
		shift
	done
esac
