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

EXT_NAME = sys.argv[1]

# This script changes the kernel binary name as "kernel_[board]_[version]".

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'
build_folder = os_folder + '/../build'
output_folder = build_folder + '/output/bin'

def get_value_from_file(file_name, target):
	with open(file_name, 'r+') as f:
		lines = f.readlines()
		value = 'None'
		for line in lines:
			if target in line:
				value = (line.split("=")[1])
				break		
	return value;

def check_version_config(file_name):
    with open(file_name, 'r+') as f:
        lines = f.readlines()
    
    return any([True if 'CONFIG_BOARD_BUILD_DATE' in line and not line.startswith('#') else False for line in lines ])

def save_bininfo(bin_name) :
	with open(os_folder + '/.bininfo', "w") as f :
		f.write('KERNEL_BIN_NAME=' + bin_name)

# Check the board type. Because kernel binary name is different based on board type.
BOARD_TYPE = get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

if check_version_config(cfg_file) :
	BIN_VERSION = get_value_from_file(cfg_file, "CONFIG_BOARD_BUILD_DATE=").replace('"', '').rstrip("\n")
	BIN_NAME = 'kernel_' + BOARD_TYPE + '_' + BIN_VERSION
else :
	BIN_NAME = 'kernel_' + BOARD_TYPE

# Read the kernel binary name from board_metadata.txt.
metadata_file = build_folder + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
if os.path.isfile(metadata_file) :
	kernel_bin_name = get_value_from_file(metadata_file, "KERNEL=").replace('"','').rstrip('\n')
	for filename in os.listdir(output_folder) :
		if (filename == kernel_bin_name + '.' + EXT_NAME) :
			# Change the kernel bin name as "kernel_[board]_[version].extension"
			os.rename(output_folder + '/' + filename, output_folder + '/' + BIN_NAME + '.' + EXT_NAME)
			save_bininfo(BIN_NAME + '.' + EXT_NAME)
			break
else :
	save_bininfo("tinyara.bin")

