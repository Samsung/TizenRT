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
#
# File   : sidk_s5jt200_download.sh
# Description : interface script for sidk_s5jt200 download

# Include the configuration file
source .config

# Board name for which tinyara has been compiled
BOARD_NAME=${CONFIG_ARCH_BOARD}

# ENV : Set to proper path's
OS_DIR_PATH=${PWD}
BUILD_DIR_PATH=${OS_DIR_PATH}/../build
OUTPUT_BIN_PATH=${BUILD_DIR_PATH}/output/bin
BOARD_DIR_PATH=${BUILD_DIR_PATH}/configs/${BOARD_NAME}
OPENOCD_DIR_PATH=${BOARD_DIR_PATH}/tools/openocd
FW_DIR_PATH=${BOARD_DIR_PATH}/boot_bin
FSTOOLS_DIR_PATH=${OS_DIR_PATH}/../tools/fs
RESOURCE_DIR_PATH=${FSTOOLS_DIR_PATH}/contents

SYSTEM_TYPE=`getconf LONG_BIT`
if [ "$SYSTEM_TYPE" = "64" ]; then
	OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux64
else
	OPENOCD_BIN_PATH=${OPENOCD_DIR_PATH}/linux32
fi

# ROMFS
prepare_resource()
{
	if [ -d "${RESOURCE_DIR_PATH}" ]; then
		echo "Packing resources into romfs.img ..."

		# create romfs.img
		sh ${FSTOOLS_DIR_PATH}/mkromfsimg.sh
	fi
}


# MAIN
main()
{
	echo "openocd is picked from ${OPENOCD_BIN_PATH}"
	echo "Binaries are picked from ${OUTPUT_BIN_PATH}"
	echo "Board path is ${BOARD_DIR_PATH}"

	# Process arguments
	for arg in $@
	do
		case ${arg} in
		ALL)
			echo "ALL :"

			# check existence of os binary
			if [ ! -f "${OUTPUT_BIN_PATH}/tinyara_head.bin" ]; then
				echo "TinyAra binary is not existed, build first"
				exit 1
			fi

			# check existence of firmware binaries
			if [ ! -f "${FW_DIR_PATH}/t20.nbl1.bin" ] ||\
				[ ! -f "${FW_DIR_PATH}/t20.bl2.head.bin" ] ||\
				[ ! -f "${FW_DIR_PATH}/t20.sss.fw.bin" ] ||\
				[ ! -f "${FW_DIR_PATH}/t20.wlan.bin" ]; then
				echo "Firmware binaries for sidk_s5jt200 are not existed"
				exit 1
			fi

			if [ "${CONFIG_FS_ROMFS}" == "y" ]; then
				prepare_resource
				if [ ! -f "${OUTPUT_BIN_PATH}/romfs.img" ]; then
					echo "ROMFS image is not present"
					exit 1
				fi
			fi

			# Generate Partition Map
			if [ -f "${OPENOCD_DIR_PATH}/partition_gen.sh" ]; then
				${OPENOCD_DIR_PATH}/partition_gen.sh
			fi

			# download all binaries using openocd script
			pushd ${OPENOCD_DIR_PATH}
			${OPENOCD_BIN_PATH}/openocd -f s5jt200_silicon_evt0_fusing_flash_all.cfg || exit 1
			popd
			;;

		*)
			echo "${arg} is not suppported in ${BOARD_NAME}"
			echo "Usage : make download [ ALL ]"
			exit 1
			;;
		esac
	done
	# Done
}

main $*

# END
