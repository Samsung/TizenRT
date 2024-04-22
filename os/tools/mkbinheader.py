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
import shutil
import config_util as util

cfg_path = os.path.dirname(__file__) + '/../.config'

# User Binary Format
ELF = 1

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

def get_static_ram_size(bin_type):
    # Calculate static RAM size
    textsize = 0
    rosize = 0
    datasize = 0
    bsssize = 0
    if bin_type == ELF :
        # Static RAM size : Extract from readelf command in linux(ONLY for elf)
        os.system('readelf -S ' + file_path + '_dbg > ' + STATIC_RAM_ESTIMATION)
        ram_fp = open(STATIC_RAM_ESTIMATION, 'rb')
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
        if util.check_config_existence(cfg_path, 'CONFIG_OPTIMIZE_APP_RELOAD_TIME=y') == True:
            rosize = rosize + datasize;
            rosize = roundup_power_two(rosize)
            textsize = roundup_power_two(textsize)
        static_ram_size = textsize + rosize + datasize + bsssize
        ram_fp.close()
        os.remove(STATIC_RAM_ESTIMATION)
        return static_ram_size
    else : #Not supported.
        print("Error : Not supported Binary Format")
        sys.exit(1)

############################################################################
#
# Kernel binary header information :
#
# total header size is (12 + #)bytes.
# +-----------------------------------------------------------------------------------
# | Header size | Binary Version |  Binary Size | Secure Header Size | Secure Header |
# |   (2bytes)  |    (4bytes)    |   (4bytes)   |      (2bytes)      |    (#bytes)   |
# +-----------------------------------------------------------------------------------
# A size of Secure Header depends on board.
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is binary type.
# argv[3] is secure header size.
#
###########################################################################
def make_kernel_binary_header():
    secure_header_size = sys.argv[3]

    # Path to directory of this file
    mkbinheader_path = os.path.dirname(__file__)

    SIZE_OF_HEADERSIZE = 2
    SIZE_OF_BINVER = 4
    SIZE_OF_BINSIZE = 4
    SIZE_OF_SECURE_HEADER_SIZE = 2

    # Calculate binary header size
    header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINVER + SIZE_OF_BINSIZE + SIZE_OF_SECURE_HEADER_SIZE

    # Get binary version
    bin_ver = util.get_value_from_file(cfg_path, "CONFIG_BOARD_BUILD_DATE=").replace('"','').replace('\n','')
    if bin_ver == 'None' :
        print("Error : Not Found config for version, CONFIG_BOARD_BUILD_DATE")
        sys.exit(1)
    bin_ver = int(bin_ver)
    if bin_ver < 101 or bin_ver > 991231 :
        print("Error : Invalid Kernel Binary Version, ",bin_ver,".")
        print("        Please check CONFIG_BOARD_BUILD_DATE with 'YYMMDD' format in (101, 991231)")
        sys.exit(1)

    with open(file_path, 'rb') as fp:
        # binary data copy to 'data'
        data = fp.read()
        file_size = fp.tell()
        fp.close()

        fp = open(file_path, 'wb')

        # Generate binary with header data
        fp.write(struct.pack('H', header_size))
        fp.write(struct.pack('I', int(bin_ver)))
        fp.write(struct.pack('I', file_size))
        fp.write(struct.pack('H', int(secure_header_size)))
        fp.write(data)

        fp.close()

############################################################################
#
# User binary header information on APP_BINARY_SEPARTION
#
# total header size is 41bytes (CONFIG_ELF) or 44bytes (CONFIG_XIP_ELF).
# +-----------------------------------------------------------------
# | Header size | Binary type | Binary priority | Loading priority |
# |   (2bytes)  |   (1byte)   |     (1byte)     |      (1bytes)    |
# +-----------------------------------------------------------------
# -------------------------------------------------------------------------------------
# | Binary size |  Binary name | Binary version | Binary ram size | Binary stack size |
# |  (4bytes)   |   (16bytes)  |    (4bytes)    |    (4bytes)     |     (4bytes)      |
# -------------------------------------------------------------------------------------
# -----------------------------------------------------+
# | Kernel Version | Padding (Only for CONFIG_XIP_ELF) |
# |  (4bytes)      |          (3bytes)                 |
# -----------------------------------------------------+
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is binary type.
# argv[3] is binary format.
# argv[4] is binary name.
# argv[5] is binary version.
# argv[6] is a dynamic ram size required to run this binary.
# argv[7] is main task stack size.
# argv[8] is main task priority.
# argv[9] is a loading priority.
#
############################################################################

