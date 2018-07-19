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
# File          : artik05x_cmn.sh
# Description   : Common file for other *sh in directory

set -e

# Remember, make is invoked from "os" directory
OS_DIR_PATH=${PWD}
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
CONFIGS_DIR_PATH=${BUILD_DIR_PATH}/configs
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
ARTIK05X_DIR_PATH=${CONFIGS_DIR_PATH}/artik05x
TIZENRT_BIN=$OUTPUT_BINARY_PATH/tinyara_head.bin
CODESIGNER_TOOL=artik05x_AppCodesigner
CODESIGNER_PATH=${ARTIK05X_DIR_PATH}/tools/codesigner
CODESIGNER_DIR_PATH=${ARTIK05X_DIR_PATH}/tools/codesigner
OPENOCD_DIR_PATH=${BUILD_DIR_PATH}/tools/openocd

if [[ $OSTYPE == "darwin"* ]]; then
	OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/macos
	CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/macos
elif [[ $OSTYPE == "linux"* ]]; then
	SYSTEM_TYPE=`getconf LONG_BIT`
	if [ "$SYSTEM_TYPE" = "64" ]; then
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
		CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/linux64
	else
		OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
		CODESIGNER_PATH=${CODESIGNER_DIR_PATH}/linux32
	fi
else
    echo "Doesn’t support Host OS: $OSTYPE"
    exit 1
fi

OPENOCD=${OPENOCD_BIN_PATH}/openocd

signing() {
	if [ ! -f ${CODESIGNER_PATH}/${CODESIGNER_TOOL} ]; then
		echo "${CODESIGNER_TOOL} should be in ${CODESIGNER_PATH} to use secure boards like ARTIK053S, ARTIK055S."
		exit 1
	fi

	${CODESIGNER_PATH}/${CODESIGNER_TOOL} ${CODESIGNER_DIR_PATH}/rsa_private.key $TIZENRT_BIN
	TIZENRT_BIN=${TIZENRT_BIN}-signed
}


