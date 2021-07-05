#!/usr/bin/env bash
###########################################################################
#
# Copyright 2019 NXP Semiconductors All Rights Reserved.
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
# imxrt1020-evk_download.sh

##Utility Function to Bootstrap and configure memory##
function imxrt1020_bootstrap()
{
	source ${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/bootstrap.sh
	${BLHOST} -p $TTYDEV -- fill-memory 0x2000 0x04 0xc0000006
	${BLHOST} -p $TTYDEV -- configure-memory 0x09 0x2000
}

##Utility function to erase a part of flash##
#Input: Address and length
function flash_erase()
{
	echo -e "\nFLASH_ERASE: ADDR:$2 LENGTH:$3 KB"
	size_in_bytes=$(($3 * 1024))
	${BLHOST} -p $1 -- flash-erase-region $2 $size_in_bytes
}

##Utility function to write a binary on a flash partition##
#Input: Address and filepath
function flash_write()
{
	echo -e "\nFLASH_WRITE ADDR:$2 \nFILEPATH:$3 "
	${BLHOST} -p $1 -- write-memory $2 $3
	sleep 2
}

function pre_download()
{
	imxrt1020_bootstrap
}

function post_download()
{
	:;
}

function board_download()
{
	if [ ! -f ${BIN_PATH}/$3 ];then
		echo "${BIN_PATH}/$3 not found"
	else
		echo "${BIN_PATH}/$3"
		flash_erase $1 $2 $4
		flash_write $1 $2 ${BIN_PATH}/$3
	fi
}

function board_erase()
{
	flash_erase $1 $2 $3
}
