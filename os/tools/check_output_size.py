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

# This script is for checking if the kernel, common and user partitions are large enough to accommodate their respective binaries.
# For this, get the partition size and binary size,
# and check if the binary sizes are smaller than their respective partition sizes.

import os
import sys
import string
import config_util as util

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'

FAIL_TO_BUILD = False

def number_with_comma_align(number):
    return ("{:10,}".format(number))

def number_with_comma(number):
    return ("{:,}".format(number))

def check_binary_size(bin_type, part_size):
    # Read the binary name from .bininfo
    bin_name = util.get_binname_from_bininfo(bin_type)
    if bin_name == 'None' :
        return
    output_path = build_folder + '/output/bin/' + bin_name

    # Get the partition and binary size
    BINARY_SIZE=os.path.getsize(output_path)
    PARTITION_SIZE = part_size

    # Print each information
    print(bin_type + "\tBinary Size : " + number_with_comma_align(BINARY_SIZE) + " bytes" + "\tPartition Size : " + number_with_comma_align(PARTITION_SIZE) + " bytes")

    # Compare the partition size and its binary size
    if PARTITION_SIZE < int(BINARY_SIZE) :
        print("   !!!!!!!! ERROR !!!!!!!")
        print("   Built " + bin_type + " (" + number_with_comma(BINARY_SIZE) + " bytes) is greater than its partition (" + number_with_comma(PARTITION_SIZE) + " bytes).")
        print("   " + bin_type + " Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        os.remove(output_path)
        FAIL_TO_BUILD = True

PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=")
PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=")

CONFIG_APP_BINARY_SEPARATION = util.get_value_from_file(cfg_file, "CONFIG_APP_BINARY_SEPARATION=").rstrip('\n')
CONFIG_SUPPORT_COMMON_BINARY = util.get_value_from_file(cfg_file, "CONFIG_SUPPORT_COMMON_BINARY=").rstrip('\n')

if PARTITION_SIZE_LIST == 'None' :
    sys.exit(0)

NAME_LIST = PARTITION_NAME_LIST.replace('"','').split(",")
SIZE_LIST = PARTITION_SIZE_LIST.replace('"','').split(",")

# Find Partition Index
PART_IDX = -1
KERNEL_IDX = 0
COMMON_IDX = 0
APP1_IDX = 0
APP2_IDX = 0

FLASH_SIZE = 0
for part in SIZE_LIST :
    if part.isdigit() == True :
        FLASH_SIZE += int(part) * 1024

KERNEL_PART_TMP_SIZE = FLASH_SIZE
COMMON_PART_TMP_SIZE = FLASH_SIZE
APP1_PART_TMP_SIZE = FLASH_SIZE
APP2_PART_TMP_SIZE = FLASH_SIZE

for name in NAME_LIST :
    PART_IDX += 1
    if name == "kernel" or name == "os" :
        if KERNEL_PART_TMP_SIZE > (int(SIZE_LIST[PART_IDX]) * 1024) :
            KERNEL_PART_TMP_SIZE = (int(SIZE_LIST[PART_IDX]) * 1024)
            KERNEL_IDX = PART_IDX
        continue
    elif name == "app1" :
        if APP1_PART_TMP_SIZE > (int(SIZE_LIST[PART_IDX]) * 1024) :
            APP1_PART_TMP_SIZE = int(SIZE_LIST[PART_IDX]) * 1024
            APP1_IDX = PART_IDX
        continue
    elif name == "app2" :
        if APP2_PART_TMP_SIZE > (int(SIZE_LIST[PART_IDX]) * 1024) :
            APP2_PART_TMP_SIZE = int(SIZE_LIST[PART_IDX]) * 1024
            APP2_IDX = PART_IDX
        continue
    elif name == "common" :
        if COMMON_PART_TMP_SIZE > (int(SIZE_LIST[PART_IDX]) * 1024) :
            COMMON_PART_TMP_SIZE = int(SIZE_LIST[PART_IDX]) * 1024
            COMMON_IDX = PART_IDX
        continue

KERNEL_PARTITION_SIZE = int(SIZE_LIST[KERNEL_IDX]) * 1024
COMMON_PARTITION_SIZE = int(SIZE_LIST[COMMON_IDX]) * 1024
APP1_PARTITION_SIZE = int(SIZE_LIST[APP1_IDX]) * 1024
APP2_PARTITION_SIZE = int(SIZE_LIST[APP2_IDX]) * 1024

# Check if the binary size is smaller than its partition size
print("\n========== Size Verification of built Binaries ==========")
check_binary_size("KERNEL", KERNEL_PARTITION_SIZE)
if CONFIG_APP_BINARY_SEPARATION == "y" :
    if APP1_IDX != 0 :
        check_binary_size("APP1", APP1_PARTITION_SIZE)
    if APP2_IDX != 0 :
        check_binary_size("APP2", APP2_PARTITION_SIZE)
    if CONFIG_SUPPORT_COMMON_BINARY == "y" :
        check_binary_size("COMMON", COMMON_PARTITION_SIZE)

if FAIL_TO_BUILD == True :
    # Stop to build, because there is mismatched size problem.
    sys.exit(1)
else :
    print("Size verification is done.")
