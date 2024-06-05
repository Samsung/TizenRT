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

INTERNAL_FLASH = 0
EXTERNAL_FLASH = 1

CONFIG_APP_BINARY_SEPARATION = util.get_value_from_file(cfg_file, "CONFIG_APP_BINARY_SEPARATION=").rstrip('\n')
CONFIG_SUPPORT_COMMON_BINARY = util.get_value_from_file(cfg_file, "CONFIG_SUPPORT_COMMON_BINARY=").rstrip('\n')

INTERNAL_PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=")
INTERNAL_PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=")

CONFIG_SECOND_FLASH_PARTITION = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PARTITION=").rstrip('\n')
SECOND_PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PART_NAME=")
SECOND_PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PART_SIZE=")

FAIL_TO_BUILD = False
WARNING_RATIO = 95

def number_with_comma_align(number):
    return ("{:10,}".format(number))

def number_with_comma(number):
    return ("{:,}".format(number))

def validate_binary_size(bin_type, part_size):
    outfile = open(output_file_name, 'w')

    # Read the binary name from .bininfo
    bin_name = util.get_binname_from_bininfo(bin_type)
    if bin_name == 'None' :
        return
    output_path = build_folder + '/output/bin/' + bin_name

    # Get the partition and binary size
    BINARY_SIZE=os.path.getsize(output_path)
    PARTITION_SIZE = part_size
    used_ratio = 0

    # Calculate the used ratio
    if PARTITION_SIZE != 0 :
        used_ratio = round(float(BINARY_SIZE) / float(PARTITION_SIZE) * 100, 2)

    # Compare the partition size and its binary size
    if PARTITION_SIZE < int(BINARY_SIZE) :
        fail_type_list.append(bin_type)
        os.remove(output_path)
        global FAIL_TO_BUILD
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
    outfile.close()

def check_part_size(flash_type, bin_type):
    if flash_type == INTERNAL_FLASH :
        PARTITION_NAME_LIST = INTERNAL_PARTITION_NAME_LIST
        PARTITION_SIZE_LIST = INTERNAL_PARTITION_SIZE_LIST
    else :
        PARTITION_NAME_LIST = SECOND_PARTITION_NAME_LIST
        PARTITION_SIZE_LIST = SECOND_PARTITION_SIZE_LIST

    if PARTITION_SIZE_LIST == 'None' :
        sys.exit(0)

    NAME_LIST = PARTITION_NAME_LIST.replace('"','').split(",")
    SIZE_LIST = PARTITION_SIZE_LIST.replace('"','').split(",")

    PART_IDX = 0

    for name in NAME_LIST :
        if (name.lower() == bin_type.lower()) or (bin_type == "KERNEL" and name.lower() == "os") :
            return int(SIZE_LIST[PART_IDX]) * 1024
        PART_IDX += 1
    return 0

def check_binary_size(bin_name):
    part_size = check_part_size(INTERNAL_FLASH, bin_name)
    if part_size == 0 and CONFIG_SECOND_FLASH_PARTITION == "y" :
        part_size = check_part_size(EXTERNAL_FLASH, bin_name)

    validate_binary_size(bin_name, part_size)

# Check if the binary size is smaller than its partition size
print("\n========== Size Verification of built Binaries ==========")
print("Type        Binary Size     Partition Size      used(%)")

# File print init
outfile = open(output_file_name, 'w')
outfile.write("========== Size Verification of built Binaries ==========\n")
outfile.write("Type | Binary Size | Partition Size | used(%) | result\n")
outfile.write("-- | -- | -- | -- | --\n")
outfile.close()

fail_type_list = []
check_binary_size("KERNEL")
if CONFIG_APP_BINARY_SEPARATION == "y" :
    check_binary_size("APP1")
    check_binary_size("APP2")
    if CONFIG_SUPPORT_COMMON_BINARY == "y" :
        check_binary_size("COMMON")

if FAIL_TO_BUILD == True :
    # Stop to build, because there is mismatched size problem.
    print("!!!!!!!! ERROR !!!!!!!")
    for fail_type in fail_type_list :
        print("=> " + fail_type + " Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
    sys.exit(1)
else :
    print("=> Size verification SUCCESS!! The size of all binaries are OK.\n")
