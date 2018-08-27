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
# File 	 : getfile.sh
# Description : getfile extract util
# Store path
FILE_NUM=$1
TOTAL_FILE=" "
TOTAL_NUM=$[$FILE_NUM + 1]
# Compile getfile application
gcc getfile_tool.c -o fget
i=1
while [ $i -le $TOTAL_NUM ]
do
	if [ $i -le $FILE_NUM ]
	then
		TOTAL_FILE="$TOTAL_FILE $2"
	else
		STORE_PATH=$2
		if [ "$STORE_PATH" = "" ]
		then
			STORE_PATH=$(pwd)/
		else
			STORE_PATH=$2/
		fi
	fi
	((i++))
	shift
done
sudo ./fget /dev/ttyUSB1 $TOTAL_FILE $STORE_PATH
sudo rm fget
