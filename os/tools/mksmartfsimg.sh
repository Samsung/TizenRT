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

source .config

BOARDNAME=$CONFIG_ARCH_BOARD
POSTFIX=_smartfs.bin
BINNAME=$BOARDNAME$POSTFIX

blkcount=8192
x=$CONFIG_BCM4390X_BOOTLOADER_REGION_SIZE
(( blkcount -= x ))

echo "============================================================="
echo "mksmartfsimg.sh : $BINNAME, Target Board: $BOARDNAME"
echo " - block count := $blkcount (bs=1024)"
echo "============================================================="

if [ ! -d ../external/contents/$BOARDNAME/mnt ]; then
mkdir ../external/contents/$BOARDNAME/mnt/
fi

# Making image file
dd if=/dev/zero of=../build/bin/$BINNAME bs=1024 count=$blkcount

# Formatting
tools/nxfuse -t smartfs -g 1 -m ../build/bin/$BINNAME

# Mounting mnt
tools/nxfuse -t smartfs ../external/contents/$BOARDNAME/mnt ../build/bin/$BINNAME

# Copying files to smartfs file system
cp -rf ../external/contents/$BOARDNAME/base-files/* ../external/contents/$BOARDNAME/mnt/

# Unmounting
sleep 2
fusermount -u ../external/contents/$BOARDNAME/mnt
echo "DONE"
