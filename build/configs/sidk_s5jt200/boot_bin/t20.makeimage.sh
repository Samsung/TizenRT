#!/bin/bash
###########################################################################
#
# Copyright 2016 Samsung Electronics All Rights Reserved.
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
HTOOL=../../../../os/tools/AttachHeaderNS2

arg1="$1"
case $arg1 in
	cm0 )
	echo "attach cm0  version"
	#$(HTOOL) t20.ledctrlblk.m0.bin t20.ledctrlblk.m0_head.bin -embd
	#make size to 256KB, add version in end of 12byte
	printf "DATE%s" $(date '+%Y%m%d')  | dd of=t20.ledctrlblk.m0.bin bs=1 seek=262132 count=12
    ;;
	wifi )
	echo "attach wifi f/w version"
	#$(HTOOL) t20.wlan.bin t20.wlan_head.bin -embd
	#make size to 256KB, add version in end of 12byte
	#16/10/19 changes wlan.bin sized in 752KB-8KB, due to wifi nvram
	#761856-12
	printf "DATE%s" $(date '+%Y%m%d')  | dd of=t20.wlan.bin bs=1 seek=761844 count=12

	echo "attach wifi f/w header and version"
    ;;
	sss )
	echo "attach sss f/w version"
	#sss f/w can be upto 32KB
	#32768 - 12	
	printf "DATE%s" $(date '+%Y%m%d')  | dd of=t20.sss.fw.bin bs=1 seek=32756 count=12
	;;
    *)
    echo " unknown option"
    ;;
esac

