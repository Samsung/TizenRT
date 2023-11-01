#!/usr/bin/env python
###########################################################################
#
# Copyright 2023 Samsung Electronics All Rights Reserved.
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

# This script checks if the partitions are set up well.
# It checks if configured partition sizes are not greater than whole flash size
# and if the number of entries of partition configuration lists(size, type, name) are same.

import os
import sys
import string
import config_util as util

os_folder = os.path.dirname(__file__) + '/..'
cfg_file = os_folder + '/.config'

PARTITION_SIZE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=").rstrip('\n')
PARTITION_TYPE_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_TYPE=").rstrip('\n')
PARTITION_NAME_LIST = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=").rstrip('\n')

BOARD_FLASH_SIZE = util.get_value_from_file(cfg_file, "CONFIG_FLASH_SIZE=").rstrip('\n')

if PARTITION_SIZE_LIST == 'None' or PARTITION_TYPE_LIST == 'None' or PARTITION_NAME_LIST == 'None' or BOARD_FLASH_SIZE == 'None' :
    sys.exit(0)

SIZE_LIST = PARTITION_SIZE_LIST.replace('"','').split(",")
TYPE_LIST = PARTITION_TYPE_LIST.replace('"','').split(",")
NAME_LIST = PARTITION_NAME_LIST.replace('"','').split(",")

print("\n========== Verification of partition settings ==========")

# Check the number of entries of partition lists
if len(SIZE_LIST) != len(TYPE_LIST) or len(SIZE_LIST) != len(NAME_LIST) :
    print("!!!!!!!! ERROR !!!!!!!")
    print("The number of entries of partition list are mismatched. Please Re-configure.")
    print("CONFIG_FLASH_PART_SIZE (%d) : %s" %(len(SIZE_LIST)-1, PARTITION_SIZE_LIST))
    print("CONFIG_FLASH_PART_TYPE (%d) : %s" %(len(TYPE_LIST)-1, PARTITION_TYPE_LIST))
    print("CONFIG_FLASH_PART_NAME (%d) : %s" %(len(NAME_LIST)-1, PARTITION_NAME_LIST))
    sys.exit(1)

FLASH_SIZE = 0
for part in SIZE_LIST :
    if part.isdigit() == True :
        FLASH_SIZE += int(part) * 1024

# Check whether the sum of all partition sizes is not greater than whole flash size
if FLASH_SIZE > int(BOARD_FLASH_SIZE):
    print("!!!!!!!! ERROR !!!!!!!")
    print("The configured partition size is larger than board flash size. Please Re-configure.")
    print("The size of configured size : %d, Board flash size : %d\n" %(FLASH_SIZE, int(BOARD_FLASH_SIZE)))
    sys.exit(1)

print("=> Partition verification SUCCESS!! The setting of all partitions is OK.")