def make_user_binary_header():
    binary_format = sys.argv[3]
    binary_name = sys.argv[4]
    binary_ver = sys.argv[5]
    dynamic_ram_size = sys.argv[6]
    main_stack_size = sys.argv[7]
    main_priority = sys.argv[8]
    loading_priority = sys.argv[9]

    # Path to directory of this file
    mkbinheader_path = os.path.dirname(__file__)

    SIZE_OF_HEADERSIZE = 2
    SIZE_OF_BINTYPE = 1
    SIZE_OF_MAINPRIORITY = 1
    SIZE_OF_LOADINGPRIORITY = 1
    SIZE_OF_BINSIZE = 4
    SIZE_OF_BINNAME = 16
    SIZE_OF_BINVER = 4
    SIZE_OF_BINRAMSIZE = 4
    SIZE_OF_MAINSTACKSIZE = 4
    SIZE_OF_KERNELVER = 4

    header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINTYPE + SIZE_OF_MAINPRIORITY + SIZE_OF_LOADINGPRIORITY + SIZE_OF_BINSIZE + SIZE_OF_BINNAME + SIZE_OF_BINVER + SIZE_OF_BINRAMSIZE + SIZE_OF_MAINSTACKSIZE + SIZE_OF_KERNELVER

    xip_elf = util.get_value_from_file(cfg_path, "CONFIG_XIP_ELF=").replace('"','').replace('\n','')
    if xip_elf != 'None' :
        #add padding so the linker script start address aligns with 32bit/4byte
        SIZE_OF_PADDING = 3
        header_size = header_size + SIZE_OF_PADDING

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

    if int(main_stack_size) >= int(dynamic_ram_size) :
        print("Error : Dynamic ram size should be bigger than Main stack size.")
        print("Dynamic ram size : %d, Main stack size : %d" %(int(dynamic_ram_size), int(main_stack_size)))
        sys.exit(1)

    priority = int(util.get_value_from_file(cfg_path, "CONFIG_BM_PRIORITY_MAX=").replace('"','').replace('\n',''))
    if priority > 0 :
        BM_PRIORITY_MAX = priority
    priority = int(util.get_value_from_file(cfg_path, "CONFIG_BM_PRIORITY_MIN=").replace('"','').replace('\n',''))
    if priority > 0 :
        BM_PRIORITY_MIN = priority

    with open(file_path, 'rb') as fp:
        # binary data copy to 'data'
        data = fp.read()
        file_size = fp.tell()
        fp.close()

        if binary_format == 'elf' or binary_format == 'ELF' :
            bin_type = ELF
        else : # Not supported.
            bin_type = 0
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
        if util.check_config_existence(cfg_path, 'CONFIG_OPTIMIZE_APP_RELOAD_TIME=y') == True:
            binary_ram_size = int(dynamic_ram_size)
        else:
            binary_ram_size = int(static_ram_size) + int(dynamic_ram_size)
            binary_ram_size = roundup_power_two(binary_ram_size)

        # Get kernel binary version
        kernel_ver = util.get_value_from_file(cfg_path, "CONFIG_BOARD_BUILD_DATE=").replace('"','').replace('\n','')
        if kernel_ver == 'None' :
            print("Error : Not Found config for kernel version, CONFIG_BOARD_BUILD_DATE")
            sys.exit(1)
        kernel_ver = int(kernel_ver)
        if kernel_ver < 101 or kernel_ver > 991231 :
            print("Error : Invalid Kernel Binary Version, ",kernel_ver,".")
            print("        Please check CONFIG_BOARD_BUILD_DATE with 'YYMMDD' format in (101, 991231)")
            sys.exit(1)

        fp = open(file_path, 'wb')

        fp.write(struct.pack('H', header_size))
        fp.write(struct.pack('B', bin_type))
        fp.write(struct.pack('B', main_priority))
        fp.write(struct.pack('B', loading_priority))
        fp.write(struct.pack('I', file_size))
        fp.write('{:{}{}.{}}'.format(binary_name, '<', SIZE_OF_BINNAME, SIZE_OF_BINNAME - 1).replace(' ','\0'))
        fp.write(struct.pack('I', int(binary_ver)))
        fp.write(struct.pack('I', binary_ram_size))
        fp.write(struct.pack('I', int(main_stack_size)))
        fp.write(struct.pack('I', int(kernel_ver)))
        if xip_elf != 'None' :
            #add padding so the linker script start address aligns with 32bit/4byte
            fp.write(struct.pack('BBB', 0, 0, 0))
        fp.write(data)

        fp.close()


