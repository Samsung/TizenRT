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
import sys
import zlib
import struct
############################################################################
#
# This script prepends the binary file with the crc32 hash value.
#
# parameter information :
#
# argv[1] is file path of binary file.
#
############################################################################

file_path  = sys.argv[1]

with open(file_path, 'r') as fp:
    # binary data copy to 'data'.
    data = fp.read()
    fp.close()

    # hash to binary data by crc32.
    hash_data = zlib.crc32(data) & 0xFFFFFFFF

    fp = open(file_path, 'w')
    fp.write(struct.pack('I', hash_data))
    fp.write(data)
    
    fp.close()
