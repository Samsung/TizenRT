#!/usr/bin/env python
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

# This script is for checking if the kernel partition is large enough to accommodate kernel binary.
# For this, get the kernel partition size and kernel binary size,
# and check if the kernel binary size is smaller than kernel partition size.

import os
import sys
import string

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'

def get_value_from_file(file_name, target):
	with open(file_name, 'r+') as f:
		lines = f.readlines()
		value = 'None'
		for line in lines:
			if target in line:
				value = (line.split("=")[1])
				break		
	return value;

PARTITION_SIZE_LIST = get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=")
PARTITION_NAME_LIST = get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=")

if PARTITION_SIZE_LIST == 'None' :
	sys.exit(0)

NAME_LIST = PARTITION_NAME_LIST.replace('"','').split(",")
SIZE_LIST = PARTITION_SIZE_LIST.replace('"','').split(",")

# Find Kernel Partition Index
KERNEL_IDX=0
for name in NAME_LIST :
	if name == "kernel" or name == "os" :
		break
	else :
		KERNEL_IDX += 1

KERNEL_PARTITION_SIZE = int(SIZE_LIST[KERNEL_IDX]) * 1024

# Read the kernel binary name from .bininfo
bininfo_file = os_folder + '/.bininfo'
kernel_bin_name = get_value_from_file(bininfo_file, "KERNEL_BIN_NAME=").replace('"','').rstrip('\n')
output_path = build_folder + '/output/bin/' + kernel_bin_name

# Partition sizes in the list are in KB, so calculate all sizes in KB.
KERNEL_BINARY_SIZE=os.path.getsize(output_path)

# Compare the kernel partition size and kernel binary size
if KERNEL_PARTITION_SIZE < int(KERNEL_BINARY_SIZE) :
	print("!!!!!!!! ERROR !!!!!!!")
	print("Kernel Binary size(" + str(KERNEL_BINARY_SIZE) + " bytes) is greater than kernel partition size(" + str(KERNEL_PARTITION_SIZE) + " bytes).")
	print("Kernel Binary will be removed.")
	os.remove(output_path)
	sys.exit(1)
