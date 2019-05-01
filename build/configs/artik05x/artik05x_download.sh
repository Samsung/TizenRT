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
#
# File   : artik05x_download.sh
# Description : Download script for ARTIK 05X

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../os

source ${OS_DIR_PATH}/.config

PARTMAP_DIR_PATH=${THIS_PATH}
PARTITION_KCONFIG=${OS_DIR_PATH}/board/common/Kconfig
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
CONFIGS_DIR_PATH=${BUILD_DIR_PATH}/configs
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
ARTIK05X_DIR_PATH=${CONFIGS_DIR_PATH}/artik05x
SCRIPTS_PATH=${ARTIK05X_DIR_PATH}/scripts

TIZENRT_BIN=${OUTPUT_BINARY_PATH}/tinyara_head.bin

OPENOCD_DIR_PATH=${BUILD_DIR_PATH}/tools/openocd
if [[ $OSTYPE == "darwin"* ]]; then
	OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/macos
elif [[ $OSTYPE == "linux"* ]]; then
	SYSTEM_TYPE=`getconf LONG_BIT`
	if [ "$SYSTEM_TYPE" = "64" ]; then
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
	else
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
	fi
else
    echo "Doesn’t support Host OS: $OSTYPE"
    exit 1
fi
OPENOCD=${OPENOCD_BIN_PATH}/openocd

CFG_FILE=artik05x.cfg

usage() {
	cat <<EOF
USAGE: `basename $0` [OPTIONS]
OPTIONS:
	[--board[="<board-name>"]]
	[--secure]
	[ALL | ROMFS | BL1 | BL2 | SSSFW | WLANFW]

For examples:
	`basename $0` --board=artik053 ALL
	`basename $0` --board=artik053 OS ROMFS
	`basename $0` --board=artik053s --verify
	`basename $0` --board=artik055s --secure

Options:
	--board[="<board-name>"]      select target board-name
	--secure                      choose secure mode
	--verify                      verify downloaded image if you need
	ALL                           write each firmware image into FLASH
	ROMFS                         write ROM File System image into FLASH
	BL1                           write Primary Bootloader image into FLASH
	BL2                           write Secondary Bootloader image into FLASH
	SSSFW                         write Secure Element Firmware image into FLASH
	WLANFW                        write WiFi Firmware image into FLASH

EOF
}

# Exit if the file is not there
ensure_file()
{
	if [ ! -f "${1:?}" ]; then
		echo "missing file $1"
		exit 1
	fi
}

# Check if the file is present
is_file_present()
{
	if [ ! -f "${1:?}" ]; then
	    return 0
	fi
	return 1
}

compute_ocd_commands()
{
	local commands=
	for part in "$@"; do
		case "${part}" in
			bl1)
				ensure_file ${FW_DIR_PATH}/${part}.bin
				commands+="flash_protect off; flash_write bl1 ${FW_DIR_PATH}/bl1.bin ${VERIFY}; flash_protect on; "
				;;
			bl2|sssfw|wlanfw)
				ensure_file ${FW_DIR_PATH}/${part}.bin
				commands+="flash_write ${part} ${FW_DIR_PATH}/${part}.bin ${VERIFY}; "
				;;
			os)
				ensure_file ${TIZENRT_BIN}
				commands+="flash_write ${part} ${TIZENRT_BIN} ${VERIFY}; "
				;;
			ota)
			        is_file_present ${OUTPUT_BINARY_PATH}/ota.bin
			        if [[ $? -eq 1 ]]; then
				    commands+="flash_write ${part} ${OUTPUT_BINARY_PATH}/ota.bin ${VERIFY}; "
			         else
				     echo "#NOTE: ${OUTPUT_BINARY_PATH}/ota.bin is NOT present";
				fi
				;;
			rom)
			         is_file_present ${OUTPUT_BINARY_PATH}/romfs.img
				 if [[ $? -eq 1 ]]; then
				     commands+="flash_write ${part} ${OUTPUT_BINARY_PATH}/romfs.img ${VERIFY}; "
				 else
				     echo "#NOTE: ${OUTPUT_BINARY_PATH}/romfs.img is NOT present";
				 fi
				;;
			*)
				echo "#NOTE#: No binary available for ${part}"

				;;
		esac
	done
	echo ${commands}
}

download()
{
	parts=$1;

	if [[ -n $parts ]] && [[ "$parts" != "all" ]] && [[ "$parts" != "ALL" ]]
	then
		echo "##Download $parts"
		parts=$(echo $parts | tr '[:upper:]' '[:lower:]')
	else
	        default_parts=`grep -A 2 'config FLASH_PART_NAME' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
		configured_parts=${CONFIG_FLASH_PART_NAME:=${default_parts}}
		parts=`echo $configured_parts | sed "s/,/ /g"`
	fi

	# Make Openocd commands for parts
	commands=$(compute_ocd_commands ${parts})
	echo "ocd command to run: ${commands}"

	# Generate Partition Map
	${SCRIPTS_PATH}/partition_gen.sh

	# Download all binaries using openocd script
	pushd ${OPENOCD_DIR_PATH} > /dev/null
	${OPENOCD} -f ${CFG_FILE} -s ${SCRIPTS_PATH} -c "${commands}" -c "init; reset; exit" || exit 1
	popd > /dev/null

	echo "Flash DONE"
}

erase()
{
	case $optarg in
		OTA|ota)
			PART_NAME=ota
			;;
		USERFS|userfs)
			PART_NAME=user
			;;
		ALL|all)
			PART_NAME=all
			;;
		*)
			echo "${optarg} is not supported"
			exit 1
			;;
	esac

	# Generate Partition Map
	${SCRIPTS_PATH}/partition_gen.sh

	# Download all binaries using openocd script
	pushd ${OPENOCD_DIR_PATH} > /dev/null
	${OPENOCD} -f ${CFG_FILE} -s ${SCRIPTS_PATH} -c "flash_erase_part ${PART_NAME};	exit" || exit 1
	popd > /dev/null
}

if test $# -eq 0; then
	usage 1>&2
	exit 1
fi

while test $# -gt 0; do
	case "$1" in
		-*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
		ERASE_*) optarg=`echo "$1" | sed 's/ERASE_//'` ;;
		*) optarg= ;;
	esac

	case $1 in
		--board*)
			BOARD_NAME=$optarg
			BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/${BOARD_NAME}
			FW_DIR_PATH=${BOARD_DIR_PATH}/bin
			if [ ! -d ${BOARD_DIR_PATH} ]; then
				usage 1>&2
				exit 1
			fi
			;;
		--secure)
			TIZENRT_BIN=${TIZENRT_BIN}-signed
			;;
		--verify)
			VERIFY=verify
			;;
		ALL|OS|ROM|BL1|BL2|SSSFW|WLANFW|OTA|all|os|rom|bl1|bl2|sssfw|wlanfw|ota)
			download $1
			;;
		ERASE_*)
			erase
			;;
		*)
			usage 1>&2
			exit 1
			;;
	esac
	shift
done
