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
# File   : stm32f4xx_download.sh
# Description : Download script for stm32f4xx boards

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../os

USBRULE_PATH=${THIS_PATH}/../usbrule.sh
USBRULE_BOARD="stm32"
USBRULE_IDVENDOR="0483"
USBRULE_IDPRODUCT=("3748" "3752" "3753" "374d" "374e" "374f")

source ${OS_DIR_PATH}/.config

BUILD_DIR_PATH=${OS_DIR_PATH}/../build
CONFIGS_DIR_PATH=${BUILD_DIR_PATH}/configs
OUTPUT_BINARY_PATH=${BUILD_DIR_PATH}/output/bin
TIZENRT_BIN=${OUTPUT_BINARY_PATH}/tinyara.bin

download_all()
{
	if ! type st-flash &>/dev/null; then
		echo "ST Link download tool is not present"
		echo "Refer README to install it"
		exit 1
	fi

	echo ""
	echo "downloading TIZENRT ..."

	st-flash write ${TIZENRT_BIN} 0x8000000

	if [ "$?" -eq "0" ]; then
		echo "TIZENRT download DONE!!"
	else
		echo "TIZENRT download FAILED!!"
	fi
}

case ${1,,} in
	os)
		download_all
		;;
	all)
		download_all
		;;
	usbrule)
		${USBRULE_PATH} ${USBRULE_BOARD} ${USBRULE_IDVENDOR} ${USBRULE_IDPRODUCT[@]} || exit 1
		;;
	*)
		echo "Not supported"
		;;
esac
