#!/usr/bin/python
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
###########################################################################

from __future__ import print_function
import sys
import re

config_file="../../../os/.config"
infile = open(config_file,'r')
for line in infile:
    line = line.strip('\n')
    if re.search("CONFIG_ESP_FLASH_BASE", line) != None:
        flash_base_addr = line.split('=', 2)[1]
    elif re.search("CONFIG_FLASH_PART_SIZE", line) != None:
        flash_size_list = line.split('=')[1].strip('"')
        flash_size_list = flash_size_list.split(",")
    elif re.search("CONFIG_FLASH_PART_TYPE", line) != None:
        flash_type_list = line.split('=',2)[1].strip('"')
        flash_type_list = flash_type_list.split(',')
    else:
        continue
index = int(sys.argv[1])
index_startadr = int(flash_base_addr,16)
size = 0

lowest_index = 0
if index < lowest_index:
    for i in range(len(flash_type_list)-1):
        print(i,":",flash_type_list[i])
else:
    for offset in range(index):
        size+= int(flash_size_list[offset]) * 1024
    index_startadr+=size
    index_startadr = hex(index_startadr)
    cursize=int(flash_size_list[index])*1024
    cursize = hex(cursize)
    print(index_startadr,cursize)

