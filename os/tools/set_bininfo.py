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

import os
import sys
import string
import config_util as util

TARGET_EXT_NAME = "trpk" # TizenRT Package (TizenRT Header + output)

# This script save the binary name as "[bin_type]_[board]_[version].extension" in .bininfo file

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'

def save_bininfo(bin_name) :
    with open(os_folder + '/.bininfo', "a") as f :
        if ("app1" in bin_name) :
            f.write('APP1_BIN_NAME=' + bin_name + '\n')
        elif ("app2" in bin_name) :
            f.write('APP2_BIN_NAME=' + bin_name + '\n')
        elif ("common" in bin_name) :
            f.write('COMMON_BIN_NAME=' + bin_name + '\n')
        else :
            f.write('KERNEL_BIN_NAME=' + bin_name + '\n')

# Delete previous .bininfo
if os.path.isfile(os_folder + '/.bininfo') :
    os.remove(os_folder + '/.bininfo')

# Check the board type. Because kernel binary name is different based on board type.
BOARD_TYPE = util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

# Set the kernel bin name as "kernel_[board]_[version].extension"
if util.check_config_existence(cfg_file, 'CONFIG_BOARD_BUILD_DATE') == True:
    BIN_VERSION = util.get_value_from_file(cfg_file, "CONFIG_BOARD_BUILD_DATE=").replace('"', '').rstrip("\n")
    BIN_NAME = 'kernel_' + BOARD_TYPE + '_' + BIN_VERSION
else :
    BIN_NAME = 'kernel_' + BOARD_TYPE

# Check board_metadata.txt
metadata_file = build_folder + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
if os.path.isfile(metadata_file) :
    save_bininfo(BIN_NAME + '.' + TARGET_EXT_NAME)
else :
    # if this board has not board_metadata.txt, kernal binary name is setted 'tinyara.bin'
    save_bininfo("tinyara.bin")

# Set the user bin name as "app1_[board]_[version]"
if util.check_config_existence(cfg_file, 'CONFIG_APP1_INFO') == True :
    APP_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_VER=").rstrip("\n")
    USER_BIN_NAME = 'app1_' + BOARD_TYPE + '_' + APP_BIN_VER
    save_bininfo(USER_BIN_NAME + '.' + TARGET_EXT_NAME)

# Set the user bin name as "app2_[board]_[version]"
if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
    APP_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_VER=").rstrip("\n")
    USER_BIN_NAME = 'app2_' + BOARD_TYPE + '_' + APP_BIN_VER
    save_bininfo(USER_BIN_NAME + '.' + TARGET_EXT_NAME)

# Set the common bin name as "common_[board]_[version]"
if util.check_config_existence(cfg_file, 'CONFIG_SUPPORT_COMMON_BINARY') == True :
    COMMON_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_COMMON_BINARY_VERSION=").replace('"','').rstrip('\n')
    COMMON_BIN_NAME = 'common_' + BOARD_TYPE + '_' + COMMON_BIN_VER
    save_bininfo(COMMON_BIN_NAME + '.' + TARGET_EXT_NAME)
