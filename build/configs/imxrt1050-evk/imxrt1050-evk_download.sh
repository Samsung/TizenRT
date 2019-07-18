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

WARNING="\n########################################\n
	WARNING:If download fails, don't forget to replug\n
	and reset the board before next try!!\n
	#################################################\n"

echo -e $WARNING

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only TOP_PATH should be changed together!!!
TOP_PATH=${THIS_PATH}/../../..
BOOTSTRAP_SCRIPT=${THIS_PATH}/bootstrap.sh

OS_PATH=${TOP_PATH}/os
OUTBIN_PATH=${TOP_PATH}/build/output/bin
TTYDEV="/dev/ttyACM0"
SUDO=sudo

# Following to read partition information
PARTITION_KCONFIG=${OS_PATH}/board/common/Kconfig
source ${OS_PATH}/.config

FLASH_START_ADDR=0x60000000
TINYARA_BIN=${OUTBIN_PATH}/tinyara.bin

if [ ! -f ${TINYARA_BIN} ]; then
	echo "missing file ${TINYARA_BIN}"
	exit 1
fi

if fuser -s ${TTYDEV}
then 
	echo "${TTYDEV} is used by another process, can't proceed"
	exit 1
fi

#Bootstrap to set communicaiton with blhost
#Input: None
function bootstrap()
{
	source ${BOOTSTRAP_SCRIPT}
	sleep 1

	${BLHOST} -p ${TTYDEV} -- fill-memory 0x2000 0x04 0xc0233007
	sleep 1
	${BLHOST} -p ${TTYDEV} -- configure-memory 0x09 0x2000
}

#Utility function to erase a part of flash
#Input: Address and length
function flash_erase()
{
	${SUDO} ${BLHOST} -p ${TTYDEV} -- flash-erase-region $1 $2
	echo "Successfull erased flash region from $1 of size $2 KB"
}


#Utility function to write a binary on a flash partition
#Input: Address and filepath
function flash_write()
{
	${SUDO} ${BLHOST} -p ${TTYDEV} -- write-memory $1 $2
}

#Utility function to match partition name to binary name
#NOTE: Hard-coded for now
function get_executable_name()
{
	case $1 in
		kernel) echo "tinyara.bin";;
		app) echo "tinyara_user.bin";;
		micom) echo "micom";;
		wifi) echo "wifi";;
		*) echo "No Binary Match"
	esac
}	

# Start here
# Read Partitions
if [[ -z ${CONFIG_FLASH_PART_NAME} ]]
then
	parts=`grep -A 2 'config FLASH_PART_NAME' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	echo -e "Using default partitions from Kconfig file\n"
else
	parts=${CONFIG_FLASH_PART_NAME}
	echo -e "Using partitions from .config file\n"
fi
IFS=',' read -ra parts <<< "$parts"

#Read Partition Sizes
if [[ -z ${CONFIG_FLASH_PART_SIZE} ]]
then
	sizes_str=`grep -A 2 'config FLASH_PART_SIZE' ${PARTITION_KCONFIG} | sed -n 's/\tdefault "\(.*\)".*/\1/p'`
	echo -e "Using default sizes from Kconfig file\n"
else
	sizes_str=${CONFIG_FLASH_PART_SIZE}
	echo -e "Using sizes from .config file\n"
fi
IFS=',' read -ra sizes <<< "$sizes_str"

#Calculate Flash Offset
num=${#sizes[@]}
offsets[0]=`printf "0x%X" ${FLASH_START_ADDR}`

for (( i=1; i<=$num-1; i++ ))
do
	val=$((sizes[$i-1] * 1024))

	offsets[$i]=`printf "0x%X" $((offsets[$i-1] + ${val}))`
done

#Dump Info
echo "offsets: ${offsets[@]}"
echo "sizes: ${sizes[@]}"
echo "partitions: ${parts[@]}"

#bootstrap
bootstrap

if [[ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]]
then
	#Erase All Partitions
	for (( i=0; i<$num; i++ ))
	do
		flash_erase ${offsets[$i]} ${sizes[$i]}
	done

	#Write binaries
	for i in 0 1 2 4
	do
		exec=$(get_executable_name ${parts[$i]})
		echo "Gonna Write ${exec} into offset ${offsets[$i]}"
		flash_write ${offsets[$i]} ${OUTBIN_PATH}/${exec}
	done

elif [[ "${CONFIG_BUILD_PROTECTED}" == "y" ]]
then
	#Erase All Partitions
	for (( i=0; i<$num; i++ ))
	do
		flash_erase ${offsets[$i]} ${sizes[$i]}
	done

	#Write binaries
	for i in 0 1
	do
		exec=$(get_executable_name ${parts[$i]})
		echo "Gonna Write ${exec} into offset ${offsets[$i]}"
		flash_write ${offsets[$i]} ${OUTBIN_PATH}/${exec}
	done
else
	#Erase All Prev Partitions
	PREV_PART_SIZE=`printf "0x%X" $((256 * 1024 * 8))`
	flash_erase ${FLASH_START_ADDR} ${PREV_PART_SIZE}
	flash_write ${FLASH_START_ADDR} ${TINYARA_BIN}
fi	
