#!/usr/bin/env python
###########################################################################
#
# Copyright 2024 Samsung Electronics All Rights Reserved.
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

import os
import sys
import string
import config_util as util

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
tool_folder = os_folder + '/tools'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin/'

# Create output directory if it doesnt exist
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

CONFIG_ARCH_BOARD = util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").rstrip('\n')
CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY = util.get_value_from_file(cfg_file, "CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY=").rstrip('\n')
# Get flash virtual remapped address instead of physical address
CONFIG_FLASH_VSTART_LOADABLE = util.get_value_from_file(cfg_file, "CONFIG_FLASH_VSTART_LOADABLE=").rstrip('\n')

# Dynamically get the offset from Kernel TRPK binary file
# Chip specific should implement the logic for offset calculation according to trpk file content
offset = 0
loadable_start_offset = 0  # Starting offset for loadable apps (common, app1, app2)
if CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY == "y":
    if CONFIG_ARCH_BOARD[1:-1] == "rtl8730e":
        sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../build/tools/amebasmart/gnu_utility')))
        from loadable_xip_elf import get_offset
        offset_shift = get_offset()
        offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16) - int(offset_shift, 16)
        loadable_start_offset = offset
    elif CONFIG_ARCH_BOARD[1:-1] == "bk7239n":
        loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16)
        offset = loadable_start_offset
    else:
        # For other boards, use CONFIG_FLASH_VSTART_LOADABLE directly
        loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16)
        offset = loadable_start_offset
else:
    loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16)
    offset = loadable_start_offset

PART_IDX = 0

PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=").rstrip('\n').strip('"').rstrip(',')
PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=").rstrip('\n').strip('"').rstrip(',')

CONFIG_APP_BINARY_SEPARATION = util.get_value_from_file(cfg_file, "CONFIG_APP_BINARY_SEPARATION=").rstrip('\n')
CONFIG_SUPPORT_COMMON_BINARY = util.get_value_from_file(cfg_file, "CONFIG_SUPPORT_COMMON_BINARY=").rstrip('\n')
CONFIG_USE_BP = util.get_value_from_file(cfg_file, "CONFIG_USE_BP=").rstrip('\n')
CONFIG_NUM_APPS = util.get_value_from_file(cfg_file, "CONFIG_NUM_APPS=").rstrip('\n')

if PARTITION_SIZE_LIST == 'None' :
    sys.exit(0)

NAME_LIST = PARTITION_NAME_LIST.split(",")
SIZE_LIST = PARTITION_SIZE_LIST.split(",")

ld_scripts = [[], []] # two list to also include OTA partitions

for i in range(int(CONFIG_NUM_APPS) + 1) :
    start = "/* Auto-generated ld script */\nMEMORY\n"
    start += "{\n   uflash (rx)      : ORIGIN = "
    ld_scripts[0].append(start)
    ld_scripts[1].append(start)

ota_index = 1

CONFIG_RAM_SIZE = util.get_value_from_file(cfg_file, "CONFIG_RAM_SIZE=").rstrip('\n')
CONFIG_RAM_START = util.get_value_from_file(cfg_file, "CONFIG_RAM_START=").rstrip('\n')

str1 = " , LENGTH = "
str2 = "\n   usram (rwx)      : ORIGIN = "

ram_end = int(CONFIG_RAM_SIZE) + int(CONFIG_RAM_START, 16)

if ram_end % 4096 != 0 :
    print("RAM end should be 4KB aligned")
    sys.exit(1)

ram_offset = ram_end
ram_size = 0

CONFIG_APP1_BIN_DYN_RAMSIZE=util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_DYN_RAMSIZE=").rstrip('\n')
if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
    CONFIG_APP2_BIN_DYN_RAMSIZE=util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_DYN_RAMSIZE=").rstrip('\n')
CONFIG_COMMON_BIN_STATIC_RAMSIZE=util.get_value_from_file(cfg_file, "CONFIG_COMMON_BIN_STATIC_RAMSIZE=").rstrip('\n')

ram_offset = ram_offset - int(CONFIG_COMMON_BIN_STATIC_RAMSIZE)
if ram_offset % 4096 != 0 :
    print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
    print("CONFIG_COMMON_BIN_STATIC_RAMSIZE should be aligned to 4KB")
    sys.exit(1)

common_ram_size = (int(CONFIG_COMMON_BIN_STATIC_RAMSIZE) - 64 * 1024) #remove the pg table size at the end, will have to generalize it later
common_ram_str = hex(ram_offset) + str1 + hex(common_ram_size) + "\n}\n"

ram_offset = ram_offset - int(CONFIG_APP1_BIN_DYN_RAMSIZE)
if ram_offset % 4096 != 0 :
    print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
    print("CONFIG_APP1_BIN_DYN_RAMSIZE should be aligned to 4KB")
    sys.exit(1)

app1_ram_str = hex(ram_offset) + str1 + hex(int(CONFIG_APP1_BIN_DYN_RAMSIZE)) + "\n}\n"

if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
    ram_offset = ram_offset - int(CONFIG_APP2_BIN_DYN_RAMSIZE)
    app2_ram_str = hex(ram_offset) + str1 + hex(int(CONFIG_APP2_BIN_DYN_RAMSIZE)) + "\n}\n"
    if ram_offset % 4096 != 0 :
        print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("CONFIG_APP2_BIN_DYN_RAMSIZE should be aligned to 4KB")
        sys.exit(1)

