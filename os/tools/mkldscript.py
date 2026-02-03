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
board_name = CONFIG_ARCH_BOARD[1:-1]
CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY = util.get_value_from_file(cfg_file, "CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY=").rstrip('\n')
# Get flash virtual remapped address instead of physical address
CONFIG_FLASH_VSTART_LOADABLE = util.get_value_from_file(cfg_file, "CONFIG_FLASH_VSTART_LOADABLE=").rstrip('\n')

# Define boards that use dual mode (dual OTA support)
DUAL_LD_BOARDS = ["rtl8730e"]
is_dual_ld_mode = board_name in DUAL_LD_BOARDS

# Dynamically get the offset from Kernel TRPK binary file
# Chip specific should implement the logic for offset calculation according to trpk file content
offset = 0
loadable_start_offset = 0  # Starting offset for loadable apps (common, app1, app2)
if CONFIG_TRPK_CONTAINS_MULTIPLE_BINARY == "y":
    if is_dual_ld_mode:
        sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../build/tools/amebasmart/gnu_utility')))
        from loadable_xip_elf import get_offset
        offset_shift = get_offset()
        loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16) - int(offset_shift, 16)
    else:
        # For other boards, use CONFIG_FLASH_VSTART_LOADABLE directly
        loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16)
        offset = loadable_start_offset
else:
    loadable_start_offset = int(CONFIG_FLASH_VSTART_LOADABLE, 16)

# For dual mode, initialize offset with loadable_start_offset
# For generic mode, offset starts from 0 and will be reset when encountering first loadable partition
if is_dual_ld_mode:
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

# Initialize data structures based on mode
if is_dual_ld_mode:
    # dual mode: use 2D array to store scripts for dual OTA partitions
    # ld_scripts[0] = common scripts for [slot0, slot1]
    # ld_scripts[1] = app1 scripts for [slot0, slot1]
    # ld_scripts[2] = app2 scripts for [slot0, slot1]
    ld_scripts = [[], [], []]  # three lists: [common, app1, app2] for two OTA partitions
    for i in range(2):  # Two OTA slots: 0 and 1
        start = "/* Auto-generated ld script */\nMEMORY\n"
        start += "{\n   uflash (rx)      : ORIGIN = "
        ld_scripts[0].append(start)  # common slot0 and slot1
        ld_scripts[1].append(start)  # app1 slot0 and slot1
        if int(CONFIG_NUM_APPS) >= 2:
            ld_scripts[2].append(start)  # app2 slot0 and slot1
    # Track which OTA slots have been generated for each binary type
    ld_generated_rtl = {"common": [False, False], "app1": [False, False], "app2": [False, False]}
    first_loadable_encountered = False
else:
    # Generic mode: track which ld files have been generated
    ld_generated = {"common": False, "app1": False, "app2": False}
    first_loadable_encountered = False
    ld_scripts = None
    ld_generated_rtl = None

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

# Initialize app2_ram_str to app1_ram_str as default
app2_ram_str = app1_ram_str
if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
    ram_offset = ram_offset - int(CONFIG_APP2_BIN_DYN_RAMSIZE)
    app2_ram_str = hex(ram_offset) + str1 + hex(int(CONFIG_APP2_BIN_DYN_RAMSIZE)) + "\n}\n"
    if ram_offset % 4096 != 0 :
        print("!!!!!!!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!!!!!!!!!!!")
        print("CONFIG_APP2_BIN_DYN_RAMSIZE should be aligned to 4KB")
        sys.exit(1)

# RAM string mapping for apps
app_ram_str = {"app1": app1_ram_str, "app2": app2_ram_str}

CONFIG_USER_SIGN_PREPEND_SIZE = util.get_value_from_file(cfg_file, "CONFIG_USER_SIGN_PREPEND_SIZE=").rstrip('\n')

if CONFIG_USER_SIGN_PREPEND_SIZE == 'None' :
    signing_offset = 0
else :
    signing_offset = int(CONFIG_USER_SIGN_PREPEND_SIZE)

# Helper function to reset offset for generic mode boards (e.g., bk7239n)
def reset_offset_if_needed():
    global offset, first_loadable_encountered
    if not is_dual_ld_mode and board_name == "bk7239n" and not first_loadable_encountered:
        offset = loadable_start_offset
        first_loadable_encountered = True

# Helper function to generate ld script content
def generate_ld_script(flash_start, flash_size, ram_str):
    ld_script_content = "/* Auto-generated ld script */\nMEMORY\n"
    ld_script_content += "{\n   uflash (rx)      : ORIGIN = "
    ld_script_content += flash_start + str1 + flash_size + str2 + ram_str
    return ld_script_content

