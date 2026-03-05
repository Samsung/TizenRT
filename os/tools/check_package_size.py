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

############################################################################
#
# This script checks if the each partitions are large enough to accommodate their respective binaries.
# It gets the partition size and binary size and check if the binary sizes are smaller than their respective partition sizes.
#
# Usage:
#   python check_package_size.py [bin_type_or_name]
#
# bin_type_or_name: kernel, app1, app2, common, resource, all, or binary name from config (default: all)
#
# Examples:
#   python check_package_size.py kernel         # Check only kernel binary size
#   python check_package_size.py app1           # Check only app1 binary size
#   python check_package_size.py myapp          # If myapp matches CONFIG_APP1_BIN_NAME
#   python check_package_size.py all            # Check all binary sizes (legacy behavior)
#   python check_package_size.py                # Same as 'all'
#
############################################################################

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

WARNING_RATIO = 95

def get_config_value(key) :
    return util.get_value_from_file(cfg_file, key).rstrip('\n')

def get_config_bin_name(config_key) :
    return util.get_value_from_file(cfg_file, config_key).replace('"', '').rstrip("\n")

def number_with_comma_align(number):
    return ("{:10,}".format(number))

def number_with_comma(number):
    return ("{:,}".format(number))

def get_partition_lists(flash_type) :
    if flash_type == INTERNAL_FLASH :
        name_list = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=")
        size_list = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=")
    else :
        name_list = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PART_NAME=")
        size_list = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PART_SIZE=")
    return name_list, size_list

def check_part_size(flash_type, bin_type):
    PARTITION_NAME_LIST, PARTITION_SIZE_LIST = get_partition_lists(flash_type)

    if PARTITION_SIZE_LIST == 'None' :
        return 0

    NAME_LIST = PARTITION_NAME_LIST.replace('"','').split(",")
    SIZE_LIST = PARTITION_SIZE_LIST.replace('"','').split(",")

    PART_IDX = 0

    for name in NAME_LIST :
        if (name.lower() == bin_type.lower()) or (bin_type == "KERNEL" and name.lower() == "os") :
            return int(SIZE_LIST[PART_IDX]) * 1024
        PART_IDX += 1
    return 0

def get_part_size(bin_type) :
    part_size = check_part_size(INTERNAL_FLASH, bin_type)
    if part_size == 0 and get_config_value("CONFIG_SECOND_FLASH_PARTITION=") == "y" :
        part_size = check_part_size(EXTERNAL_FLASH, bin_type)
    return part_size

def validate_binary_size(bin_type, part_size, append_mode=False):
    bin_name = util.get_binname_from_bininfo(bin_type)
    if bin_name == 'None' :
        return True

    output_path = build_folder + '/output/bin/' + bin_name
    if not os.path.isfile(output_path) :
        print(" {:10}".format(bin_type) + " Binary not found: " + bin_name)
        return True

    BINARY_SIZE = os.path.getsize(output_path)
    PARTITION_SIZE = part_size
    used_ratio = 0

    if PARTITION_SIZE != 0 :
        used_ratio = round(float(BINARY_SIZE) / float(PARTITION_SIZE) * 100, 2)

    fail_to_build = False
    if PARTITION_SIZE < int(BINARY_SIZE) :
        os.remove(output_path)
        fail_to_build = True
        check_result = "FAIL"
        result_mark = ""
    elif used_ratio > WARNING_RATIO :
        check_result = "WARNING"
        result_mark = ":warning:"
    else :
        check_result = "PASS"
        result_mark = ":heavy_check_mark:"

    print(" {:10}".format(bin_type) + " " + number_with_comma_align(BINARY_SIZE) + " bytes   " +
            number_with_comma_align(PARTITION_SIZE) + " bytes    " + "{:6}".format(used_ratio) + "%    " + check_result)

    file_mode = 'a' if append_mode else 'w'
    with open(output_file_name, file_mode) as outfile :
        outfile.write(bin_type + " | " + number_with_comma(BINARY_SIZE) + " bytes | " +
            number_with_comma(PARTITION_SIZE) + " bytes | " + str(used_ratio)+"%" + " | " + result_mark + check_result + "\n")

    return not fail_to_build

def print_header() :
    print("\n========== Size Verification of built Binaries ==========")
    print("Type        Binary Size     Partition Size      used(%)")

def init_output_file() :
    with open(output_file_name, 'w') as outfile :
        outfile.write("========== Size Verification of built Binaries ==========\n")
        outfile.write("Type | Binary Size | Partition Size | used(%) | result\n")
        outfile.write("-- | -- | -- | -- | --\n")

