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

from __future__ import print_function
import os
import sys
import struct
import string

# Temporary file to estimate the static RAM size.
STATIC_RAM_ESTIMATION = 'temp_static_ram_estimation_file'

def roundup_power_two(size):
    size = size - 1
    size |= size >> 1
    size |= size >> 2
    size |= size >> 4
    size |= size >> 8
    size |= size >> 16
    size = size + 1

    return size

def get_config_value(file_name, config):
    with open(file_name, 'r+') as f:
        lines = f.readlines()
	found = False
	value = -1
	for line in lines:
		if config in line:
			value = int(line.split("=")[1])
			break		
	return value;

def check_optimize_config(file_name):
    with open(file_name, 'r+') as f:
        lines = f.readlines()
    
    return any([True if 'CONFIG_OPTIMIZE_APP_RELOAD_TIME=y' in line and not line.startswith('#') else False for line in lines ])

def get_static_ram_size(bin_type):
    textsize = 0
    rosize = 0
    datasize = 0
    bsssize = 0
    ram_fp = open(STATIC_RAM_ESTIMATION, 'rb')
    if bin_type == BIN :
        # First line is not used for calculating RAM size. So throw away it.
        ram_fp.readline()
        ram_array = ram_fp.readline()
        size_array = ram_array.split('\t')
        static_ram_size = size_array[SIZE_CMD_SUMMATION_INDEX]
    elif bin_type == ELF :
        line = ram_fp.readline()
        while line:
            words = line.split('.')
            if len(words) > 1:
                words = words[1].split()
                if len(words) > 1:
                    section = words[0]
                    size = int(words[4], 16)
                    if section == 'text':
                        textsize = size
                    elif section == 'rodata':
                        rosize = size
                    elif section == 'data':
                        datasize = size
                    elif section == 'bss':
                        bsssize = size
                        break
            line = ram_fp.readline()
        # If CONFIG_OPTIMIZE_APP_RELOAD_TIME is enabled, then we will make a copy
        # of the data section inside the ro section and it will be used in
        # reload time. So, we add datasize to rosize to make place for data section.
        cfg_path = os.getenv('TOPDIR') + '/.config'
        if check_optimize_config(cfg_path) == True:
            rosize = rosize + datasize;
            rosize = roundup_power_two(rosize)
            textsize = roundup_power_two(textsize)
        static_ram_size = textsize + rosize + datasize + bsssize
    else : #Not supported.
        print("Error : Not supported Binary Type")
        sys.exit(1)

    ram_fp.close()
    os.remove(STATIC_RAM_ESTIMATION)
    return static_ram_size


############################################################################
#
# header information :
#
# total header size is 46bytes.
# +--------------------------------------------------------------------------------
# | Header size | Binary type | Compression  | Binary priority | Loading priority |
# |   (2bytes)  |   (1byte)   |   (1byte)    |     (1byte)     |      (1bytes)    |
# +--------------------------------------------------------------------------------
# -------------------------------------------------------------------------------------
# | Binary size |  Binary name | Binary version | Binary ram size | Binary stack size |
# |  (4bytes)   |   (16bytes)  |    (4bytes)    |    (4bytes)     |     (4bytes)      |
# -------------------------------------------------------------------------------------
# ------------------------------ +
# | Kernel Version |Jump address |
# |  (4bytes)      |  (4bytes)   |
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
# argv[11] is a loading priority.
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
loading_priority = sys.argv[11]

# This path is only for dbuild.
elf_path_for_bin_type = 'root/tizenrt/build/output/bin/tinyara'

# Path to directory of this file
mkbinheader_path = os.path.dirname(__file__)

SIZE_OF_HEADERSIZE = 2
SIZE_OF_BINTYPE = 1
SIZE_OF_COMFLAG = 1
SIZE_OF_MAINPRIORITY = 1
SIZE_OF_LOADINGPRIORITY = 1
SIZE_OF_BINSIZE = 4
SIZE_OF_BINNAME = 16
SIZE_OF_BINVER = 4
SIZE_OF_BINRAMSIZE = 4
SIZE_OF_MAINSTACKSIZE = 4
SIZE_OF_KERNELVER = 4
SIZE_OF_JUMPADDR = 4

