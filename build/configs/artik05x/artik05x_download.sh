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

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!

PARTMAP_DIR_PATH=${THIS_PATH}
PARTITION_KCONFIG=${OS_PATH}/board/common/Kconfig
BUILD_DIR_PATH=${OS_PATH}/../build
CONFIGS_DIR_PATH=${TOP_PATH}/build/configs
OUTPUT_BINARY_PATH=${TOP_PATH}/build/output/bin
ARTIK05X_DIR_PATH=${CONFIGS_DIR_PATH}/artik05x
SCRIPTS_PATH=${ARTIK05X_DIR_PATH}/scripts

if [[ "${CONFIG_ARCH_BOARD_ARTIK053}" == "y" ]]; then
	BOARD_NAME=artik053
	SECURE=""
fi

if [[ "${CONFIG_ARCH_BOARD_ARTIK053S}" == "y" ]]; then
	BOARD_NAME=artik053s
	SECURE="-signed"
fi

if [[ "${CONFIG_ARCH_BOARD_ARTIK055S}" == "y" ]]; then
	BOARD_NAME=artik055s
	SECURE="-signed"
fi

BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/${BOARD_NAME}
FW_DIR_PATH=${BOARD_DIR_PATH}/bin

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

function pre_download(){
	${SCRIPTS_PATH}/partition_gen.sh
}

function board_download()
{
	commands=""
	case $5 in
		bl1)
			if [ ! -f ${FW_DIR_PATH}/$3 ];then
				echo "#NOTE#: No binary available for $5"
			else
				commands+="flash_protect off; flash_write $5 ${FW_DIR_PATH}/$3; flash_protect on; "
			fi
			;;
		bl2|sssfw|wlanfw)
			if [ ! -f ${FW_DIR_PATH}/$3 ];then
				echo "#NOTE#: No binary available for $5"
			else
				commands+="flash_write $5 ${FW_DIR_PATH}/$3 ${VERIFY}; "
			fi
			;;
		os)
			if [ ! -f ${BIN_PATH}/$3 ];then
				echo "#NOTE#: No binary available for $5"
			else
				commands+="flash_write $5 ${BIN_PATH}/$3${SECURE} ${VERIFY}; "
			fi
			;;
		ota|rom|zoneinfo|loadparam|micom|wifi|userfs)
			if [ ! -f ${BIN_PATH}/$3 ];then
				echo "#NOTE#: No binary available for $5"
			else
				commands+="flash_write $5 ${BIN_PATH}/$3 ${VERIFY}; "
			fi
			;;
		*)
			echo "#NOTE#: No binary available for $5"
	esac
	# Download all binaries using openocd script
	echo "${commands}"
	pushd ${OPENOCD_DIR_PATH} > /dev/null
	${OPENOCD} -f ${CFG_FILE} -s ${SCRIPTS_PATH} -c "${commands}" -c "init; reset; exit" || exit 1
	popd > /dev/null
}

function board_erase()
{

	pushd ${OPENOCD_DIR_PATH} > /dev/null
	${OPENOCD} -f ${CFG_FILE} -s ${SCRIPTS_PATH} -c "flash_erase_part $4;	exit" || exit 1
	popd > /dev/null
}

function post_download(){
	:;
}
