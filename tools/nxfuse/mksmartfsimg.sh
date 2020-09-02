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
parts=${CONFIG_FLASH_PART_NAME}
IFS=',' read -ra parts <<< "$parts"
smartidx=-1
for i in "${!parts[@]}"
do
	if [[ "${parts[$i]}" = "userfs" ]]; then
		smartidx=$i
	fi
done
if [[ ${smartidx} = -1 ]]; then
	echo "ERROR: Failed to find userfs partition in config file!!!"
	exit
fi
sizes=${CONFIG_FLASH_PART_SIZE}
IFS=',' read -ra sizes <<< "$sizes"
smartsize=$((sizes[$smartidx] * 1024))
blksize=$CONFIG_MTD_SMART_SECTOR_SIZE
blkcount=$((${smartsize}/${blksize}))
pagesize=256
erasesize=4096

echo "============================================================="
echo "mksmartfsimg.sh : $BINNAME, Target Board: $BOARDNAME"
echo " - block count := $blkcount (bs=$blksize)"
echo "============================================================="

if [ ! -d $CONTENTSDIR/$BOARDNAME/mnt ]; then
mkdir -p $CONTENTSDIR/$BOARDNAME/mnt/
if [[ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]]; then
mkdir $CONTENTSDIR/$BOARDNAME/mnt/bins
fi
fi

echo "make a dummy"
#Make a dummy .bin file
touch $BINDIR/$BINNAME

echo "make image file"
# Making image file
if [ "${CONFIG_SMARTFS_ERASEDSTATE}" == 0xff ]; then
	dd if=/dev/zero bs=$blksize count=$blkcount | tr "\000" "\377" > $BINDIR/$BINNAME
else
	dd if=/dev/zero of=$BINDIR/$BINNAME bs=$blksize count=$blkcount
fi

echo "copy the nxfuse"
#Copying the nxfuse
cp $NXFUSEDIR/nxfuse .


echo "Now format image"
# Formatting
./nxfuse -p $pagesize -e $erasesize -l $blksize -t smartfs -m $BINDIR/$BINNAME || exit 1

echo "mount binary"
# Mounting mnt
./nxfuse -o nonempty -p $pagesize -e $erasesize -l $blksize -t smartfs $CONTENTSDIR/$BOARDNAME/mnt $BINDIR/$BINNAME || exit 1

echo "Copy files"
# Copying files to smartfs file system
cp -rf $CONTENTSDIR/$BOARDNAME/base-files/* $CONTENTSDIR/$BOARDNAME/mnt/

# Unmounting
sleep 2
fusermount -u $CONTENTSDIR/$BOARDNAME/mnt || exit 1
rm -rf $CONTENTSDIR/$BOARDNAME/mnt/*
rm -rf nxfuse
echo "DONE"
