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

echo "Making compression tool"

# Environmental Stuff

COMPRESSION_TOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
#When location of this script is changed, only OS_PATH should be changed !!!
OS_PATH=$COMPRESSION_TOOL_PATH/../..
SRCDIR=$COMPRESSION_TOOL_PATH/src

#load .config file
source $OS_PATH/.config

# Compression Algorithm specific initialization
if [ $CONFIG_COMPRESSION_TYPE == 1 ]
then
	mkdir -p $SRCDIR/lzma
	TMPDIR=$SRCDIR/lzma
	SOURCEDIR=$OS_PATH/../external/lzma
elif [ $CONFIG_COMPRESSION_TYPE == 2 ]
then
	mkdir -p $SRCDIR/miniz
	TMPDIR=$SRCDIR/miniz
	SOURCEDIR=$OS_PATH/../external/miniz
fi

APPNAME=mkcompressimg

echo $TMPDIR
echo $SOURCEDIR
echo "==========Copying lzma files====================="
cp $SOURCEDIR/*.* $TMPDIR/

echo "Copying Done"

echo "============Executing Make command====================="
make -C $COMPRESSION_TOOL_PATH

#Waiting for make to complete

rm -rf $TMPDIR/*

echo -e "Generated compression tool \nDone"
