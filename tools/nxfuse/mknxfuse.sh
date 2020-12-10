#!/usr/bin/env bash
###########################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
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

NXFUSE_TOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

SRCDIR=$NXFUSE_TOOL_PATH/src
DEPDIR=$NXFUSE_TOOL_PATH/dep
OBJDIR=$NXFUSE_TOOL_PATH/obj
SMARTFS_TMPDIR=$SRCDIR/smartfs
BASE_DIR=$NXFUSE_TOOL_PATH/../..
SMARTFSDIR=$BASE_DIR/os/fs/smartfs
BASE_INCLUDE_DIR=$BASE_DIR/os/include
DEST_INLCUDE_DIR=$NXFUSE_TOOL_PATH/include

APPNAME=nxfuse

. $BASE_DIR/os/.config

echo "==========Copying Smartfs files====================="
mkdir -p $DEST_INLCUDE_DIR
mkdir -p $DEST_INLCUDE_DIR/sys
mkdir -p $DEST_INLCUDE_DIR/tinyara
mkdir -p $DEST_INLCUDE_DIR/tinyara/fs
mkdir -p $SMARTFS_TMPDIR

ln -sf $SMARTFSDIR/smartfs.h $SMARTFS_TMPDIR/smartfs.h
ln -sf $SMARTFSDIR/smartfs_utils.c $SMARTFS_TMPDIR/smartfs_utils.c
ln -sf $SMARTFSDIR/smartfs_smart.c $SMARTFS_TMPDIR/smartfs_smart.c
ln -sf $SMARTFSDIR/../driver/mtd/smart.c $SMARTFS_TMPDIR/smart.c
ln -svf $BASE_DIR/lib/libc/queue $SRCDIR/

#Header Files
ln -sf $BASE_INCLUDE_DIR/crc16.h $DEST_INLCUDE_DIR/crc16.h
ln -sf $BASE_INCLUDE_DIR/crc32.h $DEST_INLCUDE_DIR/crc32.h
ln -sf $BASE_INCLUDE_DIR/crc8.h $DEST_INLCUDE_DIR/crc8.h
ln -sf $BASE_INCLUDE_DIR/debug.h $DEST_INLCUDE_DIR/debug.h
ln -sf $BASE_INCLUDE_DIR/dirent.h $DEST_INLCUDE_DIR/dirent.h
ln -sf $BASE_INCLUDE_DIR/errno.h $DEST_INLCUDE_DIR/errno.h
ln -sf $BASE_INCLUDE_DIR/fixedmath.h $DEST_INLCUDE_DIR/fixedmath.h
ln -sf $BASE_INCLUDE_DIR/queue.h $DEST_INLCUDE_DIR/queue.h
ln -sf $BASE_INCLUDE_DIR/sys/statfs.h $DEST_INLCUDE_DIR/sys/statfs.h
ln -sf $BASE_INCLUDE_DIR/tinyara/math.h $DEST_INLCUDE_DIR/tinyara/math.h
ln -sf $BASE_INCLUDE_DIR/tinyara/mqueue.h $DEST_INLCUDE_DIR/tinyara/mqueue.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/mtd.h $DEST_INLCUDE_DIR/tinyara/fs/mtd.h
ln -sf $DEST_INLCUDE_DIR/tinyara/nxfuse_kmalloc.h $DEST_INLCUDE_DIR/tinyara/kmalloc.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/smart.h $DEST_INLCUDE_DIR/tinyara/fs/smart.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/smart_procfs.h $DEST_INLCUDE_DIR/tinyara/fs/smart_procfs.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/dirent.h $DEST_INLCUDE_DIR/tinyara/fs/dirent.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/ioctl.h $DEST_INLCUDE_DIR/tinyara/fs/ioctl.h
ln -sf $BASE_INCLUDE_DIR/tinyara/fs/fs.h $DEST_INLCUDE_DIR/tinyara/fs/fs.h

#Source Files
ln -sf $BASE_DIR/lib/libc/misc/lib_crc16.c $SRCDIR/lib_crc16.c
ln -sf $BASE_DIR/lib/libc/misc/lib_crc32.c $SRCDIR/lib_crc32.c
ln -sf $BASE_DIR/lib/libc/misc/lib_crc8.c $SRCDIR/lib_crc8.c

echo "Copying Done"

echo "============Executing Make command====================="
make -C $NXFUSE_TOOL_PATH || exit 1

#Waiting for make to complete
#After build done, remove the copied source & header files
rm -rf $SRCDIR/lib_crc*
rm -rf $DEST_INLCUDE_DIR/crc16.h
rm -rf $DEST_INLCUDE_DIR/crc32.h
rm -rf $DEST_INLCUDE_DIR/crc8.h
rm -rf $DEST_INLCUDE_DIR/debug.h
rm -rf $DEST_INLCUDE_DIR/dirent.h
rm -rf $DEST_INLCUDE_DIR/errno.h
rm -rf $DEST_INLCUDE_DIR/fixedmath.h
rm -rf $DEST_INLCUDE_DIR/queue.h
rm -rf $DEST_INLCUDE_DIR/sys/statfs.h
rm -rf $DEST_INLCUDE_DIR/tinyara/math.h
rm -rf $DEST_INLCUDE_DIR/tinyara/mqueue.h
rm -rf $DEST_INLCUDE_DIR/tinyara/kmalloc.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/mtd.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/smart.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/smart_procfs.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/dirent.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/fs.h
rm -rf $DEST_INLCUDE_DIR/tinyara/fs/ioctl.h

rm -rf $DEST_INLCUDE_DIR/sys
rm -rf $DEST_INLCUDE_DIR/tinyara/fs
rm -rf $SMARTFS_TMPDIR
rm -rf $SRCDIR/queue
rm -rf $DEPDIR
rm -rf $OBJDIR

echo -e "============Generated nxfuse in path ============ \nDone"
