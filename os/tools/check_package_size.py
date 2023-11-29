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

# This script checks if the each partitions are large enough to accommodate their respective binaries.
# It gets the partition size and binary size and check if the binary sizes are smaller than their respective partition sizes.

import os
import sys
import string
import config_util as util

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
tool_folder = os_folder + '/tools'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'
output_file_name = output_folder + '/tinyara_binarysize.txt'

FAIL_TO_BUILD = False
WARNING_RATIO = 95

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

    # Compare the partition size and its binary size
    used_ratio = round(float(BINARY_SIZE) / float(PARTITION_SIZE) * 100, 2)

    global FAIL_TO_BUILD

    if bin_type == "BOOTPARAM" :
        if PARTITION_SIZE == BINARY_SIZE :
            check_result = "PASS"
            result_mark = ":heavy_check_mark:"
        else :
            fail_type_list.append(bin_type)
            os.remove(output_path)
            FAIL_TO_BUILD = True
            check_result = "FAIL"
            result_mark = ""
    else :
        if PARTITION_SIZE < int(BINARY_SIZE) :
            fail_type_list.append(bin_type)
            os.remove(output_path)
            FAIL_TO_BUILD = True
            check_result = "FAIL"
            result_mark = ""
        elif used_ratio > WARNING_RATIO :
            check_result = "WARNING"
            result_mark = ":warning:"
        else :
            check_result = "PASS"
            result_mark = ":heavy_check_mark:"

    # Print each information
    print(" {:10}".format(bin_type) + " " + number_with_comma_align(BINARY_SIZE) + " bytes   " +
            number_with_comma_align(PARTITION_SIZE) + " bytes    " + "{:6}".format(used_ratio) + "%    " + check_result)
    # File print each information
    outfile.write(bin_type + " | " + number_with_comma(BINARY_SIZE) + " bytes | " +
        number_with_comma(PARTITION_SIZE) + " bytes | " + str(used_ratio)+"%" + " | " + result_mark + check_result + "\n")

def check_binary_header(bin_type):
    # Read the binary name from .bininfo
    bin_name = util.get_binname_from_bininfo(bin_type)
    if bin_name == 'None' :
        return
    output_path = build_folder + '/output/bin/' + bin_name

    # Run script for checking binary header
    os.system('python ' + tool_folder + '/check_package.py ' + output_path)

PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=")
PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=")

CONFIG_APP_BINARY_SEPARATION = util.get_value_from_file(cfg_file, "CONFIG_APP_BINARY_SEPARATION=").rstrip('\n')
CONFIG_SUPPORT_COMMON_BINARY = util.get_value_from_file(cfg_file, "CONFIG_SUPPORT_COMMON_BINARY=").rstrip('\n')
CONFIG_USE_BP = util.get_value_from_file(cfg_file, "CONFIG_USE_BP=").rstrip('\n')

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
BP_IDX = -1

FLASH_SIZE = 0
for part in SIZE_LIST :
    if part.isdigit() == True :
        FLASH_SIZE += int(part) * 1024

KERNEL_PART_TMP_SIZE = FLASH_SIZE
COMMON_PART_TMP_SIZE = FLASH_SIZE
APP1_PART_TMP_SIZE = FLASH_SIZE
APP2_PART_TMP_SIZE = FLASH_SIZE
BP_PART_TMP_SIZE = FLASH_SIZE

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
    elif name == "bootparam" :
        if BP_PART_TMP_SIZE > (int(SIZE_LIST[PART_IDX]) * 1024) :
            BP_PART_TMP_SIZE = int(SIZE_LIST[PART_IDX]) * 1024
            BP_IDX = PART_IDX
        continue

KERNEL_PARTITION_SIZE = int(SIZE_LIST[KERNEL_IDX]) * 1024
COMMON_PARTITION_SIZE = int(SIZE_LIST[COMMON_IDX]) * 1024
APP1_PARTITION_SIZE = int(SIZE_LIST[APP1_IDX]) * 1024
APP2_PARTITION_SIZE = int(SIZE_LIST[APP2_IDX]) * 1024

SIZE_OF_BP_PARTITION = 8192

# Check a partition size and offset of boot parameters
if CONFIG_USE_BP == "y" :
    if BP_IDX == -1:
        print("FAIL!! No bootparam partition.")
        print("It should be located at the end of flash with 8K.")
        sys.exit(1)
    elif BP_IDX != PART_IDX - 1:
        print("FAIL!! bootparam partition is NOT located at the end of flash.")
        print("It should be located at the end of flash with 8K.")
        sys.exit(1)
    elif int(SIZE_LIST[BP_IDX]) * 1024 != SIZE_OF_BP_PARTITION:
        print("FAIL!! Bootparam partition size is NOT 8K.")
        print("It should be located at the end of flash with 8K.")
        sys.exit(1)
    BP_PARTITION_SIZE = int(SIZE_LIST[BP_IDX]) * 1024

# Check if the binary size is smaller than its partition size
print("\n========== Size Verification of built Binaries ==========")
print("Type        Binary Size     Partition Size      used(%)")

# File print init
outfile = open(output_file_name, 'w')
outfile.write("========== Size Verification of built Binaries ==========\n")
outfile.write("Type | Binary Size | Partition Size | used(%) | result\n")
outfile.write("-- | -- | -- | -- | --\n")

fail_type_list = []

# Check Kernel binary size
check_binary_size("KERNEL", KERNEL_PARTITION_SIZE)

# Check Common binary and App binaries size
if CONFIG_APP_BINARY_SEPARATION == "y" :
    if APP1_IDX != 0 :
        check_binary_size("APP1", APP1_PARTITION_SIZE)
    if APP2_IDX != 0 :
        check_binary_size("APP2", APP2_PARTITION_SIZE)
    if CONFIG_SUPPORT_COMMON_BINARY == "y" :
        check_binary_size("COMMON", COMMON_PARTITION_SIZE)

# Check Boot parameter size
if CONFIG_USE_BP == "y" :
    check_binary_size("BOOTPARAM", BP_PARTITION_SIZE)

outfile.close()
if FAIL_TO_BUILD == True :
    # Stop to build, because there is mismatched size problem.
    print("!!!!!!!! ERROR !!!!!!!")
    for fail_type in fail_type_list :
        print("=> " + fail_type + " Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
    sys.exit(1)
else :
    print("=> Size verification SUCCESS!! The size of all binaries are OK.\n")