for name in NAME_LIST :
    part_size = int(SIZE_LIST[PART_IDX]) * 1024

    # Reset offset for first loadable partition on generic mode boards (e.g., bk7239n)
    if not is_dual_ld_mode and name in ("app1", "app2", "common") and not first_loadable_encountered:
        reset_offset_if_needed()

    if name == "kernel" :
        ota_index = (ota_index + 1) % 2
    elif name == "app1" :
        if is_dual_ld_mode:
            # dual mode: generate dual OTA files
            # Use current ota_index to determine which slot this partition belongs to
            # Generate file for current slot if not already generated
            # Note: ota_index is switched when encountering "kernel", not here
            if not ld_generated_rtl["app1"][ota_index]:
                app1_start = hex(offset + 0x30 + signing_offset)
                app1_size = hex(part_size - 0x30 - signing_offset)
                ld_scripts[1][ota_index] = ld_scripts[1][ota_index] + app1_start + str1 + app1_size + str2 + app1_ram_str
                with open(output_folder + "app1_" + str(ota_index) + ".ld", "w") as ld :
                    ld.write(ld_scripts[1][ota_index])
                ld_generated_rtl["app1"][ota_index] = True
        else:
            # Generic mode: generate single OTA file (_0.ld only)
            ota_index = (ota_index + 1) % 2
            if not ld_generated["app1"]:
                app1_start = hex(offset + 0x30 + signing_offset)
                app1_size = hex(part_size - 0x30 - signing_offset)
                ld_file = "app1_0.ld"
                with open(output_folder + ld_file, "w") as ld :
                    print("Generating " + ld_file + " for position-independent code")
                    ld.write(generate_ld_script(app1_start, app1_size, app_ram_str["app1"]))
                ld_generated["app1"] = True
    elif name == "app2" :
        if is_dual_ld_mode:
            # dual mode: generate dual OTA files
            # Use current ota_index to determine which slot this partition belongs to
            # Generate file for current slot if not already generated
            if not ld_generated_rtl["app2"][ota_index]:
                app2_start = hex(offset + 0x30 + signing_offset)
                app2_size = hex(part_size - 0x30 - signing_offset)
                ld_scripts[2][ota_index] = ld_scripts[2][ota_index] + app2_start + str1 + app2_size + str2 + app2_ram_str
                with open(output_folder + "app2_" + str(ota_index) + ".ld", "w") as ld :
                    ld.write(ld_scripts[2][ota_index])
                ld_generated_rtl["app2"][ota_index] = True
            # Note: ota_index is switched when encountering "kernel", not here
        else:
            # Generic mode: generate single OTA file (_0.ld only)
            if not ld_generated["app2"]:
                app2_start = hex(offset + 0x30 + signing_offset)
                app2_size = hex(part_size - 0x30 - signing_offset)
                ld_file = "app2_0.ld"
                with open(output_folder + ld_file, "w") as ld :
                    print("Generating " + ld_file + " for position-independent code")
                    ld.write(generate_ld_script(app2_start, app2_size, app_ram_str["app2"]))
                ld_generated["app2"] = True
    elif name == "common" :
        if is_dual_ld_mode:
            # dual mode: generate dual OTA files
            # Use current ota_index to determine which slot this partition belongs to
            # Generate file for current slot if not already generated
            if not ld_generated_rtl["common"][ota_index]:
                common_start = hex(offset + 0x10 + signing_offset)
                common_size = hex(part_size - 0x10 - signing_offset)
                ld_scripts[0][ota_index] = ld_scripts[0][ota_index] + common_start + str1 + common_size + str2 + common_ram_str
                with open(output_folder + "common_" + str(ota_index) + ".ld", "w") as ld :
                    ld.write(ld_scripts[0][ota_index])
                ld_generated_rtl["common"][ota_index] = True
            # Note: ota_index is switched when encountering "kernel", not here
        else:
            # Generic mode: generate single OTA file (_0.ld only)
            if not ld_generated["common"] and CONFIG_SUPPORT_COMMON_BINARY == 'y':
                common_start = hex(offset + 0x10 + signing_offset)
                common_size = hex(part_size - 0x10 - signing_offset)
                with open(output_folder + "common_0.ld", "w") as ld :
                    ld.write(generate_ld_script(common_start, common_size, common_ram_str))
                ld_generated["common"] = True
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
