#!/usr/bin/env python
############################################################################
#
# Copyright 2019 Samsung Electronics All Rights Reserved.
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
import struct
############################################################################
#
# header information :
#
# / header size (2byte) / binary type (2byte) / binary size (4byte)
# / Binary name (16byte) / Binary version (16byte)
# / Kernel version (8byte) / Jump address (4byte)
# header size is 52byte.
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is file type.
# argv[3] is kernel version.
# argv[4] is binary name.
# argv[5] is binary version.
#
############################################################################

file_path  = sys.argv[1]
binary_type = sys.argv[2]
kernel_ver = sys.argv[3]
binary_name = sys.argv[4]
binary_ver = sys.argv[5]

# This path is only for dbuild.
tinyara_path = 'root/tizenrt/build/output/bin/tinyara'

START = 0
HEADER_SIZE_S = 2
BINARY_TYPE_S = 2
BINARY_SIZE_S = 4
BINARY_NAME_S = 16
BINARY_VERSION_S = 16
KERNEL_VERSION_S = 8
JUMP_ADDRESS_S = 4

header_size = HEADER_SIZE_S + BINARY_TYPE_S + BINARY_SIZE_S + BINARY_NAME_S + BINARY_VERSION_S + KERNEL_VERSION_S + JUMP_ADDRESS_S

ELF = 1
BIN = 2

with open(file_path, 'rb') as fp:
    # binary data copy to 'data'
    data = fp.read()
    file_size = fp.tell()
    fp.close()
    fp = open(file_path, 'wb')

    fp.write(struct.pack('h', header_size))

    if binary_type == 'bin' or binary_type == 'BIN' :
        bin_type = BIN
    elif binary_type == 'elf' or binary_type == 'ELF' :
        bin_type = ELF
    else : # Not supported.
        bin_type = 0

    fp.write(struct.pack('h', bin_type))
    fp.write(struct.pack('I', file_size))
    fp.write('{:{}{}.{}}'.format(binary_name, '<', BINARY_NAME_S, BINARY_NAME_S - 1).replace(' ','\0'))
    fp.write('{:{}{}.{}}'.format(binary_ver, '<', BINARY_VERSION_S, BINARY_VERSION_S - 1).replace(' ','\0'))
    fp.write('{:{}{}.{}}'.format(kernel_ver, '<', KERNEL_VERSION_S, KERNEL_VERSION_S - 1).replace(' ','\0'))

    # parsing _vector_start address from elf information.
    # _vector_start is only for ARM architecture. so it
    # operates in the ARM architecture.
    if bin_type == BIN :
        os.system('readelf -s ' + path + ' | grep _vector_start > addr_file')
        addr_fp = open("addr_file", 'rb')
        jump_addr = addr_fp.read()

        addr_fp.close()
        os.remove('addr_file')

        addr_data = jump_addr.split(' ')
        addr_s = '0x%s' %addr_data[3]
        addr = int(addr_s, 0)
    else :
        addr = 0

    fp.write(struct.pack('I', addr))
    fp.write(data)

    fp.close()
