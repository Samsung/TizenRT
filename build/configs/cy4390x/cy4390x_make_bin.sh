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
# File   : cy4390x_make_bin.sh
# Description : make cy4390x specific binary

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../os
BIN_DIR_PATH=${OS_DIR_PATH}/../build/output/bin

TIZENRT_BIN=${BIN_DIR_PATH}/tinyara
STRIP_FILE=${BIN_DIR_PATH}/tinyara_master_strip

make_strip_elf()
{
	if [ ! -f "${TIZENRT_BIN}" ]; then
		echo "TizenRT image is not present"
		exit 1
	fi
	arm-none-eabi-strip -s -o ${STRIP_FILE} ${TIZENRT_BIN}
	echo "Strip ELF DONE"
}

make_strip_elf
