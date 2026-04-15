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
import subprocess

os_folder = os.path.dirname(__file__) + '/../../../../../os'
cfg_file = os_folder + '/.config'

CONFIG_FLASH_VSTART_LOADABLE = None

def compute_flash_vstart_loadable(configs):
    global CONFIG_FLASH_VSTART_LOADABLE
    _vstart_script = os.path.abspath(os.path.join(os.path.dirname(__file__), '../../get_flash_vstart_loadable.py'))
    computed = subprocess.check_output([sys.executable, _vstart_script, os.path.abspath(cfg_file)]).decode('utf-8').strip()
    if computed:
        CONFIG_FLASH_VSTART_LOADABLE = computed
    else:
        CONFIG_FLASH_VSTART_LOADABLE = configs['CONFIG_FLASH_VSTART_LOADABLE']

def get_flash_offset(configs):
    global CONFIG_FLASH_VSTART_LOADABLE
    if CONFIG_FLASH_VSTART_LOADABLE is None:
        compute_flash_vstart_loadable(configs)
    return int(CONFIG_FLASH_VSTART_LOADABLE, 16)

