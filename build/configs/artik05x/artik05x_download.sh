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

WD=$(cd `dirname $0` && pwd)

OS_DIR_PATH=${WD}/../../../os
BUILD_DIR_PATH=${WD}/../..
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
OPENOCD_DIR_PATH=${BUILD_DIR_PATH}/tools/openocd

TIZENRT_BIN=$OUTPUT_BINARY_PATH/tinyara_head.bin

declare -a id desc addr size ro

usage() {
    local OPT="ALL"
    for n in ${id[@]}; do
        test $n = "os" && bin=${TIZENRT_BIN} || bin="${FW_DIR_PATH}/$n.bin"
        if test -f $bin; then
            OPT="$OPT | $n"
        fi
    done

    cat <<EOF
USAGE: `basename $0` [OPTIONS]
OPTIONS:
    [--board[="<board-name>"]]
    [--secure[=<exec-path>]]
      [$OPT]

For examples:
    `basename $0` --board=artik053 ALL
    `basename $0` --board=artik053s --verify wlanfw
    `basename $0` --board=artik055s --secure=../codesigner os

Options:
    --board[="<board-name>"]      select target board-name
    --secure[=<exec-path>]        choose secure mode, and set the codesinger path
    --verify                      verify downloaded image if you need
    $OPT
                                  write each firmware image into FLASH
EOF
}

partition() {
    local START=0

    while read line
    do
        test `expr match "$line" '^set\ \w*.\W$'` -ne 0 && START=1 && continue;
        test `expr index "$line" }` -eq 1 && START=0 && continue;

        if test $START -eq 1; then
            local NAME_IDX=`expr index "$line" '[^"]'`
            local NAME_LEN=`expr index "${line:NAME_IDX}" '[^"]'`
            local TMP=${line:NAME_IDX+NAME_LEN}

            id=(${id[@]} `echo $line | awk '{print $1}'`)
            desc=(${desc[@]} `echo ${line:NAME_IDX:NAME_LEN-1}`)
            addr=(${addr[@]} `echo $TMP | awk '{print $1}'`)
            size=(${size[@]} `echo $TMP | awk '{print $2}'`)
            ro=(${ro[@]} `echo $TMP | awk '{print $3}'`)
        fi
    done < $WD/scripts/partition_map.cfg
}

get_idx() {
    local field=$1
    for i in ${!id[*]}; do
        if test ${id[$i]} = $field; then
            echo $i;
        fi
    done
}

add_target() {
    local field=$1
    local bin=""
    local protect=0

    test $field = "os" && bin=${TIZENRT_BIN} || bin="${FW_DIR_PATH}/$field.bin"
    test $field = "bl1" && protect=1;

    # check existence of firmware binaries
    if test ! -f $bin; then
        echo "Firmware binaries for ARTIK 05X are not existed"
        echo "   TARGET: $bin"
        exit 1
    fi

    test $protect -eq 1 && target="$target flash_protect off;"
    target="$target flash_write $1 $bin $VERIFY;"
    test $protect -eq 1 && target="$target flash_protect on;"

}

download()
{
    local SYSTEM_TYPE=`getconf LONG_BIT`
    if [ "$SYSTEM_TYPE" = "64" ]; then
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
    else
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
    fi

    # Download all binaries using openocd script
    pushd ${OPENOCD_DIR_PATH} > /dev/null
        ${OPENOCD_BIN_PATH}/openocd -f artik05x.cfg -s $BOARD_DIR_PATH/../artik05x/scripts -c \
        "init; $target reset; exit"
    popd > /dev/null
}

signing() {
    CODESIGNER=$1
    $CODESIGNER/artik05x_AppCodesigner $CODESIGNER/rsa_private.key $TIZENRT_BIN
    TIZENRT_BIN=${TIZENRT_BIN}-signed
}

#read partition table
partition

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
        --verify)
            VERIFY=verify
            ;;
        ALL)
            add_target bl1
            add_target bl2
            add_target sssfw
            add_target wlanfw
            add_target os
            download
            ;;
        *)
            if test -n "`get_idx $1`"; then
                echo DOWNLOAD: $1 | tr [a-z] [A-Z]
                add_target $1
                download
            else
                usage 1>&2
                exit 1
            fi
            ;;
    esac
    shift
done


