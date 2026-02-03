#!/usr/bin/env python3
# PSRAM layout check:
# - Validates PSRAM data/code/heap regions do not overlap or exceed PSRAM size.
# - Ensures PSRAM code base equals PSRAM data base + size.
# - Verifies PSRAM heaps (RAM_KREGIONx in PSRAM range) do not overlap data/code.
#
# PSRAM layout (example, not to scale):
#   PSRAM_BASE
#   +----------------------------+  <-- CONFIG_BK_PSRAM_BASE (default 0x70000000)
#   | Secure (reserved, !CONFIG_XIP_KERNEL) |  <-- CONFIG_BK_PSRAM_SECURE_SIZE (default 0x40000)
#   +----------------------------+  <-- CONFIG_BK_PSRAM_BASE + SECURE_SIZE
#   | Kernel PSRAM data          |  <-- CONFIG_BK_PSRAM_DATA_SIZE (default 0x20000)
#   +----------------------------+
#   | Kernel PSRAM code          |  <-- CONFIG_BK_PSRAM_CODE_SIZE
#   +----------------------------+
#   | Kernel/App heaps (PSRAM)   |  <-- CONFIG_RAM_KREGIONx_* in PSRAM range
#   | ...                        |
#   +----------------------------+  <-- CONFIG_BK_PSRAM_BASE + TOTAL_SIZE
#
# Default layout when CONFIG_XIP_KERNEL=y:
#   kernel data base  : 0x70000000
#   kernel data size  : 0x20000
#   kernel code base  : 0x70020000  (must equal data base + data size)
#   kernel code size  : 0x20000
#   kernel heap (PSRAM, from CONFIG_RAM_KREGIONx_* in loadable_apps):
#     RAM_KREGION0 : 0x70040000 size 0x3C0000
#   app heap (PSRAM, from CONFIG_RAM_KREGIONx_* in loadable_apps):
#     RAM_KREGION2 : 0x70400000 size 0x400000
#   (RAM_KREGION1 is SRAM: 0x28000000 size 0x80000)
#
# Default layout when !CONFIG_XIP_KERNEL:
#   PSRAM_BASE       : 0x70000000
#   PSRAM_SECURE_SIZE     : 0x40000 (first 256KB reserved)
#   kernel data base  : 0x70040000
#   kernel data size  : 0x20000
#   kernel code base  : 0x70060000  (must equal data base + data size)
#   kernel code size  : 0x180000
#   kernel heap (PSRAM, from CONFIG_RAM_KREGIONx_* in loadable_all):
#     RAM_KREGION0 : 0x701E0000 size 0x320000
#   app heap (PSRAM, from CONFIG_RAM_KREGIONx_* in loadable_all):
#     RAM_KREGION2 : 0x70500000 size 0x300000
#   (RAM_KREGION1 is SRAM: 0x28000000 size 0x80000)
import sys


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

    psram_base = parse_int(config.get("CONFIG_BK_PSRAM_BASE"), "CONFIG_BK_PSRAM_BASE")
    psram_total = parse_int(
        config.get("CONFIG_BK_PSRAM_TOTAL_SIZE"),
        "CONFIG_BK_PSRAM_TOTAL_SIZE",
    )
    psram_secure = parse_int(
        config.get("CONFIG_BK_PSRAM_SECURE_SIZE"),
        "CONFIG_BK_PSRAM_SECURE_SIZE",
    )

    xip_kernel = config.get("CONFIG_XIP_KERNEL", "n") == "y"
    data_base = parse_int(
        config.get("CONFIG_BK_PSRAM_DATA_BASE"),
        "CONFIG_BK_PSRAM_DATA_BASE",
    )
    data_size = parse_int(
        config.get("CONFIG_BK_PSRAM_DATA_SIZE"),
        "CONFIG_BK_PSRAM_DATA_SIZE",
    )
    code_base = parse_int(
        config.get("CONFIG_BK_PSRAM_CODE_BASE"),
        "CONFIG_BK_PSRAM_CODE_BASE",
    )
    code_size = parse_int(
        config.get("CONFIG_BK_PSRAM_CODE_SIZE"),
        "CONFIG_BK_PSRAM_CODE_SIZE",
    )

    psram_end = psram_base + psram_total
    if xip_kernel:
        effective_secure = 0
    else:
        effective_secure = psram_secure
    secure_end = psram_base + effective_secure
    if effective_secure > psram_total:
        raise RuntimeError("CONFIG_BK_PSRAM_SECURE_SIZE exceeds total PSRAM size")

    if not xip_kernel and data_base != secure_end:
        raise RuntimeError(
            "CONFIG_BK_PSRAM_DATA_BASE must be 0x%08X when !XIP_KERNEL"
            % secure_end
        )

    def require_region(name, start, size):
        if size <= 0:
            raise RuntimeError(f"{name} size must be > 0")
        end = start + size
        if start < psram_base or end > psram_end:
            raise RuntimeError(
                f"{name} [{hex(start)}, {hex(end)}) is outside PSRAM "
                f"[{hex(psram_base)}, {hex(psram_end)})"
            )
        if region_overlap(start, end, psram_base, secure_end):
            raise RuntimeError(
                f"{name} [{hex(start)}, {hex(end)}) overlaps secure region "
                f"[{hex(psram_base)}, {hex(secure_end)})"
            )
        return end

    data_end = require_region("PSRAM data", data_base, data_size)
    code_end = require_region("PSRAM code", code_base, code_size)

    if code_base != data_end:
        raise RuntimeError(
            "PSRAM code base must equal PSRAM data end "
            f"({hex(data_end)}), got {hex(code_base)}"
        )

    if region_overlap(data_base, data_end, code_base, code_end):
        raise RuntimeError("PSRAM data and PSRAM code regions overlap")

    start_list = parse_list(
        config.get("CONFIG_RAM_KREGIONx_START"), "CONFIG_RAM_KREGIONx_START"
    )
    size_list = parse_list(
        config.get("CONFIG_RAM_KREGIONx_SIZE"), "CONFIG_RAM_KREGIONx_SIZE"
    )
    app_heap_index_value = config.get("CONFIG_HEAP_INDEX_LOADED_APP")
    app_heap_index = None
    if app_heap_index_value is not None:
        app_heap_index = parse_int(app_heap_index_value, "CONFIG_HEAP_INDEX_LOADED_APP")

    if len(start_list) != len(size_list):
        raise RuntimeError("CONFIG_RAM_KREGIONx_START/SIZE length mismatch")

    kregions = []
    for idx, (start_str, size_str) in enumerate(zip(start_list, size_list)):
        start = parse_int(start_str, f"CONFIG_RAM_KREGIONx_START[{idx}]")
        size = parse_int(size_str, f"CONFIG_RAM_KREGIONx_SIZE[{idx}]")
        label = kregion_label(idx, app_heap_index)
        if size <= 0:
            raise RuntimeError(f"{label} size must be > 0")
        end = start + size
        if start < psram_base or start >= psram_end:
            continue
        if end > psram_end:
            raise RuntimeError(
                f"{label} [{hex(start)}, {hex(end)}) exceeds PSRAM "
                f"[{hex(psram_base)}, {hex(psram_end)}); "
                "check CONFIG_RAM_KREGIONx_START/SIZE"
            )
        if region_overlap(start, end, psram_base, secure_end):
            raise RuntimeError(
                f"{label} [{hex(start)}, {hex(end)}) overlaps secure region "
                f"[{hex(psram_base)}, {hex(secure_end)}); "
                "RAM_KREGIONx from CONFIG_RAM_KREGIONx_START/SIZE, "
                "secure region from CONFIG_BK_PSRAM_SECURE_SIZE"
            )
        if region_overlap(start, end, data_base, data_end):
            raise RuntimeError(
                f"{label} [{hex(start)}, {hex(end)}) overlaps PSRAM data "
                f"[{hex(data_base)}, {hex(data_end)}); "
                "RAM_KREGIONx from CONFIG_RAM_KREGIONx_START/SIZE, "
                "PSRAM data from CONFIG_BK_PSRAM_DATA_BASE/SIZE"
            )
        if region_overlap(start, end, code_base, code_end):
            raise RuntimeError(
                f"{label} [{hex(start)}, {hex(end)}) overlaps PSRAM code "
                f"[{hex(code_base)}, {hex(code_end)}); "
                "RAM_KREGIONx from CONFIG_RAM_KREGIONx_START/SIZE, "
                "PSRAM code from CONFIG_BK_PSRAM_CODE_BASE/SIZE"
            )
        kregions.append((idx, start, end))

    for i in range(len(kregions)):
        for j in range(i + 1, len(kregions)):
            idx_a, a_start, a_end = kregions[i]
            idx_b, b_start, b_end = kregions[j]
            if region_overlap(a_start, a_end, b_start, b_end):
                label_a = kregion_label(idx_a, app_heap_index)
                label_b = kregion_label(idx_b, app_heap_index)
                raise RuntimeError(
                    f"{label_a} overlaps {label_b}; "
                    "check CONFIG_RAM_KREGIONx_START/SIZE"
                )


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as exc:
        print(f"[psram_layout_check] ERROR: {exc}", file=sys.stderr)
        sys.exit(1)

