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

GET_FILENAME() {
	if [ -z ${OUTBIN_PATH} ]; then
		echo "Error!! Need to set OUTBIN_PATH"
		exit 1
	fi

	if [[ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]]; then
		KERN_IMG=${OUTBIN_PATH}/tinyara.bin
		USER_IMG=${OUTBIN_PATH}/tinyara_user.bin
		APP1_IMG1=${OUTBIN_PATH}/micom
		APP1_IMG2=${OUTBIN_PATH}/micom
		APP2_IMG1=${OUTBIN_PATH}/wifi
		APP2_IMG2=${OUTBIN_PATH}/wifi
		FLASH_IMG=${OUTBIN_PATH}/tinyara_flash.bin
	elif [[ "${CONFIG_BUILD_PROTECTED}" == "y" ]]; then
		KERN_IMG=${OUTBIN_PATH}/tinyara.bin
		USER_IMG=${OUTBIN_PATH}/tinyara_user.bin
		FLASH_IMG=${OUTBIN_PATH}/tinyara_prot.bin
	elif [[ "${CONFIG_BUILD_FLAT}" == "y" ]]; then
		FLASH_IMG=${OUTBIN_PATH}/tinyara.hex
	else
		echo ".config is not there. Impossible to get file"
		exit 1
	fi
}
