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

if [ "$1" != "ALL" ]; then
	echo -e "\nWarning: Unsupported download option \""$1"\" is detected !!!\n\nSuggested option: \"ALL\"\n";
	exit 0
fi

if [ -n "$2" ]; then
	PORT="$2"
else
	PORT="ttyUSB1"
fi

WARNING="\n Port $PORT is selected\n\n
	############################################\n
	WARNINGS:\n
	1. Make sure the board is in DOWNLOAD MODE.\n
	2. Make sure NO other application like putty,\n
	is occupying $PORT.\n
	############################################\n"

echo -e $WARNING

CURDIR=$(readlink -f "$0")
CUR_PATH=$(dirname "$CURDIR")
TOOL_PATH=${CUR_PATH}/../../tools/amebad
IMG_TOOL_PATH=${TOOL_PATH}/image_tool
BIN_PATH=${CUR_PATH}/../../output/bin
SUDO=sudo

cp -p ${BIN_PATH}/km0_boot_all.bin ${IMG_TOOL_PATH}/km0_boot_all.bin
cp -p ${BIN_PATH}/km4_boot_all.bin ${IMG_TOOL_PATH}/km4_boot_all.bin
cp -p ${BIN_PATH}/km0_km4_image2.bin ${IMG_TOOL_PATH}/km0_km4_image2.bin

cd ${IMG_TOOL_PATH}

echo "Start Download"
$SUDO ./amebad_image_tool /dev/$PORT

$SUDO [ -e km0_boot_all.bin ] && rm km0_boot_all.bin
$SUDO [ -e km4_boot_all.bin ] && rm km4_boot_all.bin
$SUDO [ -e km0_km4_image2.bin ] && rm km0_km4_image2.bin
