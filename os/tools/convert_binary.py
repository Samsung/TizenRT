#!/usr/bin/env python
###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
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
import shutil
import config_util as util

############################################################################
#
# This script converts binaries to different extension or different formats.
#
# Usage:
#   python convert_binary.py <bin_type_or_name> [source_ext] [objcopy] [objcopyargs] [kernel_start]
#
# bin_type_or_name: kernel, app1, app2, common, resource, all, or binary name from config
#
# For kernel conversion:
#   python convert_binary.py kernel "bin" "arm-none-eabi-objcopy" "" "0x08000000"
#
# For app/common/resource conversion:
#   python convert_binary.py app1
#   python convert_binary.py myapp   # If myapp matches CONFIG_APP1_BIN_NAME
#
# For all (legacy behavior):
#   python convert_binary.py all "bin" "arm-none-eabi-objcopy" "" "0x08000000"
#
############################################################################

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'

def get_board_type() :
    return util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

def get_config_bin_name(config_key) :
    return util.get_value_from_file(cfg_file, config_key).replace('"', '').rstrip("\n")

def get_kernel_bin_name(source_ext_name) :
    BOARD_TYPE = get_board_type()
    metadata_file = build_folder + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
    if os.path.isfile(metadata_file) :
        return util.get_value_from_file(metadata_file, "KERNEL=").replace('"','').rstrip('\n') + source_ext_name
    else :
        return "tinyara.bin"

def convert_kernel_binary(source_ext_name, objcopy, objcopyargs, kernel_start) :
    kernel_bin_name = get_kernel_bin_name(source_ext_name)
    kernel_trpk_name = util.get_binname_from_bininfo("KERNEL")
    if kernel_trpk_name and kernel_bin_name != kernel_trpk_name :
        src_path = output_folder + '/' + kernel_bin_name
        dst_path = output_folder + '/' + kernel_trpk_name
        if os.path.isfile(src_path) :
            shutil.copyfile(src_path, dst_path)

    source_bin_path = output_folder + '/' + kernel_bin_name

    if util.check_config_existence(cfg_file, 'CONFIG_INTELHEX_BINARY') == True:
        target_path = output_folder + '/' + os.path.splitext(kernel_bin_name)[0] + '.hex'
        print("CP: " + os.path.splitext(kernel_bin_name)[0] + '.hex')
        os.system('%s %s --change-addresses %s -I binary -O ihex %s %s' % (objcopy, objcopyargs, kernel_start, source_bin_path, target_path))

    if util.check_config_existence(cfg_file, 'CONFIG_MOTOROLA_SREC') == True:
        target_path = output_folder + '/' + os.path.splitext(kernel_bin_name)[0] + '.srec'
        print("CP: " + os.path.splitext(kernel_bin_name)[0] + '.srec')
        os.system('%s %s --change-addresses %s -I binary -O srec %s %s' % (objcopy, objcopyargs, kernel_start, source_bin_path, target_path))

def convert_app1_binary() :
    if util.check_config_existence(cfg_file, 'CONFIG_APP1_INFO') == True :
        app1_bin_name = util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_NAME=").replace('"', '').rstrip("\n")
        app1_trpk_name = util.get_binname_from_bininfo("APP1")
        if app1_trpk_name and app1_bin_name != app1_trpk_name :
            src_path = output_folder + '/' + app1_bin_name
            dst_path = output_folder + '/' + app1_trpk_name
            if os.path.isfile(src_path) :
                shutil.copyfile(src_path, dst_path)

def convert_app2_binary() :
    if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
        app2_bin_name = util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_NAME=").replace('"', '').rstrip("\n")
        app2_trpk_name = util.get_binname_from_bininfo("APP2")
        if app2_trpk_name and app2_bin_name != app2_trpk_name :
            src_path = output_folder + '/' + app2_bin_name
            dst_path = output_folder + '/' + app2_trpk_name
            if os.path.isfile(src_path) :
                shutil.copyfile(src_path, dst_path)

def convert_common_binary() :
    if util.check_config_existence(cfg_file, 'CONFIG_SUPPORT_COMMON_BINARY') == True :
        common_bin_name = util.get_value_from_file(cfg_file, "CONFIG_COMMON_BINARY_NAME=").replace('"', '').rstrip("\n")
        common_trpk_name = util.get_binname_from_bininfo("COMMON")
        if common_trpk_name and common_bin_name != common_trpk_name :
            src_path = output_folder + '/' + common_bin_name
            dst_path = output_folder + '/' + common_trpk_name
            if os.path.isfile(src_path) :
                shutil.copyfile(src_path, dst_path)

def convert_resource_binary() :
    if util.check_config_existence(cfg_file, 'CONFIG_RESOURCE_FS') == True :
        resource_bin_name = "resourcefs.img"
        resource_trpk_name = util.get_binname_from_bininfo("RESOURCE")
        if resource_trpk_name and resource_bin_name != resource_trpk_name :
            src_path = output_folder + '/' + resource_bin_name
            dst_path = output_folder + '/' + resource_trpk_name
            if os.path.isfile(src_path) :
                shutil.copyfile(src_path, dst_path)

def convert_all_binaries(source_ext_name, objcopy, objcopyargs, kernel_start) :
    convert_kernel_binary(source_ext_name, objcopy, objcopyargs, kernel_start)
    convert_app1_binary()
    convert_app2_binary()
    convert_common_binary()
    convert_resource_binary()

if __name__ == "__main__" :
    if len(sys.argv) < 2 :
        print("Usage: python convert_binary.py <bin_type_or_name> [source_ext] [objcopy] [objcopyargs] [kernel_start]")
        print("bin_type_or_name: kernel, app1, app2, common, resource, all, or binary name from config")
        sys.exit(1)

    arg = sys.argv[1]

    if arg == "kernel" or arg == "all" :
        source_ext_name = sys.argv[2] if len(sys.argv) > 2 else ""
        if source_ext_name != '' :
            source_ext_name = '.' + source_ext_name
        objcopy = sys.argv[3] if len(sys.argv) > 3 else ""
        objcopyargs = sys.argv[4] if len(sys.argv) > 4 else ""
        kernel_start = sys.argv[5] if len(sys.argv) > 5 else ""

        if arg == "kernel" :
            convert_kernel_binary(source_ext_name, objcopy, objcopyargs, kernel_start)
        else :
            convert_all_binaries(source_ext_name, objcopy, objcopyargs, kernel_start)
    elif arg == "app1" or arg == get_config_bin_name("CONFIG_APP1_BIN_NAME=") :
        convert_app1_binary()
    elif arg == "app2" or arg == get_config_bin_name("CONFIG_APP2_BIN_NAME=") :
        convert_app2_binary()
    elif arg == "common" or arg == get_config_bin_name("CONFIG_COMMON_BINARY_NAME=") :
        convert_common_binary()
    elif arg == "resource" :
        convert_resource_binary()
    else :
        print("Unknown argument: " + arg)
        print("Usage: python convert_binary.py <bin_type_or_name> [source_ext] [objcopy] [objcopyargs] [kernel_start]")
        sys.exit(1)
