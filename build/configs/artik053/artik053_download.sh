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
# File   : artik053_download.sh
# Description : Download script for ARTIK 053

# Remember, make is invoked from "os" directory
function warn() {
	echo "WARNING: $@" >&2
}

function die() {
	echo "FATAL: $@" >&2
	exit 1
}

# Include the configuration file
source .config
OS_DIR_PATH=${PWD}
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/artik053
OPENOCD_DIR_PATH=${BOARD_DIR_PATH}/tools/openocd
FW_DIR_PATH=${BOARD_DIR_PATH}/bin

CFG_FILE=artik053.cfg


if ! which openocd >/dev/null; then
	SYSTEM_TYPE=`getconf LONG_BIT`
	if [ "$SYSTEM_TYPE" = "64" ]; then
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
	else
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
	fi
	OPENOCD=${OPENOCD_BIN_PATH}/openocd
else
	OPENOCD=openocd
fi

# In - $@ (download parameters: [bl] [bl1] [bl2] [sssfw] [wlan] [os] [rom])
function compute_fw_parts() {
	bl1=0; bl2=2; sssfw=0; wlanfw=0; os=0; rom=0;
	for arg in "$@"; do
		case "${arg}" in
		BL|bl)
			bl1=1;bl2=1;;
		bl1|BL1)
			bl1=1;;
		bl2|BL2)
			bl2=1;;
		sssfw|ssfw|sfw)
			sssfw=1;;
		wlan|wlanfw)
			wlanfw=1;;
		os|OS|tizenrt)
			os=1;;
		rom|romfs|ROM|ROMFS)
			rom=1;;
		ALL)
			bl1=1; bl2=1; sssfw=1; wlanfw=1; os=1; rom=1;;
		*)
			warn "Unknown fw part ${arg}";;
		esac
	done

	parts=
	for var in bl1 bl2 sssfw wlanfw os rom; do
		eval value='${'${var}:-}
		if [ "x${value:-}" == x1 ]; then
			parts+=" ${var}"
		fi
	done
	[ -z "${parts}" ] || echo ${parts}
}

# Exit if the file is not there
function ensure_file() {
	[ -f "${1:?}" ] || die "missing file $1"
}

# $@ - bl1 bl2 sssfw wlanfw os rom
function compute_ocd_commands() {
	commands=
	for part in "$@"; do
		case "${part}" in
		bl1|bl2|sssfw|wlanfw)
			ensure_file ${FW_DIR_PATH}/${part}.bin
			commands+="flash_write ${part} ${FW_DIR_PATH}/${part}.bin; ";;
		os)
			ensure_file ${OUTPUT_BINARY_PATH}/tinyara_head.bin
			commands+="flash_write ${part} ${OUTPUT_BINARY_PATH}/tinyara_head.bin; ";;
		rom)
			ensure_file ${OUTPUT_BINARY_PATH}/romfs.img
			commands+="flash_write ${part} ${OUTPUT_BINARY_PATH}/romfs.img; ";;
		*)
			warn "Unrecognized firmware part ${part}";;
		esac
	done
	echo ${commands}
}

function setup_romfs_variable() {
	# Prepare for ROMFS
	if [ "${CONFIG_FS_ROMFS}" == "y" ]; then
		OPENOCD_ROMFS_SETUP="set romfs_partition_enable 1; echo \"romfs is enabled\""
	else
		OPENOCD_ROMFS_SETUP="set romfs_partition_enable 0; echo \"romfs is disabled\""
	fi
}

# $@ - bl1 bl2 sssfw wlanfw os rom
function upload_components() {
	commands=$(compute_ocd_commands "$@")

	setup_romfs_variable

	pushd ${OPENOCD_DIR_PATH}
	"${OPENOCD}" -c "${OPENOCD_ROMFS_SETUP}" -f "${CFG_FILE}" -c "${commands} exit"
	popd >/dev/null
}

function erase_userfs_maybe() {
	while [ $# -gt 0 ]; do
		case "${1}" in
		ERASE_USERFS|erase_userfs)
			echo "USERFS :"
			pushd ${OPENOCD_DIR_PATH}
			${OPENOCD} -f "${CFG_FILE}" -c 'flash_erase_part user;	exit'
			popd > /dev/null;;
		*)
			:;;
		esac
		shift
	done
}

main() {
	echo "Parameters are: $@"
	echo "openocd is ${OPENOCD}"
	echo "Binaries are picked from ${OUTPUT_BINARY_PATH}"
	echo "Board path is ${BOARD_DIR_PATH}"

	erase_userfs_maybe "$@"

	parts=$(compute_fw_parts "$@")
	if [ -n "${parts:-}" ]; then
		echo "Flashing parts ${parts}"
		upload_components ${parts}
	else
		warn "Flashing no parts"
	fi
}

main "$@"
