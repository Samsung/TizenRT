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
# total header size is 61bytes.
# +--------------------------------------------------------------------------
# | Header size | Binary type | Compression  | Binary priority | Binary size|
# |   (2bytes)  |   (1byte)   |   (1byte)    |     (1byte)     |  (4bytes)  |
# +--------------------------------------------------------------------------
# ----------------------------------------------------------------------
# | Binary name | Binary version | Binary ram size | Binary stack size |
# |  (16bytes)  |   (16bytes)    |    (4bytes)     |     (4bytes)      |
# ----------------------------------------------------------------------
# ------------------------------ +
# | Kernel Version |Jump address |
# |  (8bytes)      |  (4bytes)   |
# -------------------------------+
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is file type.
# argv[3] is kernel version.
# argv[4] is binary name.
# argv[5] is binary version.
# argv[6] is a dynamic ram size required to run this binary.
# argv[7] is main task stack size.
# argv[8] is main task priority.
# argv[9] is compression type
# argv[10] is block size for compression
#
############################################################################

file_path  = sys.argv[1]
binary_type = sys.argv[2]
kernel_ver = sys.argv[3]
binary_name = sys.argv[4]
binary_ver = sys.argv[5]
dynamic_ram_size = sys.argv[6]
main_stack_size = sys.argv[7]
main_priority = sys.argv[8]
comp_enabled = sys.argv[9]
comp_blk_size = sys.argv[10]

# This path is only for dbuild.
elf_path_for_bin_type = 'root/tizenrt/build/output/bin/tinyara'

# Path to directory of this file
mkbinheader_path = os.path.dirname(__file__)

SIZE_OF_HEADERSIZE = 2
SIZE_OF_BINTYPE = 1
SIZE_OF_COMFLAG = 1
SIZE_OF_MAINPRIORITY = 1
SIZE_OF_BINSIZE = 4
SIZE_OF_BINNAME = 16
SIZE_OF_BINVER = 16
SIZE_OF_BINRAMSIZE = 4
SIZE_OF_MAINSTACKSIZE = 4
SIZE_OF_KERNELVER = 8
SIZE_OF_JUMPADDR = 4

header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINTYPE + SIZE_OF_COMFLAG + SIZE_OF_MAINPRIORITY + SIZE_OF_BINSIZE + SIZE_OF_BINNAME + SIZE_OF_BINVER + SIZE_OF_BINRAMSIZE + SIZE_OF_MAINSTACKSIZE + SIZE_OF_KERNELVER + SIZE_OF_JUMPADDR

ELF = 1
BIN = 2

COMP_NONE = 0
COMP_LZMA = 1
COMP_MAX = COMP_LZMA

# In size command on linux, 4th value is the summation of text, data and bss.
# We will use this value for elf.
SIZE_CMD_SUMMATION_INDEX = 3

# Temporary file to estimate the static RAM size.
STATIC_RAM_ESTIMATION = 'temp_static_ram_estimation_file'

if int(main_stack_size) >= int(dynamic_ram_size) :
    print "Error : Dynamic ram size should be bigger than Main stack size."
    print "Dynamic ram size : %d, Main stack size : %d" %(int(dynamic_ram_size), int(main_stack_size))
    sys.exit(1)

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
        print "Error : Not supported Binary Type"
        sys.exit(1)

    # Calculate RAM size
    # Dynamic RAM size : user input of argv[6]
    # Static RAM size : Extract from size command in linux(ONLY for elf)
    if bin_type == BIN :
        os.system('size ' + elf_path_for_bin_type + ' > ' + STATIC_RAM_ESTIMATION)
    elif bin_type == ELF :
        os.system('size ' + file_path + ' > ' + STATIC_RAM_ESTIMATION)
    else : #Not supported.
        print "Error : Not supported Binary Type"
        sys.exit(1)

    if 0 < int(main_priority) <= 255 :
        main_priority = int(main_priority)
    else :
        print "Error : This binary priority is not valid"
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

    # based on comp_enabled, check if we need to compress binary.
    # If yes, assign to bin_comp value for compression algorithm to use.
    # Else, assign 0 to bin_comp to represent no compression
    if 0 < int(comp_enabled) <= COMP_MAX :
        bin_comp = int(comp_enabled)
    else :
        bin_comp = 0

    # Compress data according to Compression Algorithm represented by bin_comp
    # Run mkcompressimg tool with provided options. Read output compressed file into data.
    if bin_comp > COMP_NONE :
        fp_tmp = open("tmp", 'wb+')
        fp_tmp.write(data)
        fp_tmp.close()
        os.system(mkbinheader_path + '/compression/mkcompressimg ' + comp_blk_size + ' ' + comp_enabled + ' tmp' + ' tmp_comp')
        fp_tmp = open("tmp_comp", 'rb')
        data = fp_tmp.read()
        file_size = fp_tmp.tell()
        fp_tmp.close()
        os.system('rm tmp tmp_comp')

    fp = open(file_path, 'wb')

    fp.write(struct.pack('H', header_size))
    fp.write(struct.pack('B', bin_type))
    fp.write(struct.pack('B', bin_comp))
    fp.write(struct.pack('B', main_priority))
    fp.write(struct.pack('I', file_size))
    fp.write('{:{}{}.{}}'.format(binary_name, '<', SIZE_OF_BINNAME, SIZE_OF_BINNAME - 1).replace(' ','\0'))
    fp.write('{:{}{}.{}}'.format(binary_ver, '<', SIZE_OF_BINVER, SIZE_OF_BINVER - 1).replace(' ','\0'))
    fp.write(struct.pack('I', binary_ram_size))
    fp.write(struct.pack('I', int(main_stack_size)))
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
