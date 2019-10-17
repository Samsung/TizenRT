#!/bin/bash
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
# File : mksmartfsimg.sh

NXFUSE_TOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

BASE_DIR=$NXFUSE_TOOL_PATH/../..

#source $BASE_DIR/os/.config
. $BASE_DIR/os/.config

BOARDNAME=$CONFIG_ARCH_BOARD
POSTFIX=_smartfs.bin
BINDIR=$BASE_DIR/build/output/bin
BINNAME=$BOARDNAME$POSTFIX
CONTENTSDIR=$BASE_DIR/tools/fs/contents-smartfs
NXFUSEDIR=$NXFUSE_TOOL_PATH

# For the below values check partition sizes in .config
blkcount=512
blksize=$CONFIG_MTD_SMART_SECTOR_SIZE
pagesize=256
erasesize=4096

echo "============================================================="
echo "mksmartfsimg.sh : $BINNAME, Target Board: $BOARDNAME"
echo " - block count := $blkcount (bs=$blksize)"
echo "============================================================="

if [ ! -d $CONTENTSDIR/$BOARDNAME/mnt ]; then
mkdir -p $CONTENTSDIR/$BOARDNAME/mnt/
fi

#Make a dummy .bin file
touch $BINDIR/$BINNAME

# Making image file
dd if=/dev/zero of=$BINDIR/$BINNAME bs=$blksize count=$blkcount

#Copying the nxfuse
cp $NXFUSEDIR/nxfuse .

# Formatting
./nxfuse -p $pagesize -e $erasesize -l $blksize -t smartfs -m $BINDIR/$BINNAME || exit 1

# Mounting mnt
./nxfuse -p $pagesize -e $erasesize -l $blksize -t smartfs $CONTENTSDIR/$BOARDNAME/mnt $BINDIR/$BINNAME || exit 1

# Copying files to smartfs file system
cp -a $CONTENTSDIR/$BOARDNAME/base-files/* $CONTENTSDIR/$BOARDNAME/mnt/

# Unmounting
sleep 2
fusermount -u $CONTENTSDIR/$BOARDNAME/mnt || exit 1
rm -rf $CONTENTSDIR/$BOARDNAME/mnt/*
rm -rf nxfuse
echo "DONE"
