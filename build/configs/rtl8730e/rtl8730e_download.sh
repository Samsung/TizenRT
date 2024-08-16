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
LAST_IMAGE=0
LAST_OFFSET=0
USB_DOWNLOAD=0

function pre_download()
{
	if [[ $TTYDEV == *"USB"* ]]; then
		USB_DOWNLOAD=0
	elif [[ $TTYDEV == *"ACM"* ]]; then
		USB_DOWNLOAD=1
	else
		echo "Port name $TTYDEV not valid"
		exit 1
	fi
	source ${TOP_PATH}/os/.bininfo
	if [ -f ${IMG_TOOL_PATH}/USB_download_setting.txt ]; then
		rm -rf ${IMG_TOOL_PATH}/USB_download_setting.txt
	fi
	if [ -f ${IMG_TOOL_PATH}/USB_erase_setting.txt ]; then
		rm -rf ${IMG_TOOL_PATH}/USB_erase_setting.txt
	fi
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
	if test -f "${BIN_PATH}/${RESOURCE_BIN_NAME}"; then
		cp -p ${BIN_PATH}/${RESOURCE_BIN_NAME} ${IMG_TOOL_PATH}/${RESOURCE_BIN_NAME}
	fi
	if test -f "${SMARTFS_BIN_PATH}"; then
		cp -p ${SMARTFS_BIN_PATH} ${IMG_TOOL_PATH}/${CONFIG_ARCH_BOARD}_smartfs.bin
	fi
	if test -f "${BIN_PATH}/${BOOTPARAM}.bin"; then
		cp -p ${BIN_PATH}/${BOOTPARAM}.bin ${IMG_TOOL_PATH}/${BOOTPARAM}.bin
	fi
	if test -f "${BIN_PATH}/${EXTERNAL}.bin"; then
		cp -p ${BIN_PATH}/${EXTERNAL}.bin ${IMG_TOOL_PATH}/${EXTERNAL}.bin
	fi

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
		LAST_OFFSET=${offsets[$partidx]}
	done
	LAST_IMAGE=${BOOTPARAM}.bin

	if [[ -n ${CONFIG_ARCH_BOARD_HAVE_SECOND_FLASH} ]];then
		source ${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/board_metadata.txt
		if test -f "${BIN_PATH}/${EXTERNAL}.bin"; then
			cp -p ${BIN_PATH}/${EXTERNAL}.bin ${IMG_TOOL_PATH}/${EXTERNAL}.bin
		fi
		LAST_IMAGE=${RESOURCE_BIN_NAME}

	fi
	if [ "$USB_DOWNLOAD" -eq "1" ]; then
		touch "${IMG_TOOL_PATH}/USB_download_setting.txt"
		touch "${IMG_TOOL_PATH}/USB_erase_setting.txt"
	fi
}

function board_download()
{
	cd ${IMG_TOOL_PATH}
	if [ ! -f ${IMG_TOOL_PATH}/$3 ];then
		echo "$3 not present"
	else
		if [ "$USB_DOWNLOAD" -eq "0" ]; then
			#echo "UART download"
			./upload_image_tool_linux "download" $1 1 $2 $3
		fi
		if [ "$USB_DOWNLOAD" -eq "1" ]; then
			#echo "USB download"
			#echo "Save info to USB_download_setting.txt"
			echo "$3" >> USB_download_setting.txt
			echo "$2" >> USB_download_setting.txt
			if [ "$6" == "ALL" ] && [ "$3" == "$LAST_IMAGE" ]; then
				echo ""
				echo "==================================="
				echo "Start USB full download in Flash"
				echo "==================================="
				./upload_image_tool_linux "download" $1 "ALL"
				echo "Complete USB download"
			elif [ "$6" == "KERNEL" ]; then
				return;
			elif [ "$6" == "bootparam" ]; then
				echo ""
				echo "========================================="
				echo "Start USB download to kernel partition"
				echo "========================================="
				./upload_image_tool_linux "download" $1 "ALL"
				echo "Complete USB download"
			elif [ "$6" != "ALL" ]; then
				echo ""
				echo "========================================="
				echo "Start USB download to $5 partition"
				echo "========================================="
				./upload_image_tool_linux "download" $1 $2 $3
				echo "Complete USB download"
			else
				return;
			fi
		fi
	fi
}

function board_erase()
{
	cd ${IMG_TOOL_PATH}
	if [ "$USB_DOWNLOAD" -eq "0" ]; then
		#echo "UART erase"
		./upload_image_tool_linux "erase" $1 1 $2 $3
	fi
	if [ "$USB_DOWNLOAD" -eq "1" ]; then
		#echo "USB erase"
		#echo "Save info to USB_erase_setting.txt"
		echo "$2" >> USB_erase_setting.txt
		echo "$3" >> USB_erase_setting.txt
		if [ "$5" == "ALL" ] && [ "$2" == "$LAST_OFFSET" ]; then
			echo ""
			echo "==================================="
			echo "Start USB full erase in Flash"
			echo "==================================="
			./upload_image_tool_linux "erase" $1 "ALL"
			echo "Complete USB erase"
		elif [ "$5" != "ALL" ]; then
			echo ""
			echo "==================================="
			echo "Start USB $4 erase in Flash"
			echo "==================================="
			./upload_image_tool_linux "erase" $1 $2 $3
			echo "Complete USB download"
		else
			return;
		fi
	fi
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
	if test -f "${RESOURCE_BIN_PATH}"; then
		[ -e  ] && rm ${RESOURCE_BIN_PATH}
	fi
	if test -f "${BOOTPARAM}.bin"; then
		[ -e ${BOOTPARAM}.bin ] && rm ${BOOTPARAM}.bin
	fi
	if test -f "${RESOURCE_BIN_NAME}"; then
		[ -e ${RESOURCE_BIN_NAME} ] && rm ${RESOURCE_BIN_NAME}
	fi
	if test -f "${EXTERNAL}.bin"; then
		[ -e ${EXTERNAL}.bin ] && rm ${EXTERNAL}.bin
	fi
	if [ "$USB_DOWNLOAD" -eq "1" ]; then
		rm "${IMG_TOOL_PATH}/USB_download_setting.txt"
		rm "${IMG_TOOL_PATH}/USB_erase_setting.txt"
	fi
}