############################################################################
#
# Common binary header information :
#
# total header size is 10 bytes (CONFIG_ELF) or 12 bytes (CONFIG_XIP_ELF).
# +---------------------------------------------------------------------------------+
# | Header size | Binary Version |  Binary Size | Padding (Only for CONFIG_XIP_ELF) |
# |   (2bytes)  |    (4bytes)    |   (4bytes)   |           (2bytes)                |
# +---------------------------------------------------------------------------------+
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is binary type.
# argv[3] is binary version.
#
###########################################################################
def make_common_binary_header():

    SIZE_OF_HEADERSIZE = 2
    SIZE_OF_BINVER = 4
    SIZE_OF_BINSIZE = 4

    # Calculate binary header size
    header_size = SIZE_OF_HEADERSIZE + SIZE_OF_BINVER + SIZE_OF_BINSIZE

    # Get binary version
    bin_ver = util.get_value_from_file(cfg_path, "CONFIG_COMMON_BINARY_VERSION=").replace('"','').replace('\n','')
    if bin_ver == 'None' :
        print("Error : Not Found config for Common binary version, CONFIG_COMMON_BINARY_VERSION")
        sys.exit(1)
    bin_ver = int(bin_ver)
    if bin_ver < 101 or bin_ver > 991231 :
        print("Error : Invalid Common Binary Version, ",bin_ver,".")
        print("        Please check CONFIG_COMMON_BINARY_VERSION with 'YYMMDD' format in (101, 991231)")
        sys.exit(1)

    xip_elf = util.get_value_from_file(cfg_path, "CONFIG_XIP_ELF=").replace('"','').replace('\n','')
    if xip_elf != 'None' :
        #add padding so the linker script start address aligns with 32bit/4byte
        SIZE_OF_PADDING = 2
        header_size = header_size + SIZE_OF_PADDING

    with open(file_path, 'rb') as fp:
        # binary data copy to 'data'
        data = fp.read()
        file_size = fp.tell()
        fp.close()

        fp = open(file_path, 'wb')

        # Generate binary with header data
        fp.write(struct.pack('H', header_size))
        fp.write(struct.pack('I', int(bin_ver)))
        fp.write(struct.pack('I', file_size))
        xip_elf = util.get_value_from_file(cfg_path, "CONFIG_XIP_ELF=").replace('"','').replace('\n','')
        if xip_elf != 'None' :
            #add padding so the linker script start address aligns with 32bit/4byte
            fp.write(struct.pack('H', 0))
        fp.write(data)

############################################################################
#
# Generate headers for binary types
#
############################################################################
file_path  = sys.argv[1]
binary_type = sys.argv[2]

# copy orignal binary to (filename)_without_header.(ext)
file_path_without_header = os.path.splitext(file_path)[0] + "_without_header" + os.path.splitext(file_path)[1]
shutil.copyfile(file_path, file_path_without_header)

if binary_type == 'kernel' :
    make_kernel_binary_header()
elif binary_type == 'user' :
    make_user_binary_header()
elif binary_type == 'common' :
    make_common_binary_header()
else : # Not supported.
    print("Error : Not supported Binary Type")
    sys.exit(1)

