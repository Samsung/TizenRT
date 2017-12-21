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
erase=0
make=n

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
USAGE: `basename $0` [OPTIONS] ["<partition>"]
OPTIONS:
    [--board[="<board-name>"]]
    [--secure[=<exec-path>]]
    [--verify]

PARTITIONS:
    [$OPT]

For examples:
    `basename $0` --board=artik053 ALL
    `basename $0` --board=artik053s --verify wlanfw
    `basename $0` --board=artik055s --secure=../codesigner os

Options:
    --board[="<board-name>"]      select target board-name
    --secure[=<exec-path>]        choose secure mode, and set the codesinger path
    --verify                      verify downloaded image if you need
    --erase                       erase the FLASH memory area corresponding to "<partition>".

    <partition>                   write each firmware image into FLASH

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
    target=(${target[@]} "$1")
}

make-target-bin() {
    local bin=
    local obj=
    local optarg=

    while test $# -gt 0; do
        optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'`

        case $1 in
            --target=*) t=$optarg ;;
            --make=*)   m=$optarg ;;
            --bin=*)    bin=$FW_DIR_PATH/$optarg.bin ;;
        esac
        shift
    done

    case $t in
        os) bin=`signing $TIZENRT_BIN` || return 1 ;;
        factory)
            if test "$m" = "n" ; then
                test ! -e $bin && return 1;
            else
                obj=`make-target-bin --target=os --make=y` || return 1
                gzip -c $obj > $bin
            fi ;;
        ota)
            if test "$m" = "n" ; then
                test ! -e $bin && return 1;
            else
                obj=`make-target-bin --target=factory --bin=tmp --make=y` || return 1
                dd if=$obj of=$bin bs=1024 seek=4;
                echo $(printf "%08x" $(cat $obj | wc -c)) | tac -rs .. | xxd -r -p | \
                    dd of=$bin bs=1 seek=0 conv=notrunc
                echo $(crc32 $obj) | tac -rs .. | xxd -r -p | \
                    dd of=$bin bs=1 seek=4 conv=notrunc
                rm -rf $obj
            fi ;;
        *)
            test ! -e $bin && return 1 ;;
    esac

    echo $bin

    return 0
}

download()
{
    local SYSTEM_TYPE=`getconf LONG_BIT`
    local strcmd=""
    local bin=

    if [ "$SYSTEM_TYPE" = "64" ]; then
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
    else
        local OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
    fi

    test -e $TIZENRT_BIN-signed && rm -rf $TIZENRT_BIN-signed

    for cmd in ${target[@]}; do
        if test $erase -eq 0; then
            bin=$(make-target-bin --target=$cmd --bin=$cmd --make=$make)
            # check existence of firmware binaries
            if test $? -eq 1; then
                echo " `echo $cmd | tr a-z A-Z` binary was not existed"
                exit 1
            fi

        fi

        if test "$make" = "n" ; then
            test $cmd = "bl1" && strcmd="$strcmd flash_protect off;"
            test $erase -eq 1 && \
                strcmd="$strcmd flash_erase_part $cmd;" || \
                strcmd="$strcmd flash_write $cmd $bin $VERIFY;"

            test $cmd = "bl1" && strcmd="$strcmd flash_protect on;"
        fi
    done

    # Download all binaries using openocd script
    if test "$make" = "n" ; then
        pushd ${OPENOCD_DIR_PATH} > /dev/null
            ${OPENOCD_BIN_PATH}/openocd -f artik05x.cfg -s $BOARD_DIR_PATH/../artik05x/scripts -c \
            "init; $strcmd reset; exit"
        popd > /dev/null
    fi
}

signing() {
    local bin=$1
    local codesigner=$secure_tool_path/artik05x_AppCodesigner
    if test ! -e $1; then
        echo "OS image file not found" >&2
        return 1
    fi

    if test ! -e "$1-signed" -a -e "$codesigner"; then
        $codesigner $secure_tool_path/rsa_private.key $1 >&2
        bin="$1-signed"
    fi

    echo $bin

    return 0;
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
        --secure=*) secure_tool_path=$(echo $optarg | sed -e "s#^\~#$HOME#")
            ;;
        --verify)
            VERIFY=verify
            ;;
        --make-only)
            make=y
            ;;
        erase)
            erase=1
            ;;
        ALL)
            add_target all
            ;;
        *)
            if test -n "`get_idx $1`"; then
                add_target $1
            else
                usage 1>&2
                exit 1
            fi
            ;;
    esac
    shift
done

if test -z "$target"; then
    echo "Not selected target"
    usage 1>&2
fi

test $erase -eq 0 -a $target = "all" && \
    target=(bl1 bl2 sssfw wlanfw os)
download
