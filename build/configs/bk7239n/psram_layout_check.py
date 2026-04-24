#!/usr/bin/env python3
# PSRAM layout check:
# - Validates PSRAM regions (0x7xxxxxxx) do not overlap with each other.
# - Only checks RAM_KREGIONx entries in PSRAM address range.
# - For XIP_ELF: ensures CONFIG_RAM_START (user app RAM) is NOT in RAM_KREGIONx,
#   to avoid kernel heap corrupting user app data/bss.
import sys

# PSRAM address range on bk7239n: 0x70000000 - 0x78000000
PSRAM_ADDR_MASK = 0x70000000


def load_config(path):
    config = {}
    try:
        with open(path, "r", encoding="utf-8") as infile:
            for line in infile:
                line = line.strip()
                if not line:
                    continue
                if line.startswith("#") and " is not set" in line:
                    key = line.split(" ", 1)[1].split(" is not set", 1)[0]
                    config[key] = "n"
                    continue
                if line.startswith("CONFIG_") and "=" in line:
                    key, value = line.split("=", 1)
                    config[key] = value.strip()
    except FileNotFoundError:
        raise RuntimeError(f"Config file not found: {path}")
    return config


def parse_int(value, name, default=None):
    if value is None:
        if default is None:
            raise RuntimeError(f"Missing {name} in .config")
        return default
    value = value.strip().strip('"')
    try:
        return int(value, 0)
    except ValueError as exc:
        raise RuntimeError(f"Invalid integer for {name}: {value}") from exc


def parse_list(value, name):
    if value is None:
        raise RuntimeError(f"Missing {name} in .config")
    value = value.strip().strip('"')
    if not value:
        return []
    return [item for item in value.split(",") if item]


def region_overlap(a_start, a_end, b_start, b_end):
    return a_start < b_end and b_start < a_end


def kregion_label(index, app_heap_index):
    if app_heap_index is None:
        return f"RAM_KREGION{index}"
    if index == app_heap_index:
        return f"RAM_KREGION{index} (app heap)"
    return f"RAM_KREGION{index} (kernel heap)"


def main():
    if len(sys.argv) != 2:
        raise RuntimeError("Usage: psram_layout_check.py <path-to-.config>")

    config = load_config(sys.argv[1])

    app_heap_index_value = config.get("CONFIG_HEAP_INDEX_LOADED_APP")
    app_heap_index = None
    if app_heap_index_value is not None:
        app_heap_index = parse_int(app_heap_index_value, "CONFIG_HEAP_INDEX_LOADED_APP")

    start_list = parse_list(
        config.get("CONFIG_RAM_KREGIONx_START"), "CONFIG_RAM_KREGIONx_START"
    )
    size_list = parse_list(
        config.get("CONFIG_RAM_KREGIONx_SIZE"), "CONFIG_RAM_KREGIONx_SIZE"
    )

    if len(start_list) != len(size_list):
        raise RuntimeError("CONFIG_RAM_KREGIONx_START/SIZE length mismatch")

    # Collect only PSRAM regions (0x7xxxxxxx)
    psram_regions = []
    for idx, (start_str, size_str) in enumerate(zip(start_list, size_list)):
        start = parse_int(start_str, f"CONFIG_RAM_KREGIONx_START[{idx}]")
        size = parse_int(size_str, f"CONFIG_RAM_KREGIONx_SIZE[{idx}]")
        if size <= 0:
            continue
        # Only check regions in PSRAM address range (0x7xxxxxxx)
        if (start & 0xF0000000) != PSRAM_ADDR_MASK:
            continue
        end = start + size
        label = kregion_label(idx, app_heap_index)
        psram_regions.append((idx, start, end, label))

    # Check for overlaps between PSRAM regions only
    for i in range(len(psram_regions)):
        for j in range(i + 1, len(psram_regions)):
            idx_a, a_start, a_end, label_a = psram_regions[i]
            idx_b, b_start, b_end, label_b = psram_regions[j]
            if region_overlap(a_start, a_end, b_start, b_end):
                raise RuntimeError(
                    f"{label_a} and {label_b} overlap; check CONFIG_RAM_KREGIONx_START/SIZE"
                )

    # XIP_ELF: CONFIG_RAM_START is reserved for user apps. It must NOT be in
    # RAM_KREGIONx, or kernel heap allocations will corrupt user app data/bss.
    if config.get("CONFIG_XIP_ELF") in ("y", "Y"):
        ram_start = parse_int(config.get("CONFIG_RAM_START"), "CONFIG_RAM_START", default=0)
        ram_size = parse_int(config.get("CONFIG_RAM_SIZE"), "CONFIG_RAM_SIZE", default=0)
        if ram_start and ram_size and (ram_start & 0xF0000000) == PSRAM_ADDR_MASK:
            user_start, user_end = ram_start, ram_start + ram_size
            for idx, k_start, k_end, label in psram_regions:
                if region_overlap(k_start, k_end, user_start, user_end):
                    raise RuntimeError(
                        f"{label} overlaps with CONFIG_RAM_START (0x{ram_start:x}). "
                        "For XIP_ELF, user app RAM must be exclusive - do not include "
                        "CONFIG_RAM_START in CONFIG_RAM_KREGIONx_START."
                    )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as exc:
        print(f"[psram_layout_check] ERROR: {exc}", file=sys.stderr)
        sys.exit(1)
