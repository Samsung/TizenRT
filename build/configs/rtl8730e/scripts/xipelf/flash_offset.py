#!/usr/bin/env python
###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
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

def get_flash_offset(configs):
    sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '../../../../tools/amebasmart/gnu_utility')))
    from loadable_xip_elf import get_offset
    offset_shift = get_offset()
    offset = int(configs['CONFIG_FLASH_VSTART_LOADABLE'], 16) - int(offset_shift, 16)
    name_list = configs['CONFIG_FLASH_PART_NAME'].split(",") if configs['CONFIG_FLASH_PART_NAME'] else []
    size_list = configs['CONFIG_FLASH_PART_SIZE'].split(",") if configs['CONFIG_FLASH_PART_SIZE'] else []
    for i, name in enumerate(name_list):
        if name == "kernel":
            part_size = int(size_list[i].strip()) * 1024
            offset += part_size
            break
    return offset

