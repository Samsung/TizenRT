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
# parameter information :
#
# argv[1] is kernel binary extension
#
############################################################################

SOURCE_EXT_NAME = sys.argv[1]
if SOURCE_EXT_NAME != '' :
    SOURCE_EXT_NAME = '.' + SOURCE_EXT_NAME

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'


############################################################################
# Convert binary to name stored in .bininfo
############################################################################

# Check the board type. Because kernel binary name is different based on board type.
BOARD_TYPE = util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

# Read the kernel binary name from board_metadata.txt.
metadata_file = build_folder + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
if os.path.isfile(metadata_file) :
    kernel_bin_name = util.get_value_from_file(metadata_file, "KERNEL=").replace('"','').rstrip('\n') + SOURCE_EXT_NAME
else :
    kernel_bin_name = "tinyara.bin"


# Convert kernel binary to name stored in .bininfo
kernel_trpk_name = util.get_binname_from_bininfo("KERNEL")
shutil.copyfile(output_folder + '/' + kernel_bin_name, output_folder + '/' + kernel_trpk_name)

# Convert app binaries to name stored in .bininfo
if util.check_config_existence(cfg_file, 'CONFIG_APP1_INFO') == True :
    app1_bin_name = util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_NAME=").replace('"', '').rstrip("\n")
    app1_trpk_name = util.get_binname_from_bininfo("APP1")
    shutil.copyfile(output_folder + '/' + app1_bin_name, output_folder + '/' + app1_trpk_name)
if util.check_config_existence(cfg_file, 'CONFIG_APP2_INFO') == True :
    app2_bin_name = util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_NAME=").replace('"', '').rstrip("\n")
    app2_trpk_name = util.get_binname_from_bininfo("APP2")
    shutil.copyfile(output_folder + '/' + app2_bin_name, output_folder + '/' + app2_trpk_name)

# Convert common binary to name stored in .bininfo
if util.check_config_existence(cfg_file, 'CONFIG_SUPPORT_COMMON_BINARY') == True :
    common_bin_name = util.get_value_from_file(cfg_file, "CONFIG_COMMON_BINARY_NAME=").replace('"', '').rstrip("\n")
    common_trpk_name = util.get_binname_from_bininfo("COMMON")
    shutil.copyfile(output_folder + '/' + app2_bin_name, output_folder + '/' + app2_trpk_name)
