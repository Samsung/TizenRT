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

VID_FOUND=0
USB_DOWNLOAD=0
RETRY_COUNT=0
MAX_RETRIES=10

function pre_download()
{
	while [ $VID_FOUND -ne 1 ] && [ $RETRY_COUNT -lt $MAX_RETRIES ]; do
		VENDOR_ID=$(udevadm info -a -n "$TTYDEV" | grep -m1 'ATTRS{idVendor}' | sed -nE 's/.*"([0-9a-fA-F]+)".*/\1/p')

		if [ -n "$VENDOR_ID" ]; then
			VID_FOUND=1
			echo "Found VID: $VENDOR_ID"
			break
		else
			echo "VID not found, retrying..."
			((RETRY_COUNT++))
			sleep 1
		fi
	done
	if [ $VID_FOUND -ne 1 ]; then
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
	done

	if [[ -n ${CONFIG_ARCH_BOARD_HAVE_SECOND_FLASH} ]];then
		source ${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/board_metadata.txt
		if test -f "${BIN_PATH}/${EXTERNAL}.bin"; then
			cp -p ${BIN_PATH}/${EXTERNAL}.bin ${IMG_TOOL_PATH}/${EXTERNAL}.bin
		fi

	fi

}

function is_batch_supported()
{
	if [[ $VENDOR_ID == "0bda" ]]; then
		echo 1
	else
		echo 0
	fi
}

function board_download_batch()
{
	TTYDEV=$1
	shift

	touch "${IMG_TOOL_PATH}/USB_download_setting.txt"
	
	cd ${IMG_TOOL_PATH}

	while test $# -gt 0
	do
		if [ ! -f ${IMG_TOOL_PATH}/$2 ];then
			echo "$2 not present"
			shift 4
			continue
		fi
		echo "$2" >> USB_download_setting.txt
		echo "$1" >> USB_download_setting.txt
		shift 4
	done


	./upload_image_tool_linux "download" $TTYDEV "ALL"

	rm "${IMG_TOOL_PATH}/USB_download_setting.txt"

	echo "Complete USB download"
}

function board_download()
{
	cd ${IMG_TOOL_PATH}
	if [ ! -f ${IMG_TOOL_PATH}/$3 ];then
		echo "$3 not present"
	else
		#echo "UART download"
		./upload_image_tool_linux "download" $1 1 $2 $3
	fi
}

function board_erase_batch()
{
	TTYDEV=$1
	shift

	touch "${IMG_TOOL_PATH}/USB_erase_setting.txt"

	cd ${IMG_TOOL_PATH}

	while test $# -gt 0
	do
		echo "$1" >> USB_erase_setting.txt
		echo "$2" >> USB_erase_setting.txt
		shift 3
	done
	./upload_image_tool_linux "erase" $TTYDEV "ALL"

	rm "${IMG_TOOL_PATH}/USB_erase_setting.txt"

	echo "Complete USB erase"
}

function board_erase()
{
	cd ${IMG_TOOL_PATH}
	./upload_image_tool_linux "erase" $1 1 $2 $3
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

}


