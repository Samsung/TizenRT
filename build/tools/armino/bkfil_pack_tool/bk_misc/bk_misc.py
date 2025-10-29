from __future__ import annotations


def parse_format_size(size_str: str) -> int:
    size_str = size_str.strip().upper()
    if size_str.endswith("K"):
        return int(size_str[:-1]) * 1024
    elif size_str.endswith("M"):
        return int(size_str[:-1]) * 1024 * 1024
    elif size_str.endswith("G"):
        return int(size_str[:-1]) * 1024 * 1024 * 1024
    elif size_str.endswith("KB"):
        return int(size_str[:-2].strip()) * 1024
    elif size_str.endswith("MB"):
        return int(size_str[:-2].strip()) * 1024 * 1024
    elif size_str.endswith("GB"):
        return int(size_str[:-2].strip()) * 1024 * 1024 * 1024
    elif size_str.endswith("B"):
        return int(size_str[:-1])
    else:
        return int(size_str)


def format_size(size: int) -> str:
    for val, suffix in [(0x100000, "M"), (0x400, "K")]:
        if size % val == 0:
            return f"{size // val}{suffix}"
    return str(size)


def check_overlaps(spaces: list[tuple[int, int]]) -> bool:
    intervals = [(start, start + length) for start, length in spaces]
    intervals.sort()
    for i in range(1, len(intervals)):
        if intervals[i][0] < intervals[i - 1][1]:
            return True
    return False
