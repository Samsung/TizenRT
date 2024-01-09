#!/usr/bin/env bash
###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
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
# rtl8730e_download.sh

TOOL_PATH=${TOP_PATH}/build/tools/amebasmart
IMG_TOOL_PATH=${TOOL_PATH}/image_tool

OS_PATH=${TOP_PATH}/os
CONFIG=${OS_PATH}/.config
source ${CONFIG}
APP_NUM=0

function pre_download()
{
	source ${TOP_PATH}/os/.bininfo
	cp -p ${BIN_PATH}/${BL1}.bin ${IMG_TOOL_PATH}/${BL1}.bin 
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

	echo ""
	echo "=========================="
	echo "Checking flash size"
	echo "=========================="
	board_check $TTYDEV
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

function board_check()
{
	cd ${IMG_TOOL_PATH}
	./upload_image_tool_linux "check" $1
	flashsz=$?
	echo "Flash size is ${flashsz} MB"

	for partidx in ${!parts[@]}; do
		if [[ "${parts[$partidx]}" == "reserved" ]];then
			continue
		fi

		if [[ "${parts[$partidx]}" == "ftl" ]];then
			continue
		fi

		if [[ "${parts[$partidx]}" == "ss" ]];then
			continue
		fi

		#echo "${parts[$partidx]}  offset ${a} size ${b}KB"
		MBtoB=$((1024 * 1024))
		FLASHEND=$((flashsz * MBtoB))
		#echo "flash end address ${FLASHEND}"
		KBtoB=1024
		IMAGESIZE_B=$((${sizes[partidx]} * KBtoB))
		IMAGEEND=$((${offsets[$partidx]} + $IMAGESIZE_B))
		#echo "image end address ${IMAGEEND}"

		VIRTUALOFFSET=134217728
		if [ $((${IMAGEEND} - $VIRTUALOFFSET)) -gt $FLASHEND ]
		then
			echo "ERROR: Flash size ${flashsz} MB is smaller than the end address at ${parts[$partidx]}"
			exit 1
		else
			continue
		fi
	done

	echo "flash check pass"
	echo ""
}

function post_download()
{
	cd ${IMG_TOOL_PATH}
	[ -e ${BL1}.bin ] && rm ${BL1}.bin
	if test -f "${KERNEL_BIN_NAME}"; then
		[ -e ${KERNEL_BIN_NAME} ] && rm ${KERNEL_BIN_NAME}
	fi
	if test -f "${APP1_BIN_NAME}"; then
		[ -e ${APP1_BIN_NAME} ] && rm ${APP1_BIN_NAME}
	fi
	if test -f "${APP2_BIN_NAME}"; then
		[ -e ${APP2_BIN_NAME} ] && rm ${APP2_BIN_NAME}
	fi
	if test -f "${COMMON_BIN_NAME}"; then
		[ -e ${COMMON_BIN_NAME} ] && rm ${COMMON_BIN_NAME}
	fi
	if test -f "${SMARTFS_BIN_PATH}"; then
		[ -e ${CONFIG_ARCH_BOARD}_smartfs.bin ] && rm ${CONFIG_ARCH_BOARD}_smartfs.bin
	fi
	if test -f "${BOOTPARAM}.bin"; then
		[ -e ${BOOTPARAM}.bin ] && rm ${BOOTPARAM}.bin
	fi
}


