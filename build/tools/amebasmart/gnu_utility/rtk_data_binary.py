#!/usr/bin/env python
###########################################################################
#
# Copyright 2024 Samsung Electronics All Rights Reserved.
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
import string

os_folder = os.path.dirname(__file__) + '/../../../../os'
cfg_file = os_folder + '/.config'
dirpath = os.path.dirname(__file__)

def text_to_binary(input_file, output_file):
    with open(input_file, 'r') as infile, open(output_file, 'wb') as outfile:
        for line in infile:
            # Split the line by whitespace and remove '0x' prefix
            hex_values = line.strip().split()
            # Convert hex values to binary data
            binary_data = bytearray(int(value, 16) for value in hex_values)
            # Write binary data to the output file
            outfile.write(binary_data)

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../os/tools')))
import config_util as util
CONFIG_SECOND_FLASH_PARTITION = util.get_value_from_file(cfg_file, "CONFIG_SECOND_FLASH_PARTITION=").rstrip('\n')
if CONFIG_SECOND_FLASH_PARTITION == "y":
    input_file = dirpath + "/rtk_data_binary.txt"
    output_file = dirpath + "/rtk_ext_flash_data.bin"

    text_to_binary(input_file, output_file)
    # Ext flash valid
    print("1")
else:
    # Ext flash invalid
    print("0")
# Return value back to bash
sys.exit(0)
