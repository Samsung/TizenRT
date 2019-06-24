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

# This script is used to auto generate board specific memory.ld file for PROTECTED_BUILD

THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
TOP_PATH=${THIS_PATH}/../..
OS_PATH=${TOP_PATH}/os

CONFIG=${OS_PATH}/.config
if [ ! -f ${CONFIG} ]; then
        echo "No .config file"
        exit 1
fi

source ${CONFIG}

LD_FILE=${TOP_PATH}/build/configs/${CONFIG_ARCH_BOARD}/scripts/memory_layout.ld

if [[ "${CONFIG_AUTOGEN_MEMORY_LDSCRIPT}" == "y" ]]; then
	echo "Generating ${LD_FILE} file..."
	rm ${LD_FILE}

	echo "/* This file is auto-generated based on values from .config file */" > ${LD_FILE}

	# Extract partition sizes form CONFIG_FLASH_PART_SIZE
	OIFS=$IFS
	IFS=","
	NAMES=($CONFIG_FLASH_PART_NAME)
	SIZES=($CONFIG_FLASH_PART_SIZE)

	# Get the flash partition sizes.
	for i in "${!NAMES[@]}"; do
		if [ "${NAMES[$i]}" = "kernel" ]; then
			let KFLASH_SZ=${SIZES[$i]}

		elif [ "${NAMES[$i]}" = "app" ]; then
			let UFLASH_SZ=${SIZES[$i]}

		fi
	done
	IFS=$OIFS

	KFLASH_START="$(printf 0x"%08X" $CONFIG_FLASH_START_ADDR)"
	let UFLASH_START=$KFLASH_START+$KFLASH_SZ*1024
	UFLASH_START="$(printf 0x"%08X" $UFLASH_START)"

	KSRAM_START="$(printf 0x"%08X" $CONFIG_SRAM_START_ADDR)"
	KSRAM_SZ=$CONFIG_KSRAM_SIZE
	let USRAM_START=$CONFIG_SRAM_START_ADDR+$KSRAM_SZ*1024
	USRAM_START="$(printf 0x"%08X" $USRAM_START)"
	USRAM_SZ=$CONFIG_USRAM_SIZE

	# Write data to the memory.ld file
	echo "" >> ${LD_FILE}
	echo "MEMORY" >> ${LD_FILE}
	echo "{" >> ${LD_FILE}
	echo "  kflash (rx)  : ORIGIN = $KFLASH_START, LENGTH = ${KFLASH_SZ}K" >> ${LD_FILE}
	echo "  uflash (rx)  : ORIGIN = $UFLASH_START, LENGTH = ${UFLASH_SZ}K" >> ${LD_FILE}
	echo "  ksram  (rwx)  : ORIGIN = $KSRAM_START, LENGTH = ${KSRAM_SZ}K" >> ${LD_FILE}
	echo "  usram  (rwx)  : ORIGIN = $USRAM_START, LENGTH = ${USRAM_SZ}K" >> ${LD_FILE}
	echo "}" >> ${LD_FILE}

	echo "Done"
fi
