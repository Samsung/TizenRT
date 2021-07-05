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
# imxrt1050-evk_download.sh

BOOTSTRAP_SCRIPT=${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/bootstrap.sh

#Bootstrap to set communicaiton with blhost
#Input: None
function bootstrap()
{
	source ${BOOTSTRAP_SCRIPT}

	${BLHOST} -p ${TTYDEV} -- fill-memory 0x2000 0x04 0xc0233007
	${BLHOST} -p ${TTYDEV} -- configure-memory 0x09 0x2000
}

#Utility function to erase a part of flash
#Input: Address and length
function flash_erase()
{
	echo -e "\nFLASH_ERASE: ADDR:$1 LENGTH:$2 KB"
	size_in_bytes=$(($2 * 1024))
	${BLHOST} -p ${TTYDEV} -- flash-erase-region $1 $size_in_bytes
}


#Utility function to write a binary on a flash partition
#Input: Address and filepath
function flash_write()
{
	echo -e "\nFLASH_WRITE ADDR:$1 \nFILEPATH:$2 "
	${BLHOST} -p ${TTYDEV} -- write-memory $1 $2
	sleep 2
}

function pre_download()
{
	bootstrap
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
		flash_erase $2 $4
		flash_write $2 ${BIN_PATH}/$3
	fi
}

function board_erase()
{
	flash_erase $2 $3
}
