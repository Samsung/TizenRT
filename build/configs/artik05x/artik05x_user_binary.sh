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
# File   : artik05x_user_binary.sh
# Description : script of archive user binary for ARTIK 053

# Remember, make is invoked from "os" directory

usage() {
    cat <<EOF
USAGE: `basename $0` [OPTIONS]
OPTIONS:
    [--topdir[=<TOPDIR>]]
    [--board[="<board-name>"]]
    [--secure[=<exec-path>]]

For examples:
    `basename $0` --topdir=`pwd` --board=artik053
    `basename $0`--topdir=../ --board=artik055s --secure=./codesigner

Options:
    --topdir[="<TOPDIR>"]         assign the base path of tizenrt
    --board[="<board-name>"]      select target board-name
    --secure[=<exec-path>]        choose secure mode, and set the codesinger path

EOF
}

signing() {
    $CODESIGNER/artik05x_AppCodesigner $CODESIGNER/rsa_private.key $TIZENRT_IMAGE
    TIZENRT_IMAGE=${TIZENRT_IMAGE}-signed
}

factory-image() {
    if [ ! -e $1 ]; then
        exit 1;
    fi

    gzip -c $1 > $OUTPUT_PATH/bin/factoryimage.gz
}

download()
{
    local SYSTEM_TYPE=`getconf LONG_BIT`
    if [ "$SYSTEM_TYPE" = "64" ]; then
        local OPENOCD_BIN_PATH=${OPENOCD_PATH}/linux64
    else
        local OPENOCD_BIN_PATH=${OPENOCD_PATH}/linux32
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
        flash_write bl1 ${FW_DIR_PATH}/bl1.bin; 		\
        flash_write bl2 ${FW_DIR_PATH}/bl2.bin; 		\
        flash_write sssfw ${FW_DIR_PATH}/sssfw.bin; 		\
        flash_write wlanfw ${FW_DIR_PATH}/wlanfw.bin;	\
        flash_write os ../../../../output/bin/tinyara_head.bin;	\
        exit"
    popd > /dev/null
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
        --topdir=*) TOPDIR=$optarg ;;
        --secure=*)
            CODESIGNER=$optarg
            if [ ! -e $CODESIGNER ]; then
                echo "No Such as codesigner. Please check the path:"
                echo "   $CODESIGNER"
                exit 1
            fi
            ;;
        --board=*) BOARD_NAME=$optarg ;;
        *)
            usage 1>&2
            exit 1
            ;;
    esac
    shift
done

# chech the path
if [ ! -d $TOPDIR ]; then
    exit 1
fi

BUILD_PATH=${TOPDIR}/../build
CONFIGS_PATH=$BUILD_PATH/configs
OUTPUT_PATH=${BUILD_PATH}/output/
OPENOCD_PATH=${BUILD_PATH}/tools/openocd
TIZENRT_IMAGE=$OUTPUT_PATH/bin/tinyara_head.bin

if [ ! -d $BUILD_PATH ] || [ ! -d $CONFIGS_PATH ] || [ ! -d $OUTPUT_PATH ] || [ ! -d $OPENOCD_PATH ]; then
    echo "No such as target directory";
    exit 1;
fi

if [ ! -e $TIZENRT_IMAGE ]; then
    echo "can not found target binary : $TIZENRT_IMAGE";
    exit 1;
fi

if [ ! -z $CODESIGNER ]; then
    signing
fi

# make factory image
factory-image ${TIZENRT_IMAGE}

# Packaging

mkdir -p user_binary/boot_bin

## openocd
cp -rf $OPENOCD_PATH user_binary
cp -f $CONFIGS_PATH/artik05x/scripts/* user_binary/openocd/share/openocd/scripts

## boot binary
cp -rf $CONFIGS_PATH/$BOARD_NAME/bin/. user_binary/boot_bin

## tizenrt images
cp -rf ${TIZENRT_IMAGE} user_binary/boot_bin

## factory image
cp -rf $OUTPUT_PATH/bin/factoryimage.gz user_binary/boot_bin

## system map
cp -rf $OUTPUT_PATH/bin/System.map user_binary/boot_bin

## download script
tee user_binary/openocd/fusing_a05x.sh << __EOF__
#! /bin/bash
#

if [ "\$(uname -s)" == "Linux" ]; then
    if [ "\$(getconf LONG_BIT)" == "64" ]; then
        OPENOCD_BIN_PATH=linux64
    else
        OPENOCD_BIN_PATH=linux32
    fi
else
    OPENOCD_BIN_PATH=macos
fi

# Download all binaries using openocd script
\${OPENOCD_BIN_PATH}/openocd -f artik05x.cfg -c \
    "init; reset halt; \
    flash_protect off;\
    flash_write bl1 ../boot_bin/bl1.bin; 		\
    flash_protect on; \
    flash_write bl2 ../boot_bin/bl2.bin; 		\
    flash_write sssfw ../boot_bin/sssfw.bin; 		\
    flash_write wlanfw ../boot_bin/wlanfw.bin;	\
    flash_write os ../boot_bin/$(basename ${TIZENRT_IMAGE});	\
    flash_write factory ../boot_bin/factoryimage.gz;\
    flash_erase_part user; \
    flash_erase_part sssrw; \
    exit"
__EOF__

tee user_binary/openocd/fusing_a05x.bat << __EOF__
    win32\openocd.exe -f artik05x.cfg -c \
        "init;reset halt; \
        flash_protect off;\
        flash_write bl1 ../boot_bin/bl1.bin;\
        flash_protect on; \
        flash_write bl2 ../boot_bin/bl2.bin;\
        flash_write sssfw ../boot_bin/sssfw.bin;\
        flash_write wlanfw ../boot_bin/wlanfw.bin;\
        flash_write os ../boot_bin/$(basename ${TIZENRT_IMAGE});	\
        flash_write factory ../boot_bin/factoryimage.gz;\
        flash_erase_part user; \
        flash_erase_part sssrw; \
        exit"
__EOF__

chmod +x user_binary/openocd/fusing_a05x.*

# Archive
zip -r $OUTPUT_PATH/bin/$(date '+%Y%m%d-%H%M')_tizenRT100_User.zip user_binary/boot_bin/* user_binary/openocd/*

# clean
rm -rf user_binary
