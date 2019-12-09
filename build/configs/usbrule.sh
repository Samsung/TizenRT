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

function CHECK_RULE()
{
	for filelist in `ls /etc/udev/rules.d`; do
		content=`cat /etc/udev/rules.d/${filelist}`
		flag=true
		for ((i=1; i<${#args[@]}; i++)); do
			if [[ "${content}" == *"${args[$i]}"* ]]; then
				continue
			else
				flag=false
				break
			fi
		done
		if [ $flag == true ]; then
			echo ".rules file already exist"
			exit 1
		fi
	done
}

function REGISTER_RULE()
{
	if [ $(id -u) -ne 0 ]; then
		echo "Please run as root."
		exit 1
	fi

	CHECK_RULE

	if [ ${args[0]} == "stm32" ]; then
		echo SUBSYSTEMS==\"usb\", ATTRS{idVendor}==\"${args[1]}\", ATTRS{idProduct}==\"${args[2]}\", MODE:=\"0666\",\
			SYMLINK+=\"stlinkv2_%n\" > /etc/udev/rules.d/99-${args[0]}.rules
		for ((i=3; i<${#args[@]}; i++)); do
			echo SUBSYSTEMS==\"usb\", ATTRS{idVendor}==\"${args[1]}\", ATTRS{idProduct}==\"${args[$i]}\", \
				MODE:=\"0666\", SYMLINK+=\"stlinkv3_%n\" >> /etc/udev/rules.d/99-${args[0]}.rules
		done
	else
		echo SUBSYSTEMS==\"usb\",ATTRS{idVendor}==\"${args[1]}\",ATTRS{idProduct}==\"${args[2]}\",MODE=\"0666\" \
			RUN+=\"/sbin/modprobe ftdi_sio\" RUN+=\"/bin/sh -c "'echo ${args[1]} ${args[2]} > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'"\" \
			> /etc/udev/rules.d/99-${args[0]}.rules
	fi

	`udevadm control --reload-rules && udevadm trigger`
	echo "USB rule creation succeeded."
	exit 0
}
REGISTER_RULE