def check_kernel() :
    print_header()
    part_size = get_part_size("KERNEL")
    success = validate_binary_size("KERNEL", part_size, append_mode=True)
    if not success :
        print("!!!!!!!! ERROR !!!!!!!")
        print("=> KERNEL Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    print("=> Size verification SUCCESS!! The size of KERNEL binary is OK.\n")

def check_app1() :
    if get_config_value("CONFIG_APP1_INFO=") != "y" :
        return
    print_header()
    part_size = get_part_size("APP1")
    success = validate_binary_size("APP1", part_size, append_mode=True)
    if not success :
        print("!!!!!!!! ERROR !!!!!!!")
        print("=> APP1 Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    print("=> Size verification SUCCESS!! The size of APP1 binary is OK.\n")

def check_app2() :
    if get_config_value("CONFIG_APP2_INFO=") != "y" :
        return
    print_header()
    part_size = get_part_size("APP2")
    success = validate_binary_size("APP2", part_size, append_mode=True)
    if not success :
        print("!!!!!!!! ERROR !!!!!!!")
        print("=> APP2 Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    print("=> Size verification SUCCESS!! The size of APP2 binary is OK.\n")

def check_common() :
    if get_config_value("CONFIG_SUPPORT_COMMON_BINARY=") != "y" :
        return
    print_header()
    part_size = get_part_size("COMMON")
    success = validate_binary_size("COMMON", part_size, append_mode=True)
    if not success :
        print("!!!!!!!! ERROR !!!!!!!")
        print("=> COMMON Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    print("=> Size verification SUCCESS!! The size of COMMON binary is OK.\n")

def check_resource() :
    if get_config_value("CONFIG_RESOURCE_FS=") != "y" :
        return
    print_header()
    part_size = get_part_size("RESOURCE")
    success = validate_binary_size("RESOURCE", part_size, append_mode=True)
    if not success :
        print("!!!!!!!! ERROR !!!!!!!")
        print("=> RESOURCE Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    print("=> Size verification SUCCESS!! The size of RESOURCE binary is OK.\n")

def check_all() :
    init_output_file()
    print_header()

    fail_type_list = []

    part_size = get_part_size("KERNEL")
    if not validate_binary_size("KERNEL", part_size, append_mode=True) :
        fail_type_list.append("KERNEL")

    if get_config_value("CONFIG_APP_BINARY_SEPARATION=") == "y" :
        if get_config_value("CONFIG_APP1_INFO=") == "y" :
            part_size = get_part_size("APP1")
            if not validate_binary_size("APP1", part_size, append_mode=True) :
                fail_type_list.append("APP1")

        if get_config_value("CONFIG_APP2_INFO=") == "y" :
            part_size = get_part_size("APP2")
            if not validate_binary_size("APP2", part_size, append_mode=True) :
                fail_type_list.append("APP2")

        if get_config_value("CONFIG_SUPPORT_COMMON_BINARY=") == "y" :
            part_size = get_part_size("COMMON")
            if not validate_binary_size("COMMON", part_size, append_mode=True) :
                fail_type_list.append("COMMON")

    if get_config_value("CONFIG_RESOURCE_FS=") == "y" :
        part_size = get_part_size("RESOURCE")
        if not validate_binary_size("RESOURCE", part_size, append_mode=True) :
            fail_type_list.append("RESOURCE")

    if len(fail_type_list) > 0 :
        print("!!!!!!!! ERROR !!!!!!!")
        for fail_type in fail_type_list :
            print("=> " + fail_type + " Binary will be deleted. Need to re-configure the partition using menuconfig and to re-build.")
        sys.exit(1)
    else :
        print("=> Size verification SUCCESS!! The size of all binaries are OK.\n")

if __name__ == "__main__" :
    arg = sys.argv[1] if len(sys.argv) > 1 else "all"

    if arg == "kernel" :
        check_kernel()
    elif arg == "app1" or arg == get_config_bin_name("CONFIG_APP1_BIN_NAME=") :
        check_app1()
    elif arg == "app2" or arg == get_config_bin_name("CONFIG_APP2_BIN_NAME=") :
        check_app2()
    elif arg == "common" or arg == get_config_bin_name("CONFIG_COMMON_BINARY_NAME=") :
        check_common()
    elif arg == "resource" :
        check_resource()
    elif arg == "all" :
        check_all()
    else :
        print("Unknown argument: " + arg)
        print("Usage: python check_package_size.py [kernel|app1|app2|common|resource|all|<binary_name>]")
        sys.exit(1)
