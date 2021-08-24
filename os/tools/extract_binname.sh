#!/bin/bash
###########################################################################
#
# Copyright 2021 Samsung Electronics All Rights Reserved.
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
BASE_DIR=$1
CONFIG_FILE="${BASE_DIR}/.config"
BUILD_DIR=${BASE_DIR}/../build
OUTPUT_DIR=${BUILD_DIR}/output/bin

source ${CONFIG_FILE}
BOARD_DIR=${BUILD_DIR}/configs/${CONFIG_ARCH_BOARD}
source ${BOARD_DIR}/board_metadata.txt

KERNEL_BIN_PATH=`find ${OUTPUT_DIR} -name ${KERNEL}*`
export KERNEL_BIN_NAME=`basename ${KERNEL_BIN_PATH}`
