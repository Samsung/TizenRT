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
# File 	 : trap.sh
# Description : RAM and USerfs extraction util

# Output PATH
BUILD_BIN_PATH=../../../build/output/bin

# Compile trap application
gcc trap_utils.c trap.c -o trap

# Run TRAP Tool
RAMDUMP_FILE=./ramdump_0x*.bin
rm -f ${RAMDUMP_FILE}
FSDUMP_FILE=./fsdump_0x*.bin
rm -f ${FSDUMP_FILE}
EXTFSDUMP_FILE=./ext_fsdump_*.bin
rm -f ${EXTFSDUMP_FILE}
ok=true

# Take user input for device port
echo Please enter serial port adapter:
echo For example: /dev/ttyUSB1 or /dev/ttyACM0

while [ $ok = true ]
do
echo Enter:
read dev_port
if sudo ./trap $dev_port;
	then
		ok=false
	else
		ok=true
fi
done

if "$ok"; then
echo "\nCopying..."
find ./ -iname "ramdump_0x*.bin"
echo "to  $(readlink -f ${BUILD_BIN_PATH})\n"
cp ${RAMDUMP_FILE} ${BUILD_BIN_PATH}
fi

# clean up
rm -f dump
#end
