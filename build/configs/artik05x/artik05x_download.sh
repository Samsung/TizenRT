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
# Description : Download script for ARTIK 053

# Remember, make is invoked from "os" directory

OS_DIR_PATH=${PWD}
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
CONFIGS_PATH=${BUILD_DIR_PATH}/configs
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
OPENOCD_DIR_PATH=${BUILD_DIR_PATH}/tools/openocd

TIZENRT_BIN=$OUTPUT_BINARY_PATH/tinyara_head.bin

usage() {
    cat <<EOF
USAGE: `basename $0` [OPTIONS]
OPTIONS:
    [--board[="<board-name>"]]
    [--secure[=<exec-path>]]
    [ALL | BOOTLOADER | RESOURCE | KERNEL | THIRDPARTY]

For examples:
    `basename $0` --board=artik053 ALL
    `basename $0` --board=artik055s --secure=../codesigner BOOTLOADER

Options:
    --board[="<board-name>"]      select target board-name
    --secure[=<exec-path>]        choose secure mode, and set the codesinger path

    ALL                           write each firmware image into FLASH
    BOOTLOADER                    not supported yet
    RESOURCE                      not supported yet
    KERNEL                        not supported yet
    THIRDPARTY                    not supported yet

EOF
}

download()
{
    local SYSTEM_TYPE=`getconf LONG_BIT`
    if [ "$SYSTEM_TYPE" = "64" ]; then
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
    else
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
    fi

    # check existence of os binary
    if [ ! -f "${OUTPUT_BINARY_PATH}/tinyara_head.bin" ]; then
        echo "TinyAra binary is not existed, build first"
        exit 1
    fi

    # check existence of firmware binaries
    if [ ! -f "${FW_DIR_PATH}/bl1.bin" ] ||\
        [ ! -f "${FW_DIR_PATH}/bl2.bin" ] ||\
        [ ! -f "${FW_DIR_PATH}/sssfw.bin" ] ||\
        [ ! -f "${FW_DIR_PATH}/wlanfw.bin" ]; then
        echo "Firmware binaries for ARTIK 05X are not existed"
        exit 1
    fi

    # Download all binaries using openocd script
    pushd ${OPENOCD_DIR_PATH} > /dev/null
    ${OPENOCD_BIN_PATH}/openocd -f artik05x.cfg -s "$BOARD_DIR_PATH/../artik05x/scripts" -c " 	\
        flash_protect off;\
        flash_write bl1 ${FW_DIR_PATH}/bl1.bin; 		\
        flash_protect on; \
        flash_write bl2 ${FW_DIR_PATH}/bl2.bin; 		\
        flash_write sssfw ${FW_DIR_PATH}/sssfw.bin; 		\
        flash_write wlanfw ${FW_DIR_PATH}/wlanfw.bin;	\
        flash_write os ${TIZENRT_BIN};	\
        exit"
    popd > /dev/null
}

signing() {
    CODESIGNER=$1
    if [ ! -e $CODESIGNER ]; then
        echo "No Such as codesigner. Please check the path:"
        echo "   $CODESIGNER"
        exit 1
    fi
    $CODESIGNER -sign $TIZENRT_BIN
    TIZENRT_BIN=${TIZENRT_BIN}-signed
}

if test $# -eq 0; then
    usage 1>&2
    exit 1
fi

while test $# -gt 0; do
    case "$1" in
        -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
        *) optarg= ;;
    esac

    case $1 in
        --board*)
            BOARD_NAME=$optarg
            BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/$BOARD_NAME
            FW_DIR_PATH=${BOARD_DIR_PATH}/bin
            if [ ! -d $BOARD_DIR_PATH ]; then
                usage 1>&2
                exit 1
            fi
            ;;
        --secure=*) signing $optarg
            ;;
        ALL)
            download
            ;;
        *)
            usage 1>&2
            exit 1
            ;;
    esac
    shift
done


