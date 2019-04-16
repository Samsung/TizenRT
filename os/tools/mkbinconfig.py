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
import struct
import sys
############################################################################
# Bin config information :
#
# The number of binaries (2 bytes) / binary name (16bytes) / flag (2 bytes)
#
# Example:
# 3'wifi   0''micom  0''hello  0'
#
# Parameter information :
#
# Two parameter are required to operate script.
# argv[1] is dirpath of bin config file.
# argv[2] is binary name (partition name).
#
############################################################################

# Path of 'bin_config' file
file_name = 'bin_config'
file_path = sys.argv[1] + '/' + file_name

# Binary/Partition Name
bin_name = sys.argv[2]

# Binary/Partition name Length
NAME_LEN = 16

# Partition flag to represent which partition is used. (default:0, which means first partition)
flag = 0

data_count = 0
with open(file_path, 'a+') as fp:
    # Get file size
    fp.seek(0, 2)
    file_size = fp.tell()

    # Read count if file exist
    if file_size != 0:
        fp.seek(0,0)
        data_count = fp.read(2)
	(data_count,) = struct.unpack('h', data_count)

    # Write the number of data at the beginning of the file
    fp.seek(0,0)
    fp.write(struct.pack('h', data_count + 1))

    # Write binary name and flag
    fp.seek(0,2)
    fp.write('{:{}{}.{}}'.format(bin_name, '<', int(NAME_LEN), int(NAME_LEN - 1)).replace(' ','\0'))
    fp.write(struct.pack('h', int(flag)))

    fp.close()