CONFIG_USER_SIGN_PREPEND_SIZE = util.get_value_from_file(cfg_file, "CONFIG_USER_SIGN_PREPEND_SIZE=").rstrip('\n')

if CONFIG_USER_SIGN_PREPEND_SIZE == 'None' :
    signing_offset = 0
else :
    signing_offset = int(CONFIG_USER_SIGN_PREPEND_SIZE)

count = 0
countapp2 = 0
countapp1 = 0
countcommon = 0

# Track if we've encountered the first loadable partition (for bk7239n offset reset)
first_loadable_encountered = False

for name in NAME_LIST :
    count += 1
    part_size = int(SIZE_LIST[PART_IDX]) * 1024
    if name == "kernel" :
        ota_index = (ota_index + 1) % 2
    elif name == "app1" :
        ota_index = (ota_index + 1) % 2
        # For bk7239n, ensure offset is set correctly for first app1
        board_name = CONFIG_ARCH_BOARD[1:-1]
        if board_name == "bk7239n" and not first_loadable_encountered:
            offset = loadable_start_offset
            first_loadable_encountered = True
        app1_start = hex(offset + 0x30 + signing_offset)
        app1_size = hex(part_size - 0x30 - signing_offset)
        countapp1 += 1
        # Generate app1_0.ld for first app1 partition, app1_1.ld for second app1 partition
        # countapp1: 1 -> ota_idx 0, 2 -> ota_idx 1
        app1_ota_idx = (countapp1 - 1) % 2
        ld_script_content = "/* Auto-generated ld script */\nMEMORY\n"
        ld_script_content += "{\n   uflash (rx)      : ORIGIN = "
        ld_script_content += app1_start + str1 + app1_size + str2 + app1_ram_str
        with open(output_folder + "app1_" + str(app1_ota_idx) + ".ld", "w") as ld :
            print("OTA index in app1 ", app1_ota_idx)
            ld.write(ld_script_content)
    elif name == "app2" :
        ota_index = (ota_index + 1) % 2
        # For bk7239n, ensure offset is set correctly for first app2
        board_name = CONFIG_ARCH_BOARD[1:-1]
        if board_name == "bk7239n" and not first_loadable_encountered:
            offset = loadable_start_offset
            first_loadable_encountered = True
        app2_start = hex(offset + 0x30 + signing_offset)
        app2_size = hex(part_size - 0x30 - signing_offset)
        countapp2 += 1
        # Generate app2_0.ld for first app2 partition, app2_1.ld for second app2 partition
        # countapp2: 1 -> ota_idx 0, 2 -> ota_idx 1
        app2_ota_idx = (countapp2 - 1) % 2
        ld_script_content = "/* Auto-generated ld script */\nMEMORY\n"
        ld_script_content += "{\n   uflash (rx)      : ORIGIN = "
        ld_script_content += app2_start + str1 + app2_size + str2 + app2_ram_str
        with open(output_folder + "app2_" + str(app2_ota_idx) + ".ld", "w") as ld :
            print("OTA index in app2 ", app2_ota_idx)
            ld.write(ld_script_content)
    elif name == "common" :
        # Track which common partition instance we're processing
        # The first common should map to OTA index 0, second to OTA index 1
        common_ota_idx = countcommon % 2
        countcommon += 1
        if CONFIG_SUPPORT_COMMON_BINARY == 'y' :
            print("*** Making common (OTA index: " + str(common_ota_idx) + ")")
            # For bk7239n, reset offset to loadable_start_offset when encountering first loadable partition
            board_name = CONFIG_ARCH_BOARD[1:-1]
            if board_name == "bk7239n" and not first_loadable_encountered:
                offset = loadable_start_offset
                first_loadable_encountered = True
            common_start = hex(offset + 0x10 + signing_offset)
            common_size = hex(part_size - 0x10 - signing_offset)
            # Generate link script for this specific OTA index only
            ld_script_content = "/* Auto-generated ld script */\nMEMORY\n"
            ld_script_content += "{\n   uflash (rx)      : ORIGIN = "
            ld_script_content += common_start + str1 + common_size + str2 + common_ram_str
            with open(output_folder + "common_" + str(common_ota_idx) + ".ld", "w") as ld :
                ld.write(ld_script_content)
        else:
            # Even if common binary is not supported, still generate link scripts
            # to avoid linker errors when apps reference common_0.ld
            common_start = hex(offset + 0x10 + signing_offset)
            common_size = hex(part_size - 0x10 - signing_offset)
            # Generate minimal link script for this specific OTA index only
            ld_script_content = "/* Auto-generated ld script (common binary not supported) */\nMEMORY\n"
            ld_script_content += "{\n   uflash (rx)      : ORIGIN = "
            ld_script_content += common_start + str1 + common_size
            ld_script_content += "\n   usram (rwx)      : ORIGIN = 0x0, LENGTH = 0x0\n}\n"
            with open(output_folder + "common_" + str(common_ota_idx) + ".ld", "w") as ld :
                ld.write(ld_script_content)
    else:
        PART_IDX = PART_IDX + 1
        continue

    if name == 'app1' or name == 'app2' or name == 'common' :
        if offset % 4096 != 0 :
            print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
            print("flash start [" + hex(offset) + "] of " + name + " should be aligned to 4KB")
            sys.exit(1)
        if part_size % 4096 != 0 :
            print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
            print("flash partition size [" + str(part_size) + "] of " + name + " should be aligned to 4KB")
            sys.exit(1)

    offset = offset + part_size
    PART_IDX = PART_IDX + 1
