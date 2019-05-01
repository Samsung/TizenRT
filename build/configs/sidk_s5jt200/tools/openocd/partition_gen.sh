#!/bin/bash

###########################################################################
#
# Copyright 2016-2017 Samsung Electronics All Rights Reserved.
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

# This script reads partition size and name list from .config and
# creates a partition map cfg file for openocd.
# Created partition map cfg file can be included in the main openocd cfg script
# for flashing.

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=${THIS_PATH}/../../../../../os

source ${OS_DIR_PATH}/.config

# Path ENV
BOARD_NAME=${CONFIG_ARCH_BOARD}
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/${BOARD_NAME}
OPENOCD_DIR_PATH=${BOARD_DIR_PATH}/tools/openocd

#FLASH BASE ADDRESS (Can it be made to read dynamically from .config?)
FLASH_BASE=0x04000000

partsize_list=${CONFIG_SIDK_S5JT200_FLASH_PART_LIST}
partname_list=${CONFIG_SIDK_S5JT200_FLASH_PART_NAME}

# OpenOCD cfg file to be created for flashing
PARTITION_MAP_CFG=${OPENOCD_DIR_PATH}/partition_map.cfg

#Comma Separator
IFS=","

#Variables
total=1
count=1
id=1
romfs_part_exist=0

#Array to hold partition size of all partitions
part_size[$count]=0

#Array to hold partition start address  of all partitions
part_start[$count]=0

#Loop partition size list
for psize in $partsize_list
do
	while [ "$count" -le "$total" ];
	do
		sum=`expr $sum + $psize`
		let "count += 1"
	done

	part_size[$count-1]=`expr $psize \\* 1024`
	part_start[$count]=`expr $sum \\* 1024`
	let "total += 1"
done

#Generate cfg file by looping partition name list

echo -n "Generating Partition Map CFG file ..."

echo "# Partition Map (Auto generated)" > ${PARTITION_MAP_CFG}
echo "set FLASH_START $FLASH_BASE" >> ${PARTITION_MAP_CFG}
for pname in $partname_list
do
	if [ "$pname" == "rom" ]; then
		romfs_part_exist=1
	fi

	echo "set ${pname}_part_start [expr $(printf 0x%X ${part_start[$id]})+""$""FLASH_START]" >> ${PARTITION_MAP_CFG}
	echo "set ${pname}_part_size $(printf 0x%X ${part_size[$id]})">> ${PARTITION_MAP_CFG}
	let "id += 1"
done

echo "# Configuration Variables" >> ${PARTITION_MAP_CFG}

if [ "${CONFIG_FS_ROMFS}" == "y" ] && [ "${romfs_part_exist}" == "1" ]; then
	echo "set romfs_partition_enable 1" >> ${PARTITION_MAP_CFG}
else
	echo "set romfs_partition_enable 0" >> ${PARTITION_MAP_CFG}
fi

echo "Done"
