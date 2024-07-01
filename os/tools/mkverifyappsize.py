#!/usr/bin/env python
############################################################################
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
############################################################################

from __future__ import print_function
import os
import sys
import math
import config_util as util

############################################################################
#
# This script verifies that L2 pages assigned per app are sufficient enough or not.
#
# parameter information :
#
# argv[1] is file path of binary file.
# argv[2] is app dynamic RAM size.
#
############################################################################

file_path  = sys.argv[1]
dynamic_ram_size = int(sys.argv[2])
cfg_path = os.path.dirname(__file__) + '/../.config'

# Temporary file to estimate application size
APP_SIZE_FILE = 'app_size_file'

l2_pg_per_app = int(util.get_value_from_file(cfg_path, "CONFIG_NUM_L2_PER_APP=").replace('"','').replace('\n',''))
l2_pg_need = 0
os.system('size ' + file_path + ' > ' + APP_SIZE_FILE)
app_data = open(APP_SIZE_FILE, 'rb')
line1 = app_data.readline().split()
line2 = app_data.readline().split()
size = dict(zip(line1,line2))
textsize = int(size["text"])
datasize = int(size["data"])
bsssize = int(size["bss"])

# When XIP is enabled, text section is stored in flash and mapped to RAM during execution. On the other hand, 
# the BSS and data sections are not stored in flash memory and are initialized to their respective values during 
# startup and can be accessed directly from RAM.

if util.check_config_existence(cfg_path, 'CONFIG_XIP_ELF') == True :
    l2_text = math.ceil(float(textsize) / (1024*1024))
    l2_other = math.ceil(float(datasize + bsssize + dynamic_ram_size) / (1024*1024))
    l2_pg_need = int(l2_text + l2_other)
else :
    l2_pg_need = int(math.ceil(float(textsize + datasize + bsssize + dynamic_ram_size) / (1024*1024)))
if l2_pg_per_app < l2_pg_need :
    print("Error : Please allocate more L2 page table per app. ", file_path, "binary requires more L2 page table.")
    print("L2 page table needed : ", l2_pg_need,end=", ")
    print("L2 page table allocated :", l2_pg_per_app)
    sys.exit(1)



