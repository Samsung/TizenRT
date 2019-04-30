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

#echo $THIS_PATH

# When location of this script is changed, only OS_DIR_PATH should be changed together!!!
OS_DIR_PATH=$THIS_PATH/../../../os

IMXRT1050_USB_STRING=`mount | grep RT1050-EVK`
FIND_WORD="RT1050-EVK"

#echo $IMXRT1050_USB_STRING

for ELEMENT in $IMXRT1050_USB_STRING; do 
	#echo $ELEMENT
	if [[ "$ELEMENT" =~ "$FIND_WORD" ]]; then
		IMXRT1050_USB_PATH=$ELEMENT
	fi 
done;

#echo $IMXRT1050_USB_PATH
#echo $OS_DIR_PATH

CONFIG=${OS_DIR_PATH}/.config
if [ ! -f ${CONFIG} ]; then
        echo "No .config file"
        exit 1
fi

source ${CONFIG}

if [[ "${CONFIG_APP_BINARY_SEPARATION}" == "y" ]]; then


echo "Downloading tinyara_flash.bin for App binary separation"

# Prepare the binary paths

BIN_PATH=${OS_DIR_PATH}/../build/output/bin
KERN_IMG=${BIN_PATH}/tinyara.bin
USER_IMG=${BIN_PATH}/tinyara_user.bin
APP1_IMG1=${BIN_PATH}/micom
APP1_IMG2=${BIN_PATH}/micom
APP2_IMG1=${BIN_PATH}/wifi
APP2_IMG2=${BIN_PATH}/wifi
FLASH_IMG=${BIN_PATH}/tinyara_flash.bin

# Set the flash partition sizes. This value needs to be modified whenever
# there is a change in flash partition size in the project.

let OS_PART=256*1024
let USER_PART=128*1024
let APP_PART=128*1024

# Fetch the binary file sizes

KERN_SIZE=$(stat -c%s "$KERN_IMG")
USER_SIZE=$(stat -c%s "$USER_IMG")
APP1_SIZE1=$(stat -c%s "$APP1_IMG1")
APP1_SIZE2=$(stat -c%s "$APP1_IMG2")
APP2_SIZE1=$(stat -c%s "$APP2_IMG1")
APP2_SIZE2=$(stat -c%s "$APP2_IMG2")

# Calculate padding sizes. Padding is required to adjust the binary size with
# the flash partition size.

let kern_padding="$OS_PART - $KERN_SIZE"
let user_padding="$USER_PART - $USER_SIZE"
let app1_padding1="$APP_PART - $APP1_SIZE1"
let app1_padding2="$APP_PART - $APP1_SIZE2"
let app2_padding1="$APP_PART - $APP2_SIZE1"
let app2_padding2="$APP_PART - $APP2_SIZE2"

# Create a temporary flash img file by concatenation of all binaries and padding

(
head -c $KERN_SIZE $KERN_IMG ;
head -c $kern_padding /dev/zero ;
head -c $USER_SIZE $USER_IMG ;
head -c $user_padding /dev/zero ;
head -c $APP1_SIZE1 $APP1_IMG1 ;
head -c $app1_padding1 /dev/zero ;
head -c $APP1_SIZE2 $APP1_IMG2 ;
head -c $app1_padding2 /dev/zero ;
head -c $APP2_SIZE1 $APP2_IMG1 ;
head -c $app2_padding1 /dev/zero ;
head -c $APP2_SIZE2 $APP2_IMG2 ;
head -c $app2_padding2 /dev/zero ;
) > $FLASH_IMG

# Flash the flash img file onto device

cp $FLASH_IMG $IMXRT1050_USB_PATH


elif [[ "${CONFIG_BUILD_PROTECTED}" == "y" ]]; then


sed '$d' $OS_DIR_PATH/../build/output/bin/tinyara.hex > $OS_DIR_PATH/../build/output/bin/tinyara_temp.hex
cat $OS_DIR_PATH/../build/output/bin/tinyara_temp.hex $OS_DIR_PATH/../build/output/bin/tinyara_user.hex > $OS_DIR_PATH/../build/output/bin/tinyara_prot.hex
echo "Downloading tinyara_prot.hex for Protected build"
cp $OS_DIR_PATH/../build/output/bin/tinyara_prot.hex $IMXRT1050_USB_PATH


else


echo "Downloading tinyara.hex for Flat build"
cp $OS_DIR_PATH/../build/output/bin/tinyara.hex $IMXRT1050_USB_PATH


fi
