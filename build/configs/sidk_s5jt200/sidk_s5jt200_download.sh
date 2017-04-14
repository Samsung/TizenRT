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
topdir=$PWD
tinyara_path=${topdir}/..
build_path=${tinyara_path}/build
output_path=${build_path}/output
bin_path=${output_path}/bin
openocd_path=${tinyara_path}/build/configs/${BOARD_NAME}/openocd

SYSTEM_TYPE=`getconf LONG_BIT`
if [ "$SYSTEM_TYPE" = "64" ]; then
	COMMAND=openocd_linux64
else
	COMMAND=openocd_linux32
fi

# Prepare resouces, pack into romfs.img
prepare_resource()
{
	echo "Prepare resouces and pack into romfs.img ..."
	# Only if FS_ROMFS enabled.
	if [ "${CONFIG_FS_ROMFS}" = "y" ]; then
		# create resource directory in ${output_path}
		mkdir -p ${output_path}/res
		cp -rf ${tinyara_path}/external/contents/${BOARD_NAME}/base-files/* ${output_path}/res/

		# create romfs.img
		echo "Creating ROMFS Image ..."
		sh ${tinyara_path}/apps/tools/mkromfsimg.sh

		romfs_size=`stat -c%s ${bin_path}/romfs.img`
		echo "ROMFS Image Size : ${romfs_size}"

		# download romfs.img using openocd script
		pushd ${openocd_path}
		./openocd_linux64 -f s5jt200_evt0_flash_romfs.cfg
		popd

	else
		echo "CONFIG_FS_ROMFS is not enabled, skip download ..."
	fi
}

# MAIN
main()
{
	echo "System is $SYSTEM_TYPE bits so that $COMMAND will be used to program"

	# Process arguments
	for arg in $@
	do
		case ${arg} in
		ALL)
			echo "ALL :"
			# download all binaries using openocd script
			pushd ${openocd_path}
			./$COMMAND -f s5jt200_silicon_evt0_fusing_flash_all.cfg
			popd
			prepare_resource
			;;

		RESOURCE)
			echo "RESOURCE :"
			prepare_resource
			;;

		FOTA_ALL)
			echo "FOTA_ALL :"
			if [ "${CONFIG_BOARD_FOTA_SUPPORT}" = "y" ]; then
				pushd ${openocd_path}
				./$COMMAND -f s5jt200_evt0_flash_all_fota.cfg
				popd
				prepare_resource
			else
				echo "FOTA is not supported, skip download ..."
			fi
			;;
		TINYARA_OTA0)
			echo "TINYARA_OTA0 :"
			if [ "${CONFIG_BOARD_FOTA_SUPPORT}" = "y" ]; then
				pushd ${openocd_path}
				./$COMMAND -f s5jt200_evt0_flash_tinyara_ota0.cfg
				popd
			else
				echo "FOTA is not supported, skip download ..."
			fi
			;;
		TINYARA_OTA1)
			echo "TINYARA_OTA1 :"
			if [ "${CONFIG_BOARD_FOTA_SUPPORT}" = "y" ]; then
				pushd ${openocd_path}
				./$COMMAND -f s5jt200_evt0_flash_tinyara_ota1.cfg
				popd
			else
				echo "FOTA is not supported, skip download ..."
			fi
			;;
		*)
			echo "${arg} is not suppported in ${BOARD_NAME}"
			echo "Usage : make download [ ALL | RESOURCE | FOTA_ALL | TINYARA_OTA0 | TINYARA_OTA1 ]"
			exit 1
			;;
		esac
	done
	# Done
}

main $*

# END
