#!/bin/bash
###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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
# File          : artik05x_make_bin.sh
# Description   : make board-specific binary for ARTIK boards like signing, adding checksum

set -e

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../os
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
CONFIGS_DIR_PATH=${BUILD_DIR_PATH}/configs
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
ARTIK05X_DIR_PATH=${CONFIGS_DIR_PATH}/artik05x
CODESIGNER_TOOL=artik05x_AppCodesigner
CODESIGNER_DIR_PATH=${ARTIK05X_DIR_PATH}/tools/codesigner

while test $# -gt 0; do
	case $1 in
		--bin)
			shift
			BIN_NAME=$1
			;;
		--ext)
			shift
			EXT_NAME=$1
			;;
		--secure)
			SECURE_BOARD=y
			;;
	esac
	shift
done


TIZENRT_BIN=${BIN_NAME}.${EXT_NAME}
SAMSUNG_NS2_BIN=${BIN_NAME}_head.${EXT_NAME}

if [ "${TIZENRT_BIN}" == "." ]; then
	echo "Error!! Fail to make a binary for ARTIK. Please check --bin and --ext options"
	exit 1
fi

if [[ $OSTYPE == "darwin"* ]]; then
	CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/macos
elif [[ $OSTYPE == "linux"* ]]; then
	SYSTEM_TYPE=`getconf LONG_BIT`
	if [ "$SYSTEM_TYPE" = "64" ]; then
		CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/linux64
	else
		CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/linux32
	fi
else
    echo "Doesn’t support Host OS: $OSTYPE"
    exit 1
fi

add_checksum() {
	echo "ATTACHNS2: ${TIZENRT_BIN} --> ${SAMSUNG_NS2_BIN}"
	${OS_DIR_PATH}/tools/s5jchksum.py ${OUTPUT_BINARY_PATH}/${TIZENRT_BIN} ${OUTPUT_BINARY_PATH}/${SAMSUNG_NS2_BIN}
}

signing() {
	if [ ! -f ${CODESIGNER_PATH}/${CODESIGNER_TOOL} ]; then
		echo "${CODESIGNER_TOOL} should be in ${CODESIGNER_PATH} to use secure boards like ARTIK053S, ARTIK055S."
		exit 1
	fi

	${CODESIGNER_PATH}/${CODESIGNER_TOOL} ${CODESIGNER_DIR_PATH}/rsa_private.key ${OUTPUT_BINARY_PATH}/${SAMSUNG_NS2_BIN}
}

add_checksum
if [ "${SECURE_BOARD}" == "y" ]; then
	signing
fi
