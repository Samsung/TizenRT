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
# imxrt1050-evk_download.sh


THIS_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

# When location of this script is changed, only TOP_PATH should be changed together!!!
TOP_PATH=${THIS_PATH}/../../..

OS_PATH=${TOP_PATH}/os
OUTBIN_PATH=${TOP_PATH}/build/output/bin

CONFIG=${OS_PATH}/.config
if [ ! -f ${CONFIG} ]; then
        echo "No .config file"
        exit 1
fi

source ${CONFIG}
source ${THIS_PATH}/imxrt1050-evk_get_filename.sh

IMXRT1050_USB_STRING=`mount | grep RT1050-EVK`
FIND_WORD="RT1050-EVK"

for ELEMENT in $IMXRT1050_USB_STRING; do 
	#echo $ELEMENT
	if [[ "$ELEMENT" =~ "$FIND_WORD" ]]; then
		IMXRT1050_USB_PATH=$ELEMENT
	fi 
done;

GET_FILENAME
echo "Downloading ${FLASH_IMG}"
cp ${FLASH_IMG} ${IMXRT1050_USB_PATH} || exit 1
