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
# File 	 : ramdump.sh
# Description : ramdump extract util

# Output PATH
BUILD_BIN_PATH=../../build/output/bin

# Compile ramdump application
gcc ramdump_tool.c -o ramdump

# Run Ramdump Tool
RAMDUMP_FILE=./ramdump_0x*.bin
rm -f ${RAMDUMP_FILE}
sudo ./ramdump /dev/ttyUSB1
echo "copying $(basename ${RAMDUMP_FILE}) to  $(readlink -f ${BUILD_BIN_PATH})"

cp ${RAMDUMP_FILE} ${BUILD_BIN_PATH}


# clean up
rm -f ramdump

#end

