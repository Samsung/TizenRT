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
# total header size is 56bytes.
# +------------------------------------------------------------------------
# | Header size | Binary type | Binary size | Binary name | Binary version
# |   (2bytes)  |   (2bytes)  |   (4bytes)  |  (16bytes)  |    (16bytes)
# +------------------------------------------------------------------------
# ---------------------------------------------------+
#  | Binary ram size | Kernel version | Jump address |
#  |    (16bytes)    |    (8bytes)    |   (4bytes)   |
# ---------------------------------------------------+
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is file type.
# argv[3] is kernel version.
# argv[4] is binary name.
# argv[5] is binary version.
# argv[6] is a dynamic ram size required to run this binary.
#
############################################################################

file_path  = sys.argv[1]
binary_type = sys.argv[2]
kernel_ver = sys.argv[3]
binary_name = sys.argv[4]
binary_ver = sys.argv[5]
dynamic_ram_size = sys.argv[6]

# This path is only for dbuild.
elf_path_for_bin_type = 'root/tizenrt/build/output/bin/tinyara'

SIZE_OF_HEADERSIZE = 2
SIZE_OF_BINTYPE = 2
SIZE_OF_BINSIZE = 4
SIZE_OF_BINNAME = 16
SIZE_OF_BINVER = 16
SIZE_OF_BINRAMSIZE = 4
SIZE_OF_KERNELVER = 8
SIZE_OF_JUMPADDR = 4

header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINTYPE + SIZE_OF_BINSIZE + SIZE_OF_BINNAME + SIZE_OF_BINVER + SIZE_OF_BINRAMSIZE + SIZE_OF_KERNELVER + SIZE_OF_JUMPADDR

ELF = 1
BIN = 2

# In size command on linux, 4th value is the summation of text, data and bss.
# We will use this value for elf.
SIZE_CMD_SUMMATION_INDEX = 3

# Temporary file to estimate the static RAM size.
STATIC_RAM_ESTIMATION = 'temp_static_ram_estimation_file'

with open(file_path, 'rb') as fp:
    # binary data copy to 'data'
    data = fp.read()
    file_size = fp.tell()
    fp.close()

    if binary_type == 'bin' or binary_type == 'BIN' :
        bin_type = BIN
    elif binary_type == 'elf' or binary_type == 'ELF' :
        bin_type = ELF
    else : # Not supported.
        bin_type = 0
        print "Not supported Binary Type"
        sys.exit(1)

    # Calculate RAM size
    # Dynamic RAM size : user input of argv[6]
    # Static RAM size : Extract from size command in linux(ONLY for elf)
    if bin_type == BIN :
        os.system('size ' + elf_path_for_bin_type + ' > ' + STATIC_RAM_ESTIMATION)
    elif bin_type == ELF :
        os.system('size ' + file_path + ' > ' + STATIC_RAM_ESTIMATION)
    else : #Not supported.
        print "Not supported Binary Type"
        sys.exit(1)

    ram_fp = open(STATIC_RAM_ESTIMATION, 'rb')
    # First line is not used for calculating RAM size. So throw away it.
    ram_fp.readline()
    ram_array = ram_fp.readline()
    ram_fp.close()
    os.remove(STATIC_RAM_ESTIMATION)
    size_array = ram_array.split('\t')
    static_ram_size = size_array[SIZE_CMD_SUMMATION_INDEX]

    binary_ram_size = int(static_ram_size) + int(dynamic_ram_size)

    fp = open(file_path, 'wb')

    fp.write(struct.pack('h', header_size))

    fp.write(struct.pack('h', bin_type))
    fp.write(struct.pack('I', file_size))
    fp.write('{:{}{}.{}}'.format(binary_name, '<', SIZE_OF_BINNAME, SIZE_OF_BINNAME - 1).replace(' ','\0'))
    fp.write('{:{}{}.{}}'.format(binary_ver, '<', SIZE_OF_BINVER, SIZE_OF_BINVER - 1).replace(' ','\0'))
    fp.write(struct.pack('I', binary_ram_size))
    fp.write('{:{}{}.{}}'.format(kernel_ver, '<', SIZE_OF_KERNELVER, SIZE_OF_KERNELVER - 1).replace(' ','\0'))

    # parsing _vector_start address from elf information.
    # _vector_start is only for ARM architecture. so it
    # operates in the ARM architecture.
    if bin_type == BIN :
        os.system('readelf -s ' + elf_path_for_bin_type + ' | grep _vector_start > addr_file')
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
