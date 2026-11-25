#!/usr/bin/env python3
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
import io
import argparse
from contextlib import redirect_stderr

os_folder = os.path.dirname(os.path.abspath(__file__)) + '/..'
cfg_file = os.path.join(os_folder, '.config')
build_folder = os.path.join(os_folder, '..', 'build')

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import config_util as util

def load_configs_from_file():
    configs = {}
    
    configs['CONFIG_ARCH_BOARD'] = util.get_value_from_file(cfg_file, "CONFIG_ARCH_BOARD=").rstrip('\n').strip('"')
    configs['CONFIG_FLASH_VSTART_LOADABLE'] = util.get_value_from_file(cfg_file, "CONFIG_FLASH_VSTART_LOADABLE=").rstrip('\n')
    configs['CONFIG_RAM_START'] = util.get_value_from_file(cfg_file, "CONFIG_RAM_START=").rstrip('\n')
    configs['CONFIG_RAM_SIZE'] = util.get_value_from_file(cfg_file, "CONFIG_RAM_SIZE=").rstrip('\n')
    configs['CONFIG_COMMON_BIN_STATIC_RAMSIZE'] = util.get_value_from_file(cfg_file, "CONFIG_COMMON_BIN_STATIC_RAMSIZE=").rstrip('\n')
    configs['CONFIG_APP1_BIN_DYN_RAMSIZE'] = util.get_value_from_file(cfg_file, "CONFIG_APP1_BIN_DYN_RAMSIZE=").rstrip('\n')
    configs['CONFIG_APP2_BIN_DYN_RAMSIZE'] = util.get_value_from_file(cfg_file, "CONFIG_APP2_BIN_DYN_RAMSIZE=").rstrip('\n')
    configs['CONFIG_USER_SIGN_PREPEND_SIZE'] = util.get_value_from_file(cfg_file, "CONFIG_USER_SIGN_PREPEND_SIZE=").rstrip('\n')
    configs['CONFIG_FLASH_PART_SIZE'] = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_SIZE=").rstrip('\n').strip('"').rstrip(',')
    configs['CONFIG_FLASH_PART_NAME'] = util.get_value_from_file(cfg_file, "CONFIG_FLASH_PART_NAME=").rstrip('\n').strip('"').rstrip(',')
    
    return configs