header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINTYPE + SIZE_OF_COMFLAG + SIZE_OF_MAINPRIORITY + SIZE_OF_LOADINGPRIORITY + SIZE_OF_BINSIZE + SIZE_OF_BINNAME + SIZE_OF_BINVER + SIZE_OF_BINRAMSIZE + SIZE_OF_MAINSTACKSIZE + SIZE_OF_KERNELVER + SIZE_OF_JUMPADDR

ELF = 1
BIN = 2

COMP_NONE = 0
COMP_LZMA = 1
COMP_MINIZ = 2
COMP_MAX = COMP_MINIZ

# Loading priority
LOADING_LOW = 1
LOADING_MID = 2
LOADING_HIGH = 3

# Scheduling priority MAX/MIN
SCHED_PRIORITY_MIN = 1
SCHED_PRIORITY_MAX = 255

# BM priority MAX/MIN
BM_PRIORITY_MAX = 205
BM_PRIORITY_MIN = 200

# In size command on linux, 4th value is the summation of text, data and bss.
# We will use this value for elf.
SIZE_CMD_SUMMATION_INDEX = 3

if int(main_stack_size) >= int(dynamic_ram_size) :
    print("Error : Dynamic ram size should be bigger than Main stack size.")
    print("Dynamic ram size : %d, Main stack size : %d" %(int(dynamic_ram_size), int(main_stack_size)))
    sys.exit(1)

config_path = os.getenv('TOPDIR') + '/.config'
priority = get_config_value(config_path, "CONFIG_BM_PRIORITY_MAX=")
if priority > 0 :
	BM_PRIORITY_MAX = priority
priority = get_config_value(config_path, "CONFIG_BM_PRIORITY_MIN=")
if priority > 0 :
	BM_PRIORITY_MIN = priority

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
        print("Error : Not supported Binary Type")
        sys.exit(1)

    # Calculate RAM size
    # Dynamic RAM size : user input of argv[6]
    # Static RAM size : Extract from size command in linux(ONLY for elf)
    if bin_type == BIN :
        os.system('size ' + elf_path_for_bin_type + ' > ' + STATIC_RAM_ESTIMATION)
    elif bin_type == ELF :
        os.system('readelf -S ' + file_path + ' > ' + STATIC_RAM_ESTIMATION)
    else : #Not supported.
        print("Error : Not supported Binary Type")
        sys.exit(1)

    main_priority = int(main_priority)
    if (main_priority < SCHED_PRIORITY_MIN) or (main_priority > SCHED_PRIORITY_MAX) or (BM_PRIORITY_MIN <= main_priority and main_priority <= BM_PRIORITY_MAX) :
        print("Error : This binary priority ", main_priority, " is not valid")
        sys.exit(1)

    # Loading priority
    if loading_priority == 'H' or loading_priority == 'HIGH' :
	loading_priority = LOADING_HIGH
    elif loading_priority == 'M' or loading_priority == 'MID' :
        loading_priority = LOADING_MID
    elif loading_priority == 'L' or loading_priority == 'LOW' :
        loading_priority = LOADING_LOW
    else : #Not supported.
        print("Error : Not supported Binary loading priority")
        sys.exit(1)

    static_ram_size = get_static_ram_size(bin_type)
    cfg_path = os.getenv('TOPDIR') + '/.config'
    if check_optimize_config(cfg_path) == True:
        binary_ram_size = int(dynamic_ram_size)
    else:
        binary_ram_size = int(static_ram_size) + int(dynamic_ram_size)
        binary_ram_size = roundup_power_two(binary_ram_size)

    if float(kernel_ver) <= 0.0 :
        kernel_ver = 3.0
    else :
        kernel_ver = float(kernel_ver)

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
    fp.write(struct.pack('B', loading_priority))
    fp.write(struct.pack('I', file_size))
    fp.write('{:{}{}.{}}'.format(binary_name, '<', SIZE_OF_BINNAME, SIZE_OF_BINNAME - 1).replace(' ','\0'))
    fp.write(struct.pack('I', int(binary_ver)))
    fp.write(struct.pack('I', binary_ram_size))
    fp.write(struct.pack('I', int(main_stack_size)))
    fp.write(struct.pack('f', kernel_ver))

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
