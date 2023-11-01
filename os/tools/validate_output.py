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

# This script is for checking package validation : kernel, common, user.
# 1. Check if the each partitions are large enough to accommodate their respective binaries.
#    It gets the partition size and binary size and check if the binary sizes are smaller than their respective partition sizes.
# 2. Check if the values of binary header are valid.
#    It verifies the package by calculating the crc and comparing it with the value of the header.

import os
import sys

os_folder = os.path.dirname(__file__) + '/..'
tool_folder = os_folder + '/tools/'
output_folder = os_folder + '/../build/output/bin/'

def check_partition_configuration():
    # Run script for checking partition configuration
    ret = os.system('python ' + tool_folder + 'check_partition.py')
    if ret != 0 :
        sys.exit(1)

def check_package_size():
    # Run script for checking binary sizes
    ret = os.system('python ' + tool_folder + 'check_package_size.py')
    if ret != 0 :
        sys.exit(1)

def check_package_header():
    # Run script for checking binary sizes
    for file in os.listdir(output_folder):
        if file.endswith('.trpk'):
            os.system('python ' + tool_folder + 'check_package_header.py ' + output_folder + file)

############################################################################
# Validates packages
############################################################################
# 1. Validate partition
check_partition_configuration()

# 2. Validate the binary size with partition size
check_package_size()

# 3. Validate the header data of binaries
check_package_header()
