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

PART_IDX = 0

PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=").rstrip('\n').strip('"').rstrip(',')
PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=").rstrip('\n').strip('"').rstrip(',')

CONFIG_APP_BINARY_SEPARATION = util.get_value_from_file(cfg_file, "CONFIG_APP_BINARY_SEPARATION=").rstrip('\n')
CONFIG_SUPPORT_COMMON_BINARY = util.get_value_from_file(cfg_file, "CONFIG_SUPPORT_COMMON_BINARY=").rstrip('\n')
CONFIG_USE_BP = util.get_value_from_file(cfg_file, "CONFIG_USE_BP=").rstrip('\n')
CONFIG_NUM_APPS = util.get_value_from_file(cfg_file, "CONFIG_NUM_APPS=").rstrip('\n')
#CONFIG_FLASH_START_ADDR = util.get_value_from_file(cfg_file, "CONFIG_FLASH_START_ADDR=").rstrip('\n')
CONFIG_FLASH_START_ADDR = "0x0E000000"

if PARTITION_SIZE_LIST == 'None' :
    sys.exit(0)

NAME_LIST = PARTITION_NAME_LIST.split(",")
SIZE_LIST = PARTITION_SIZE_LIST.split(",")

ld_scripts = [[], []] # two list to also include OTA partitions

offset = int(CONFIG_FLASH_START_ADDR, 16)

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

ram_offset = ram_end
ram_size = 0

CONFIG_APP1_BIN_DYN_RAMSIZE=util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_DYN_RAMSIZE=").rstrip('\n')
CONFIG_APP2_BIN_DYN_RAMSIZE=util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_DYN_RAMSIZE=").rstrip('\n')
CONFIG_COMMON_BIN_DYN_RAMSIZE="1048576"

ram_offset = ram_offset - int(CONFIG_COMMON_BIN_DYN_RAMSIZE)
common_ram_size = (int(CONFIG_COMMON_BIN_DYN_RAMSIZE) - 64 * 1024) #remove the pg table size at the end, will have to generalize it later
common_ram_str = hex(ram_offset) + str1 + hex(common_ram_size) + "\n}\n"

ram_offset = ram_offset - int(CONFIG_APP1_BIN_DYN_RAMSIZE)
app1_ram_str = hex(ram_offset) + str1 + hex(int(CONFIG_APP1_BIN_DYN_RAMSIZE)) + "\n}\n"

ram_offset = ram_offset - int(CONFIG_APP2_BIN_DYN_RAMSIZE)
app2_ram_str = hex(ram_offset) + str1 + hex(int(CONFIG_APP2_BIN_DYN_RAMSIZE)) + "\n}\n"

# common at the end of the region

for name in NAME_LIST :
    part_size = int(SIZE_LIST[PART_IDX]) * 1024
    if name == "kernel" :
        ota_index = (ota_index + 1) % 2
    elif name == "app1" :
        app1_start = hex(offset + 0x30)
        app1_size = hex(part_size - 0x30)
        ld_scripts[ota_index][1] = ld_scripts[ota_index][1] + app1_start + str1 + app1_size + str2 + app1_ram_str
        with open(output_folder + "app1_" + str(ota_index) + ".ld", "w") as ld :
            ld.write(ld_scripts[ota_index][1])
    elif name == "app2" :
        app2_start = hex(offset + 0x30)
        app2_size = hex(part_size - 0x30)
        ld_scripts[ota_index][2] = ld_scripts[ota_index][2] + app2_start + str1 + app2_size + str2 + app2_ram_str
        with open(output_folder + "app2_" + str(ota_index) + ".ld", "w") as ld :
            ld.write(ld_scripts[ota_index][2])
    elif name == "common" :
        common_start = hex(offset + 0x10)
        common_size = hex(part_size - 0x10)
        ld_scripts[ota_index][0] = ld_scripts[ota_index][0] + common_start + str1 + common_size + str2 + common_ram_str
        with open(output_folder + "common_" + str(ota_index) + ".ld", "w") as ld :
            ld.write(ld_scripts[ota_index][0])
    else :
        PART_IDX = PART_IDX + 1
        continue

    offset = offset + part_size
    PART_IDX = PART_IDX + 1
