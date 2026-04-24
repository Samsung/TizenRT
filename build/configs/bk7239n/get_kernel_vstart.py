#!/usr/bin/env python3
#
# Compute bk7239n kernel flash code start from .config partition data.

import argparse
import sys


DEFAULT_PRIMARY_TFM_S_BYTES = 0x30000


def _read_config_partition_start(config_path):
    part_size_str = None
    part_name_str = None

    with open(config_path, "r", encoding="utf-8") as config_file:
        for line in config_file:
            stripped = line.strip()
            if stripped.startswith("#"):
                continue
            if stripped.startswith("CONFIG_FLASH_PART_SIZE="):
                part_size_str = stripped.split("=", 1)[1].strip().strip('"').rstrip(",")
            elif stripped.startswith("CONFIG_FLASH_PART_NAME="):
                part_name_str = stripped.split("=", 1)[1].strip().strip('"').rstrip(",")

    if not part_size_str or not part_name_str:
        raise ValueError("CONFIG_FLASH_PART_SIZE or CONFIG_FLASH_PART_NAME not found")

    names = [name.strip() for name in part_name_str.split(",") if name.strip()]
    sizes = [size.strip() for size in part_size_str.split(",") if size.strip()]
    if len(names) != len(sizes):
        raise ValueError("Partition name/size counts do not match")

    offset_bytes = 0
    for name, size_kb in zip(names, sizes):
        if name == "kernel":
            return offset_bytes
        offset_bytes += int(size_kb) * 1024

    raise ValueError("No kernel partition found")


def _compute_cpu0_app_vstart(config_path):
    return _read_config_partition_start(config_path) + DEFAULT_PRIMARY_TFM_S_BYTES


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", required=True, help="Path to .config")
    args = parser.parse_args()

    cpu0_app_vstart = _compute_cpu0_app_vstart(args.config)
    print(hex(cpu0_app_vstart))


if __name__ == "__main__":
    try:
        main()
    except Exception as error:
        print("Error: %s" % error, file=sys.stderr)
        sys.exit(1)
