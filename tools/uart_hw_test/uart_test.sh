#!/bin/bash
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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
UART_TOOL_PATH=`test -d ${0%/*} && cd ${0%/*}; pwd`

BASE_DIR=$UART_TOOL_PATH/../..

#source $BASE_DIR/os/.config
. $BASE_DIR/os/.config

if [[ "${CONFIG_UART_HW_TEST}" == "y" ]]; then
	# Compile trap application
	gcc uart_test.c -o uart_test
	sudo ./uart_test
else
	echo "CONFIG_UART_HW_TEST is not enabled!!"
	echo "Rebuild TizenRT after enabling CONFIG_UART_HW_TEST and try again"
fi
