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

############################################################################
#
# This script saves the binary name as "[bin_type]_[board]_[version].extension" in .bininfo file
#
# Usage:
#   python set_bininfo.py [bin_type_or_name]
#
# bin_type_or_name: kernel, app1, app2, common, resource, all, or binary name from config
#
# Examples:
#   python set_bininfo.py kernel           # Set only kernel binary info
#   python set_bininfo.py app1             # Set only app1 binary info
#   python set_bininfo.py myapp            # If myapp matches CONFIG_APP1_BIN_NAME, set app1 info
#   python set_bininfo.py all              # Set all binary info (legacy behavior)
#   python set_bininfo.py                  # Same as 'all'
#
############################################################################

TARGET_EXT_NAME = "trpk" # TizenRT Package (TizenRT Header + output)
BP_EXT_NAME = "bin" # Boot parameter Extension

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'

def save_bininfo(bin_type, bin_name) :
    with open(os_folder + '/.bininfo', "a") as f :
        f.write(bin_type + '_BIN_NAME=' + bin_name + '\n')

def get_board_type() :
    return util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

def get_config_bin_name(config_key) :
    return util.get_value_from_file(cfg_file, config_key).replace('"', '').rstrip("\n")

def set_kernel_bininfo() :
    BOARD_TYPE = get_board_type()
    if util.check_config_existence(cfg_file, 'CONFIG_BOARD_BUILD_DATE') == True:
        BIN_VERSION = util.get_value_from_file(cfg_file, "CONFIG_BOARD_BUILD_DATE=").replace('"', '').rstrip("\n").strip()
        BIN_NAME = 'kernel_' + BOARD_TYPE + '_' + BIN_VERSION
    else :
        BIN_NAME = 'kernel_' + BOARD_TYPE

    metadata_file = build_folder + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
    if os.path.isfile(metadata_file) :
        save_bininfo('KERNEL', BIN_NAME + '.' + TARGET_EXT_NAME)
    else :
        save_bininfo('KERNEL', "tinyara.bin")

def set_app1_bininfo() :
    if util.check_config_existence(cfg_file, 'CONFIG_APP1_INFO') == True :
        BOARD_TYPE = get_board_type()
        APP_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_VER=").replace('"','').rstrip("\n").strip()
        USER_BIN_NAME = 'app1_' + BOARD_TYPE + '_' + APP_BIN_VER
        save_bininfo('APP1', USER_BIN_NAME + '.' + TARGET_EXT_NAME)

def set_app2_bininfo() :
    if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
        BOARD_TYPE = get_board_type()
        APP_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_VER=").replace('"','').rstrip("\n").strip()
        USER_BIN_NAME = 'app2_' + BOARD_TYPE + '_' + APP_BIN_VER
        save_bininfo('APP2', USER_BIN_NAME + '.' + TARGET_EXT_NAME)

def set_common_bininfo() :
    if util.check_config_existence(cfg_file, 'CONFIG_SUPPORT_COMMON_BINARY') == True :
        BOARD_TYPE = get_board_type()
        COMMON_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_COMMON_BINARY_VERSION=").replace('"','').rstrip('\n').strip()
        COMMON_BIN_NAME = 'common_' + BOARD_TYPE + '_' + COMMON_BIN_VER
        save_bininfo('COMMON', COMMON_BIN_NAME + '.' + TARGET_EXT_NAME)

def set_resource_bininfo() :
    if util.check_config_existence(cfg_file, 'CONFIG_RESOURCE_FS') == True :
        BOARD_TYPE = get_board_type()
        RESOURCE_BIN_VER = util.get_value_from_file(cfg_file, "CONFIG_RESOURCE_BINARY_VERSION=").replace('"','').rstrip('\n').strip()
        RESOURCE_BIN_NAME = 'resource_' + BOARD_TYPE + '_' + RESOURCE_BIN_VER
        save_bininfo('RESOURCE', RESOURCE_BIN_NAME + '.' + TARGET_EXT_NAME)

def set_all_bininfo() :
    # Delete previous .bininfo only when setting all
    if os.path.isfile(os_folder + '/.bininfo') :
        os.remove(os_folder + '/.bininfo')

    set_kernel_bininfo()
    set_app1_bininfo()
    set_app2_bininfo()
    set_common_bininfo()
    set_resource_bininfo()

if __name__ == "__main__" :
    arg = sys.argv[1] if len(sys.argv) > 1 else "all"

    if arg == "kernel" :
        set_kernel_bininfo()
    elif arg == "app1" or arg == get_config_bin_name("CONFIG_APP1_BIN_NAME=") :
        set_app1_bininfo()
    elif arg == "app2" or arg == get_config_bin_name("CONFIG_APP2_BIN_NAME=") :
        set_app2_bininfo()
    elif arg == "common" or arg == get_config_bin_name("CONFIG_COMMON_BINARY_NAME=") :
        set_common_bininfo()
    elif arg == "resource" :
        set_resource_bininfo()
    elif arg == "all" :
        set_all_bininfo()
    else :
        print("Unknown argument: " + arg)
        print("Usage: python set_bininfo.py [kernel|app1|app2|common|resource|all|<binary_name>]")
        sys.exit(1)
