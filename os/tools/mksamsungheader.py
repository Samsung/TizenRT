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

OS_DIR = os.path.dirname(__file__) + '/..'
TOOL_DIR = OS_DIR + '/tools/'

def read_binpath():
	with open(OS_DIR + '/.bininfo') as f:
		info = f.readlines()
	binary_path = info
	return binary_path[0].split('=')[1].rstrip('\n')
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
# argv[1] is file path of binary file.
# argv[2] is a type of binary, kernel or user.
# argv[3] is a size of secure header which is board-specific.
#
############################################################################
binary_path = OS_DIR + '/../build/output/bin/' + read_binpath()
binary_type = sys.argv[1]
secure_header_size = sys.argv[2]

mkbinheader_path = TOOL_DIR + 'mkbinheader.py'
mkchecksum_path = TOOL_DIR + 'mkchecksum.py'

############################################################################
# Generate Samsung headers
############################################################################
os.system('python %s %s %s %s' % (mkbinheader_path, binary_path, binary_type, secure_header_size))
os.system('python %s %s' % (mkchecksum_path, binary_path))
