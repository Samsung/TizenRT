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
import struct

config_file_path = os.path.dirname(__file__) + '/../.config'
bootparam_file_path = os.path.dirname(__file__) + '/../../build/output/bin/bootparam.bin'

SIZE_OF_BOOTPARAM_PART = 8192

###########################################################################################
#
# This script generates Boot Parameters for kernel.
#
# Boot Parameter information :
# +---------------------------------------------------------------------+
# | Active Idx |  First Address  | Second Address  |      Reserved      |
# |  (1 byte)  |    (4 bytes)    |    (4 bytes)    | (8Kbytes - 9bytes) |
# +---------------------------------------------------------------------+
#  * The Boot Parameters (8KB) should be downloaded and be located at the end of a flash.
#  * The "Active Idx" is a index indicating which address value to use.
#    - If the value is 0, "First address" will be loaded or is loaded.
#    - If the value is 1, "Second address" will be loaded or is loaded.
#
###########################################################################################

def get_config_value(file_name, config):
    with open(file_name, 'r+') as fp:
        lines = fp.readlines()
        found = False
        for line in lines:
            if config in line:
                value = (line.split("=")[1])
                value = value.replace('"','').replace('\n','')
                found = True
                break
    if found == False:
        print ("FAIL!! No found config %s" %config)
        sys.exit(1)
    return value;

def make_bootparam():
    print "========== Start to make boot parameters =========="
    SIZE_OF_KERNEL_INDEX = 1
    SIZE_OF_KERNEL_FIRST_ADDR = 4
    SIZE_OF_KERNEL_SECOND_ADDR = 4

    bootparam_size = SIZE_OF_KERNEL_INDEX + SIZE_OF_KERNEL_FIRST_ADDR + SIZE_OF_KERNEL_SECOND_ADDR

    names = get_config_value(config_file_path, "CONFIG_FLASH_PART_NAME=").split(",")
    sizes = get_config_value(config_file_path, "CONFIG_FLASH_PART_SIZE=").split(",")
    names = filter(None, names)
    sizes = filter(None, sizes)

    # Calculate partition size of boot parameters
    partition_size = 0
    offset = 0
    for index, name in enumerate(names):
        if name == "bootparam":
            partition_size = int(sizes[index]) * 1024
            break
        offset += int(sizes[index]) * 1024

    if partition_size == 0:
        print "FAIL!! No bootparam partition."
        sys.exit(1)
    elif partition_size != SIZE_OF_BOOTPARAM_PART:
        print "FAIL!! Bootparam partition size should be 8K. Please re-configure."
        sys.exit(1)
    elif offset != int(FLASH_SIZE) - int(SIZE_OF_BOOTPARAM_PART):
        print "FAIL!! Bootparam should be located at the end of flash with 8K. Please re-configure."
        sys.exit(1)

    # Get addresses of kernel partitions
    kernel_address = []
    offset = 0
    for index, types in enumerate(names):
        if types == "kernel":
            address = hex(int(FLASH_START_ADDR, 16) + offset)
            kernel_address.append(address)
        offset += int(sizes[index]) * 1024

    if len(kernel_address) == 0 or len(kernel_address) > 2:
        print "FAIL!! No found kernel partition"
        sys.exit(1)

    with open(bootparam_file_path, 'wb') as fp:
        # Write Data
        kernel_active_idx = 0
        fp.write(struct.pack('B', kernel_active_idx))
	for address in kernel_address:
            fp.write(struct.pack('I', int(address, 16)))
        # Fill remaining space with '0xff'
        remain_size = partition_size - bootparam_size;
        fp.write(b'\xff' * remain_size)

###################################################
# Generate boot parameters
###################################################
FLASH_START_ADDR = sys.argv[1]
FLASH_SIZE = sys.argv[2]
make_bootparam()
