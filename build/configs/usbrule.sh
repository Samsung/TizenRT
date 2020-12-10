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
#
# File   : artik05x_usbrule.sh
# Description : usb rule script for ARTIK 05X

args=("$@")
BOARD=${args[0]}
idVendor=${args[1]}
idProduct=${args[2]}
idVendor2=
idProduct2=
CMD=
NEW_RULE=

function CHECK_RULE()
{
	NEW_RULE=yes

	for filelist in `ls /etc/udev/rules.d`; do
		content=`cat /etc/udev/rules.d/${filelist}`

		if [[ "${content}" == *"${CMD}"* ]]; then
			echo "USB rules already exists in ${filelist}"
			NEW_RULE=no
			break
		fi
	done
}

function ADD_NEW_RULE()
{
	echo ${CMD} >> /etc/udev/rules.d/99-${BOARD}.rules
}

function REGISTER_RULE()
{
	if [ $(id -u) -ne 0 ]; then
		echo "Please run with root permission."
		exit 1
	fi

	if [ ${BOARD} == "stm32" ]; then
		CMD="SUBSYSTEMS==\"usb\", ATTRS{idVendor}==\"${idVendor}\", ATTRS{idProduct}==\"${idProduct}\", MODE:=\"0666\", SYMLINK+=\"stlinkv2_%n\""
		CHECK_RULE
		if [ "${NEW_RULE}" == "yes" ]; then
			ADD_NEW_RULE
		fi
		for ((i=3; i<${#args[@]}; i++)); do
			CMD="SUBSYSTEMS==\"usb\", ATTRS{idVendor}==\"${idVendor}\", ATTRS{idProduct}==\"${args[$i]}\", MODE:=\"0666\", SYMLINK+=\"stlinkv3_%n\""
			CHECK_RULE
			if [ "${NEW_RULE}" == "yes" ]; then
				ADD_NEW_RULE
			fi
		done
	else
		if [ "${args[3]}" == "" ]; then
			idVendor2=${idVendor}
			idProduct2=${idProduct}
		else
			idVendor2=${args[3]}
			idProduct2=${args[4]}
		fi

		CMD="SUBSYSTEMS==\"usb\",ATTRS{idVendor}==\"${idVendor}\",ATTRS{idProduct}==\"${idProduct}\",MODE=\"0666\" RUN+=\"/sbin/modprobe ftdi_sio\" RUN+=\"/bin/sh -c 'echo ${idVendor2} ${idProduct2} > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'\""
		CHECK_RULE
		if [ "${NEW_RULE}" == "yes" ]; then
			ADD_NEW_RULE
		fi
	fi

	if [ "${NEW_RULE}" == "yes" ]; then
		`udevadm control --reload-rules && udevadm trigger`
		echo "USB rule creation succeeded."
		exit 0
	else
		exit 1
	fi
}
REGISTER_RULE
