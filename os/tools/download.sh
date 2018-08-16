#!/bin/bash
###########################################################################
#
# Copyright 2018 Samsung Electronics All Rights Reserved.
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

TOOLPATH=`test -d ${0%/*} && cd ${0%/*}; pwd`
OSPATH="${TOOLPATH}/.."

usages() {
	cat <<EOF
USAGE: `basename $0` [OPTIONS]
Program binaries into flash

OPTIONS:
	ALL	write all of binaries, os, bootloader, romfs and etc into flash
	OS      write os binary into flash
	ROMFS   write romfs image into flash

Each board has additional options.
For example, ARTIK05X has BL1, BL2, SSSFW, WLANFW, so on.

EOF
}

if [ $1 == "--help" ]; then
	usages 1>&2
	exit 0
fi

cd ${OSPATH}

if test $# -eq 0; then
	make download all
	exit 0
fi

make download $1 $2 $3 $4 $5 $6
exit 0
