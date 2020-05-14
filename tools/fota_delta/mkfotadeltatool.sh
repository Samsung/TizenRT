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

echo "Making FOTA Delta tool"

# Environmental Stuff

FOTA_DELTA_TOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
#When location of this script is changed, only OS_PATH should be changed !!!
OS_PATH=$FOTA_DELTA_TOOL_PATH/../../os
SRCDIR=$FOTA_DELTA_TOOL_PATH/src
SRC_INC_DIR=$OS_PATH/include
DEST_INC_DIR=$FOTA_DELTA_TOOL_PATH/include

#load .config file
source $OS_PATH/.config

APPNAME=mkfotadelta

# Soft Link to needed header files
mkdir -p $DEST_INC_DIR/
mkdir -p $DEST_INC_DIR/tinyara/
mkdir -p $DEST_INC_DIR/tinyara/binfmt/compression/
mkdir -p $DEST_INC_DIR/apps/system/
mkdir -p $DEST_INC_DIR/tinyara/zlib/
mkdir -p $DEST_INC_DIR/tinyara/lzma/

ln -sf $SRC_INC_DIR/tinyara/config.h $DEST_INC_DIR/tinyara/config.h
ln -sf $SRC_INC_DIR/tinyara/compiler.h $DEST_INC_DIR/tinyara/compiler.h
ln -sf $SRC_INC_DIR/tinyara/binfmt/compression/compression.h $DEST_INC_DIR/tinyara/binfmt/compression/compression.h
ln -sf $SRC_INC_DIR/apps/system/binary_header.h $DEST_INC_DIR/apps/system/binary_header.h
ln -sf $SRC_INC_DIR/apps/system/diff.h $DEST_INC_DIR/apps/system/diff.h
ln -sf $SRC_INC_DIR/tinyara/zlib/zlib.h $DEST_INC_DIR/tinyara/zlib/zlib.h
ln -sf $SRC_INC_DIR/tinyara/zlib/zconf.h $DEST_INC_DIR/tinyara/zlib/zconf.h
ln -sf $SRC_INC_DIR/tinyara/lzma/LzmaLib.h $DEST_INC_DIR/tinyara/lzma/LzmaLib.h
ln -sf $SRC_INC_DIR/tinyara/lzma/7zTypes.h $DEST_INC_DIR/tinyara/lzma/7zTypes.h

# Compression Algorithm specific initialization
if [ $CONFIG_COMPRESSION_TYPE == 1 ]
then
	mkdir -p $SRCDIR/lzma
	TMPDIR=$SRCDIR/lzma
	SOURCEDIR=$OS_PATH/../external/lzma

	echo $TMPDIR
	echo $SOURCEDIR
	echo "==========Copying lzma files====================="
	cp $SOURCEDIR/*.c $TMPDIR/
	cp $SOURCEDIR/*.h $TMPDIR/

	echo "Copying Done for LZMA files"
fi

# ZLIB specific initialization
mkdir -p $SRCDIR/zlib
TMPDIR=$SRCDIR/zlib
SOURCEDIR=$OS_PATH/../external/zlib

echo $TMPDIR
echo $SOURCEDIR
echo "==========Copying zlib files====================="
cp $SOURCEDIR/*.c $TMPDIR/
cp $SOURCEDIR/*.h $TMPDIR/

echo "Copying Done for ZLIB files"

echo "============Executing Make command====================="
make -C $FOTA_DELTA_TOOL_PATH

#Waiting for make to complete

rm -rf $TMPDIR/*

echo -e "Generated FOTA Delta tool \nDone"
