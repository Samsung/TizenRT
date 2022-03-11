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

import os
import sys

os_folder = os.path.dirname(__file__) + '/..'

# This function returns the value which starts with the "'target'=" from "file_name".
# For example, if CONFIG_ACB=123 is in test.txt,
# get_value_from_file("test.txt", "CONFIG_ABC=") returns 123.
def get_value_from_file(file_name, target):
    with open(file_name, 'r+') as f:
        lines = f.readlines()
        value = 'None'
        for line in lines:
            if target in line:
                value = (line.split("=")[1])
                break
    return value

# This function checks that 'config' is in 'file_name' or not.
# The return type of this function is True or False.
def check_config_existence(file_name, config):
    with open(file_name, 'r+') as f:
        lines = f.readlines()
    
    return any([True if config in line and not line.startswith('#') else False for line in lines ])

# This function returns the binary name from .bininfo file.
# For example, if kernel binary name is "KERNEL_BOARD_99991231",
# get_binname_from_bininfo("KERNEL") returns "KERNEL_BOARD_99991231".
def get_binname_from_bininfo(bin_type):
    bininfo_file = os.path.dirname(__file__) + '/..' + '/.bininfo'
    return get_value_from_file(bininfo_file, bin_type + "_BIN_NAME=").replace('"','').rstrip('\n')
