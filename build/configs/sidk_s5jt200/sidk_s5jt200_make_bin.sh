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
# File          : sidk_s5jt200_make_bin.sh
# Description   : make SIDK_S5JT200 binary

set -e

# Remember, make is invoked from "os" directory
THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../os
OUTPUT_BINARY_PATH=${OS_DIR_PATH}/../build/output/bin

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
	esac
	shift
done

TIZENRT_BIN=${BIN_NAME}.${EXT_NAME}
SAMSUNG_NS2_BIN=${BIN_NAME}_head.${EXT_NAME}

if [ "${TIZENRT_BIN}" == "." ]; then
	echo "Error!! Fail to make a binary for SIDK_S5JT200. Please check --bin and --ext options"
	exit 1
fi

add_checksum() {
	echo "ATTACHNS2: ${TIZENRT_BIN} --> ${SAMSUNG_NS2_BIN}"
	${OS_DIR_PATH}/tools/s5jchksum.py ${OUTPUT_BINARY_PATH}/${TIZENRT_BIN} ${OUTPUT_BINARY_PATH}/${SAMSUNG_NS2_BIN}
}

add_checksum
