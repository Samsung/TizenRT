#!/usr/bin/env python
#
# Compute CONFIG_FLASH_VSTART_LOADABLE from partition table:
# first 'common' partition physical start + 0x12000000.
#
# - Called by mkldscript.py: get_common_partition_vstart(name_list_str, size_list_str)
# - Can also run standalone:
#   python get_flash_vstart_loadable.py <path_to_defconfig_or_.config>
#

from __future__ import print_function
import os
import sys

FLASH_VSTART_LOADABLE_BASE = 0x12000000

def get_xip_flash_start(name_list_str, size_list_str):
    """
    From partition name/size list strings (comma-separated), find
    the first 'common' partition and return its XIP virtual start address
    (physical offset + 0x12000000) as hex string. Returns None if not found.
    """
    if not name_list_str or not size_list_str:
        return None
    names = [n.strip() for n in name_list_str.split(",") if n.strip()]
    sizes = [s.strip() for s in size_list_str.split(",") if s.strip()]
    if len(names) != len(sizes):
        return None
    offset_bytes = 0
    for name, size_kb in zip(names, sizes):
        if name == "common":
            return hex(FLASH_VSTART_LOADABLE_BASE + offset_bytes)
        try:
            offset_bytes += int(size_kb) * 1024
        except ValueError:
            return None
    return None

def main():
    if len(sys.argv) < 2:
        print("Usage: %s <path_to_defconfig_or_.config>" % sys.argv[0], file=sys.stderr)
        sys.exit(1)
    config_path = sys.argv[1]
    if not os.path.isfile(config_path):
        print("Error: file not found: %s" % config_path, file=sys.stderr)
        sys.exit(1)
    part_size_str = None
    part_name_str = None
    with open(config_path, 'r') as f:
        for line in f:
            if line.strip().startswith('#'):
                continue
            if "CONFIG_FLASH_PART_SIZE=" in line:
                part_size_str = line.split("=", 1)[1].strip().strip('"').rstrip(',')
            elif "CONFIG_FLASH_PART_NAME=" in line:
                part_name_str = line.split("=", 1)[1].strip().strip('"').rstrip(',')
    if not part_size_str or not part_name_str:
        print("Error: CONFIG_FLASH_PART_SIZE or CONFIG_FLASH_PART_NAME not found", file=sys.stderr)
        sys.exit(1)
    value = get_xip_flash_start(part_name_str, part_size_str)
    if value is None:
        print("Error: no 'common' partition found", file=sys.stderr)
        sys.exit(1)
    print(value)

if __name__ == "__main__":
    main()
