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
# File   : artik05x_crashdump.sh
# Description : crachdump script for ARTIK05x


WD=$(cd `dirname $0` && pwd)

BUILD_DIR_PATH=${WD}/../..
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
OPENOCD_DIR_PATH=${BUILD_DIR_PATH}/tools/openocd

TIZENRT_BIN=$OUTPUT_BINARY_PATH/tinyara_head.bin
DUMP_PARTITION=""
FILE_NAME=""
erase=0
make=n

declare -a id desc addr size ro

usage() {
    cat <<EOF
USAGE: `basename $0` ["<partition>"] ["<filename>"]

For examples:
    `basename $0` "OTA download" crash_dump.bin

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


crash_dump()
{
    local SYSTEM_TYPE=`getconf LONG_BIT`
    local strcmd=""
    local bin=

    if [ "$SYSTEM_TYPE" = "64" ]; then
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
    else
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
    fi

    ${OPENOCD_BIN_PATH}/openocd -f artik05x.cfg -s $BOARD_DIR_PATH/../artik05x/scripts -c \
                   "init; flash_dump $DUMP_PARTITION $FILE_NAME; reset; exit"
}

#read partition table
partition
if test $# -eq 0; then
    usage 1>&2
    exit 1
fi

BOARD_NAME=$1
BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/$BOARD_NAME
DUMP_PARTITION=$2
FILE_NAME=$OUTPUT_BINARY_PATH/$3

crash_dump