def calculate_memory_layout(configs):

    path_to_folder = os.path.abspath(os.path.join(build_folder, 'configs', configs['CONFIG_ARCH_BOARD'], 'scripts', 'xipelf'))
    sys.path.append(path_to_folder)
    from flash_offset import get_flash_offset
    offset = get_flash_offset(configs)

    ram_start = int(configs['CONFIG_RAM_START'], 16)
    ram_size = int(configs['CONFIG_RAM_SIZE'])
    ram_end = ram_start + ram_size

    if ram_end % 4096 != 0:
        print("RAM end should be 4KB aligned")
        sys.exit(1)

    common_ram_size = int(configs['CONFIG_COMMON_BIN_STATIC_RAMSIZE']) - 64 * 1024
    app1_ram_size = int(configs['CONFIG_APP1_BIN_DYN_RAMSIZE'])
    app2_ram_size_val = configs.get('CONFIG_APP2_BIN_DYN_RAMSIZE')
    if app2_ram_size_val and app2_ram_size_val != 'None':
        app2_ram_size = int(app2_ram_size_val)
    else:
        app2_ram_size = app1_ram_size

    common_ram_start = ram_end - int(configs['CONFIG_COMMON_BIN_STATIC_RAMSIZE'])
    app1_ram_start = common_ram_start - int(configs['CONFIG_APP1_BIN_DYN_RAMSIZE'])
    app2_ram_start = app1_ram_start - app2_ram_size

    name_list = configs['CONFIG_FLASH_PART_NAME'].split(",") if configs['CONFIG_FLASH_PART_NAME'] else []
    size_list = configs['CONFIG_FLASH_PART_SIZE'].split(",") if configs['CONFIG_FLASH_PART_SIZE'] else []

    signing_offset = 0
    if configs.get('CONFIG_USER_SIGN_PREPEND_SIZE') and configs['CONFIG_USER_SIGN_PREPEND_SIZE'] != 'None':
        signing_offset = int(configs['CONFIG_USER_SIGN_PREPEND_SIZE'])

    memory_layout = {
        "common": {},
        "app1": {"0": {}, "1": {}},
        "app2": {"0": {}, "1": {}}
    }

    ota_index = 0
    current_offset = offset
    change_ota_index = False

    for i, name in enumerate(name_list):
        name = name.strip()
        if i >= len(size_list):
            break
        part_size = int(size_list[i].strip()) * 1024

        if change_ota_index and name == "kernel":
            ota_index = (ota_index + 1) % 2

        if name == "kernel":
            if change_ota_index == True:
                current_offset += part_size
            change_ota_index = True
            continue

        elif name in ("common", "app1", "app2"):
            if name == "common":
                flash_start = current_offset + 0x10 + signing_offset
                flash_size = part_size - 0x10 - signing_offset
                ram_start_val = common_ram_start
                ram_size_val = common_ram_size
            else:
                flash_start = current_offset + 0x30 + signing_offset
                flash_size = part_size - 0x30 - signing_offset
                if name == "app1":
                    ram_start_val = app1_ram_start
                    ram_size_val = app1_ram_size
                else:
                    ram_start_val = app2_ram_start
                    ram_size_val = app2_ram_size

            if current_offset % 4096 != 0:
                print("ERROR: flash start " + str(hex(current_offset)) + " of " + name + "should be aligned to 4KB")
                sys.exit(1)
            if part_size % 4096 != 0:
                print("ERROR: flash partition size" + str(part_size) + " of " + name + "should be aligned to 4KB")
                sys.exit(1)
            layout_entry = {
                "ram_start": hex(ram_start_val),
                "ram_size": hex(ram_size_val),
                "flash_start": hex(flash_start),
                "flash_size": hex(flash_size)
            }

            memory_layout[name][str(ota_index)] = layout_entry
        else:
            continue

        current_offset += part_size

    return memory_layout

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--binary-name', choices=['common', 'app1', 'app2'])
    parser.add_argument('--all', action='store_true',
                        help='Emit layout for all binaries (common, app1, app2)')
    parser.add_argument('--ota-index', type=int, default=0, choices=[0, 1])
    try:
        with redirect_stderr(io.StringIO()):
            args = parser.parse_args()
    except SystemExit:
        sys.exit(0)

    if args.binary_name is None and not args.all:
        sys.exit(0)

    configs = load_configs_from_file()
    memory_layout = calculate_memory_layout(configs)

    if args.all:
        for bin_name in ('common', 'app1', 'app2'):
            entry = memory_layout.get(bin_name, {}).get(str(args.ota_index), {})
            prefix = bin_name.upper()
            print(prefix + "_FLASH_ADDR="+entry.get('flash_start', '0x0'))
            print(prefix + "_FLASH_SIZE="+entry.get('flash_size', '0x0'))
            print(prefix + "_RAM_ADDR="+entry.get('ram_start', '0x0'))
            print(prefix + "_RAM_SIZE="+entry.get('ram_size', '0x0'))
    else:
        entry = memory_layout.get(args.binary_name, {}).get(str(args.ota_index), {})
        print(prefix + "_FLASH_ADDR="+entry.get('flash_start', '0x0'))
        print(prefix + "_FLASH_SIZE="+entry.get('flash_size', '0x0'))
        print(prefix + "_RAM_ADDR="+entry.get('ram_start', '0x0'))
        print(prefix + "_RAM_SIZE="+entry.get('ram_size', '0x0'))

    return

if __name__ == "__main__":
    main()
