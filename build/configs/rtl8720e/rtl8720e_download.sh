#!/usr/bin/env bash
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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
# rtl8720e_download.sh

TOOL_PATH=${TOP_PATH}/build/tools/amebalite
IMG_TOOL_PATH=${TOOL_PATH}/image_tool

OS_PATH=${TOP_PATH}/os
CONFIG=${OS_PATH}/.config
source ${CONFIG}
APP_NUM=0

function pre_download()
{
	source ${TOP_PATH}/os/.bininfo
	cp -p ${BIN_PATH}/km4_boot_all.bin ${IMG_TOOL_PATH}/km4_boot_all.bin
	cp -p ${BIN_PATH}/${KERNEL_BIN_NAME} ${IMG_TOOL_PATH}/${KERNEL_BIN_NAME}
	if [ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]; then
		if test -f "${BIN_PATH}/${APP1_BIN_NAME}"; then
			APP_NUM=$(($APP_NUM+1))
			cp -p ${BIN_PATH}/${APP1_BIN_NAME} ${IMG_TOOL_PATH}/${APP1_BIN_NAME}
		fi
		if test -f "${BIN_PATH}/${APP2_BIN_NAME}"; then
			APP_NUM=$(($APP_NUM+1))
			cp -p ${BIN_PATH}/${APP2_BIN_NAME} ${IMG_TOOL_PATH}/${APP2_BIN_NAME}
		fi
		if [ ${APP_NUM} -eq 0 ]; then
			echo "No User Binary."
			post_download
			exit 1
		fi
	fi
	if test -f "${BIN_PATH}/${COMMON_BIN_NAME}"; then
		cp -p ${BIN_PATH}/${COMMON_BIN_NAME} ${IMG_TOOL_PATH}/${COMMON_BIN_NAME}
	fi
	if test -f "${SMARTFS_BIN_PATH}"; then
		cp -p ${SMARTFS_BIN_PATH} ${IMG_TOOL_PATH}/${CONFIG_ARCH_BOARD}_smartfs.bin
	fi
	if test -f "${BIN_PATH}/${BOOTPARAM}.bin"; then
		cp -p ${BIN_PATH}/${BOOTPARAM}.bin ${IMG_TOOL_PATH}/${BOOTPARAM}.bin
	fi
}

function board_download()
{
	cd ${IMG_TOOL_PATH}
	if [ ! -f ${IMG_TOOL_PATH}/$3 ];then
		echo "$3 not present"
	else
		./upload_image_tool_linux "download" $1 1 $2 $3
	fi
}

function board_erase()
{
	cd ${IMG_TOOL_PATH}
	./upload_image_tool_linux "erase" $1 1 $2 $3
}

function post_download()
{
	cd ${IMG_TOOL_PATH}
	[ -e "${BL1}.bin" ] && rm ${BL1}.bin
	[ -e "${KERNEL_BIN_NAME}" ] && rm ${KERNEL_BIN_NAME}
	[ -e "${APP1_BIN_NAME}" ] && rm ${APP1_BIN_NAME}
	[ -e "${APP2_BIN_NAME}" ] && rm ${APP2_BIN_NAME}
	[ -e "${COMMON_BIN_NAME}" ] && rm ${COMMON_BIN_NAME}
	[ -e "${CONFIG_ARCH_BOARD}_smartfs.bin" ] && rm ${CONFIG_ARCH_BOARD}_smartfs.bin
	[ -e "${BOOTPARAM}.bin" ] && rm ${BOOTPARAM}.bin
}


