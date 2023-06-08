#!/usr/bin/env python
############################################################################
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
############################################################################
import os
import sys
import config_util as util

OS_DIR = os.path.dirname(__file__) + '/..'
TOOL_DIR = OS_DIR + '/tools'
BUILD_DIR = OS_DIR + '/../build'
OUTPUT_DIR = BUILD_DIR + '/output/bin'

CFG_PATH = OS_DIR + '/.config'

# Check the board type. Because kernel binary name is different based on board type.
BOARD_TYPE = util.get_value_from_file(CFG_PATH, "CONFIG_ARCH_BOARD=").replace('"', '').rstrip("\n")

############################################################################
#
# This script generates samsung binary header for kernel.
#
# Kernel binary header information :
#
# total header size is (16 + #)bytes.
# +----------------------------------------------------------------------------------------------
# |  CRC32   | Header size | Binary Version |  Binary Size | Secure Header Size | Secure Header |
# | (4bytes) |   (2bytes)  |    (4bytes)    |   (4bytes)   |      (2bytes)      |    (#bytes)   |
# +----------------------------------------------------------------------------------------------
#  * A CRC32 is checksum value for the entire binary from Header size to the end of binary.
#  * A size of Secure Header depends on board.
#
# parameter information :
#
# argv[1] is a extension name of kernel binary
# argv[2] is a type of binary, kernel or user.
# argv[3] is a size of secure header which is board-specific.
#
############################################################################

source_ext_name = sys.argv[1]
if source_ext_name != '' :
	source_ext_name = '.' + source_ext_name

# Read the kernel binary name from board_metadata.txt.
metadata_file = BUILD_DIR + '/configs/' + BOARD_TYPE + '/board_metadata.txt'
if os.path.isfile(metadata_file) :
	kernel_bin_name = util.get_value_from_file(metadata_file, "KERNEL=").replace('"','').rstrip('\n') + source_ext_name
else :
	kernel_bin_name = "tinyara.bin"

binary_path = OUTPUT_DIR + '/' + kernel_bin_name
binary_type = sys.argv[2]
secure_header_size = sys.argv[3]

mkbinheader_path = TOOL_DIR + '/mkbinheader.py'
mkchecksum_path = TOOL_DIR + '/mkchecksum.py'

############################################################################
# Generate Samsung headers
############################################################################
os.system('python %s %s %s %s' % (mkbinheader_path, binary_path, binary_type, secure_header_size))
os.system('python %s %s' % (mkchecksum_path, binary_path))
