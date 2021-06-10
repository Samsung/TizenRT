#!/usr/bin/env bash
###########################################################################
#
# Copyright 2020 Samsung Electronics All Rights Reserved.
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
# rtl8721csm_download.sh

TOOL_PATH=${TOP_PATH}/build/tools/amebad
IMG_TOOL_PATH=${TOOL_PATH}/image_tool

function pre_download()
{
cp -p ${BIN_PATH}/km0_boot_all.bin ${IMG_TOOL_PATH}/km0_boot_all.bin
cp -p ${BIN_PATH}/km4_boot_all.bin ${IMG_TOOL_PATH}/km4_boot_all.bin
cp -p ${BIN_PATH}/km0_km4_image2.bin ${IMG_TOOL_PATH}/km0_km4_image2.bin
if test -f "${SMARTFS_BIN_PATH}"; then
	cp -p ${BIN_PATH}/rtl8721csm_smartfs.bin ${IMG_TOOL_PATH}/rtl8721csm_smartfs.bin
fi
cp -p ${BIN_PATH}/bootparam.bin ${IMG_TOOL_PATH}/bootparam.bin
}


function board_download()
{
	cd ${IMG_TOOL_PATH}
	if [ ! -f ${IMG_TOOL_PATH}/$3 ];then
		echo "$3 not present"
	else
		./amebad_image_tool "download" $1 1 $2 $3
	fi
}

function board_erase()
{
	cd ${IMG_TOOL_PATH}
	./amebad_image_tool "erase" $1 1 $2 0 $3
}

function post_download()
{
	cd ${IMG_TOOL_PATH}
	[ -e ${BL1}.bin ] && rm ${BL1}.bin
	[ -e ${BL2}.bin ] && rm ${BL2}.bin
	[ -e ${KERNEL}.bin ] && rm ${KERNEL}.bin
	if test -f "${SMARTFS_BIN_PATH}"; then
		[ -e ${CONFIG_ARCH_BOARD}_smartfs.bin ] && rm ${CONFIG_ARCH_BOARD}_smartfs.bin
	fi
	[ -e ${BOOTPARAM}.bin ] && rm ${BOOTPARAM}.bin
}
