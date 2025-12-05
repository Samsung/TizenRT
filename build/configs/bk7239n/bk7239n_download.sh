#!/usr/bin/env bash
###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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
# bk7236_download.sh

TOOL_PATH=${TOP_PATH}/build/tools/armino
IMG_TOOL_PATH=${TOOL_PATH}/image_tool
SCRIPT_PATH="${TOOL_PATH}/download/tizen_entrance.py"
OUTPUT_PATH=${TOP_PATH}/build/output/bin
OS_PATH=${TOP_PATH}/os
CONFIG=${OS_PATH}/.config
source ${CONFIG}

APP_NUM=0

# burn config
BURNCONFIG=${TOP_PATH}/build/configs/bk7239n/scripts/.config
source ${BURNCONFIG}

if [ ! -d ${TOOL_PATH}/bk_py_libs ]; then 
     tar -zxvf ${TOOL_PATH}/bk_py_libs.tgz -C ${TOOL_PATH}
	 find "${TOOL_PATH}/bk_py_libs" -type d -exec chmod a+wx {} \;
fi 

# export python
export PYTHONPATH=${TOOL_PATH}/bk_py_libs:${PYTHONPATH}

# bl1:0  bl2:1
if [ "$CONFIG_TFM" = "y" ]; then
    WORK_SPACE=1
else
    WORK_SPACE=0
fi

function pre_download()
{
	port=$TTYDEV
	python3 -B ${SCRIPT_PATH} pre_download -p $port -b $WORKER_BAUD -d $DEBUG --link_type $WORK_SPACE
}

function board_download()
{
	# echo "### bk7239n board_download ###(0) $0 (1) $1 (2) $2 (3) $3 (4) $4 (5) $5 (6) $6 (7) $7"
	port=$TTYDEV
	start_addr=$2
	size=$(printf "0x%X" $(( $4 * 1024 )))
	bin=$3
	partition=$5
	# echo "${SCRIPT_PATH} download -p $port -i "${OUTPUT_PATH}/${bin}@tizen_separator@${start_addr}-${size}" --partition $partition"
	python3 -B ${SCRIPT_PATH} download -p $port -i "${OUTPUT_PATH}/${bin}@tizen_separator@${start_addr}-${size}" --partition $partition
}

function board_erase()
{
	# echo "### bk7239n board_erase ###(0) $0 (1) $1 (2) $2 (3) $3 (4) $4 (5) $5 (6) $6 (7) $7"
	port=$TTYDEV
	start_addr=$2
	partition=$4
	size=$(printf "0x%X" $(( $3 * 1024 )))
	python3 -B ${SCRIPT_PATH} erase -p $port -f "$start_addr-$size" --partition $partition
}

function post_download()
{
	port=$TTYDEV
	if [ $POST_REBOOT -eq 1 ]; then
		python3 -B ${SCRIPT_PATH} post_download -p $port -r
	else
		python3 -B ${SCRIPT_PATH} post_download -p $port
	fi
}
