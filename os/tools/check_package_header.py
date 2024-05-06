#!/usr/bin/env python
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
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
# This script parses the package header information and show them.
# And verify the package by calculating the crc and comparing it with the value of the header.
#
# Usage :
#   python check_package_header.py [package path]

from __future__ import with_statement
import struct
import os
import string
import sys
import zlib
import config_util as util

cfg_path = os.path.dirname(__file__) + '/../.config'

KERNEL_HEADER_SIZE = 12

xip_elf = util.get_value_from_file(cfg_path, "CONFIG_XIP_ELF=").replace('"','').replace('\n','')
if xip_elf == 'None' :
    COMMON_HEADER_SIZE = 10
    APP_HEADER_SIZE = 41
else :
    COMMON_HEADER_SIZE = 12
    APP_HEADER_SIZE = 44

SIGNING_SIZE = 32
CHECKSUM_SIZE = 4

LOADING_LOW = 1
LOADING_MID = 2
LOADING_HIGH = 3

PACKAGE_TYPE_ELF = 1

VERIFY_SUCCESS = True

target = sys.argv[1]
if target == "-h" or target == "--help" :
    print("Usage :")
    print("\tpython check_package_header.py [package_path]")
    print("\tex) python check_package_header.py ../../build/output/bin/kernel_XXX_YYMMDD.trpk")
    sys.exit(0)

# Read the package header
with open(target, 'r') as f:
    data = f.read()

check_signing = struct.unpack('H', data[4:6])

if "kernel" in target :
    print("=== Kernel Package Header Information ===")
    print("< Cannot check the signing >")
    SIGNING_OFFSET = 0 #For kernel package, signing info is not added at the first. So we will not use the offset for kernel package case.

    header_size = struct.unpack('H', data[4:6])
    package_ver = struct.unpack('I', data[6:10])
    file_size = struct.unpack('I', data[10:14])
    secure_size = struct.unpack('H', data[14:16])

    print("\tHeader Size        : " + str(header_size[0]))
    print("\tPackage Version    : " + str(package_ver[0]))
    print("\tFile Size          : " + str(file_size[0]))
    print("\tSecure Header Size : " + str(secure_size[0]))

elif "common" in target :
    print("=== Common Package Header Information ===")

    if int(check_signing[0]) == COMMON_HEADER_SIZE :
        print("< Unsigned Common Package >")
        SIGNING_OFFSET = 0
    else :
        print("< Signed Common Package >")
        SIGNING_OFFSET = SIGNING_SIZE

    header_size = struct.unpack('H', data[4 + SIGNING_OFFSET:6 + SIGNING_OFFSET])
    package_ver = struct.unpack('I', data[6 + SIGNING_OFFSET:10 + SIGNING_OFFSET])
    file_size = struct.unpack('I', data[10 + SIGNING_OFFSET:14 + SIGNING_OFFSET])

    print("\tHeader Size     : " + str(header_size[0]))
    print("\tPackage Version : " + str(package_ver[0]))
    print("\tFile Size       : " + str(file_size[0]))

elif "app" in target :
    print("=== App Package Header Information ===")

    if int(check_signing[0]) == APP_HEADER_SIZE :
        print("< Unsigned App Package >")
        SIGNING_OFFSET = 0
    else :
        print("< Signed App Package >")
        SIGNING_OFFSET = SIGNING_SIZE

    header_size = struct.unpack('H', data[4 + SIGNING_OFFSET:6 + SIGNING_OFFSET])
    package_type = struct.unpack('B', data[6 + SIGNING_OFFSET:7 + SIGNING_OFFSET])
    main_prio = struct.unpack('B', data[7 + SIGNING_OFFSET:8 + SIGNING_OFFSET])
    loading_prio = struct.unpack('B', data[8 + SIGNING_OFFSET:9 + SIGNING_OFFSET])
    file_size = struct.unpack('I', data[9 + SIGNING_OFFSET:13 + SIGNING_OFFSET])
    package_name = struct.unpack('ccc', data[13 + SIGNING_OFFSET:16 + SIGNING_OFFSET])
    package_ver = struct.unpack('I', data[29 + SIGNING_OFFSET:33 + SIGNING_OFFSET])
    ram_size = struct.unpack('I', data[33 + SIGNING_OFFSET:37 + SIGNING_OFFSET])
    stk_size = struct.unpack('I', data[37 + SIGNING_OFFSET:41 + SIGNING_OFFSET])
    kernel_ver = struct.unpack('I', data[41 + SIGNING_OFFSET:45 + SIGNING_OFFSET])

    print("\tHeader Size       : " + str(header_size[0]))
    if package_type[0] == PACKAGE_TYPE_ELF :
        print("\tPackage Type      : ELF(%d)" %package_type[0])
    else :
        print("\tPackage Type Invalid : %d" %package_type[0])
        VERIFY_SUCCESS = False
    print("\tMain Priority     : " + str(main_prio[0]))
    if loading_prio[0] == LOADING_LOW :
        print("\tLoading Priority  : LOADING_LOW(%d)" %LOADING_LOW)
    elif loading_prio[0] == LOADING_MID :
        print("\tLoading Priority  : LOADING_MID(%d)" %LOADING_MID)
    elif loading_prio[0] == LOADING_HIGH :
        print("\tLoading Priority  : LOADING_HIGH(%d)" %LOADING_HIGH)
    else :
        print("\tLoading Priority : Invalid(%d)" %loading_prio[0])
        VERIFY_SUCCESS = False
    print("\tFile Size         : " + str(file_size[0]))
    print("\tPackage Name      : " + str(package_name[0]) + str(package_name[1]) + str(package_name[2]))
    print("\tPackage Version   : " + str(package_ver[0]))
    print("\tRAM Size          : " + str(ram_size[0]))
    print("\tStack Size        : " + str(stk_size[0]))
    print("\tKernel Version    : " + str(kernel_ver[0]))

else :
    print("!!!Not Supported Package. Please Check the package!!!")
    VERIFY_SUCCESS = False
    sys.exit(1)

# Verify the package by calculating the crc and comparing it with the value of the header.

calc_start_idx = SIGNING_OFFSET + CHECKSUM_SIZE;
calc_end_idx = SIGNING_OFFSET + CHECKSUM_SIZE + file_size[0] + header_size[0]

## Calculate the checksum
checksum_calc = zlib.crc32(data[calc_start_idx:calc_end_idx]) & 0xFFFFFFFF

## Read the checksum from the header
checksum_header = struct.unpack('I', data[0 + SIGNING_OFFSET:4 + SIGNING_OFFSET])[0]

if checksum_header == checksum_calc :
    print("\t* Checksum verification Done.")
else :
    print("\t* Checksum is invalid, header : " + str(checksum_header) + ", calc : " + str(checksum_calc));
    VERIFY_SUCCESS = False


# Verify the package version between header and the file name
file_name_ver = os.path.basename(target).split("_")[2].split(".")[0]

if package_ver[0] == int(file_name_ver) :
    print("\t* Version Matched between filename and header.")
else :
    print("\t* Version NOT MATCHED!!! between filename(" + file_name_ver + ") and header(" + str(package_ver[0]) + ")")
    VERIFY_SUCCESS = False

# Return exit(1) if there is a validation failure.
if VERIFY_SUCCESS == False :
    sys.exit(1)
    print("!!!!ERROR!!!! Binary is invalid. Please check the package.\n")
else :
    print("=> Header verification SUCCESS!!\n")
