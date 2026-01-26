from __future__ import annotations

import logging
import re
import sys
from dataclasses import dataclass
from pathlib import Path

logger = logging.getLogger(__name__)


@dataclass
class PartitionInfo:
    name: str
    offset: int  # KB
    size: int  # KB


@dataclass
class SecurityPatitionInfo:
    name: str
    type: str
    size: str
    subtype: str = ""
    offset: str = ""
    flag: str = ""


class SecurityPatition:
    def __init__(self) -> None:
        self._partitions: list[SecurityPatitionInfo] = []
        self._offset: int = 0

    def add_partition(self, part: SecurityPatitionInfo) -> None:
        part.offset = hex(self._offset)
        self._partitions.append(part)
        self._offset += int(part.size.strip("K")) * 1024

    @property
    def partitions(self) -> list[SecurityPatitionInfo]:
        return self._partitions

    def extend_partitions(self, part: list[SecurityPatitionInfo]) -> None:
        for p in part:
            self.add_partition(p)

    def __eq__(self, value: object, /) -> bool:
        if not isinstance(value, SecurityPatition):
            return False
        return self._partitions == value._partitions


def read_config(config_file: Path) -> dict[str, str]:
    config: dict[str, str] = {}
    with config_file.open("r") as f:
        for line in f:
            line = line.strip()
            if line.startswith("#"):
                continue
            match = re.match(r"^([^=]+)=\"?([^\"]*)\"?$", line)
            if match:
                key = match.group(1)
                value = match.group(2)
                config[key] = value
    return config


def parse_partitions(parts_name: str, parts_size: str) -> list[PartitionInfo]:
    parts_name = parts_name.strip(",")
    parts_size = parts_size.strip(",")
    parts_name_list = parts_name.split(",")
    parts_size_list = parts_size.split(",")
    partitions: list[PartitionInfo] = []
    offset = 0
    for name, size in zip(parts_name_list, parts_size_list):
        partitions.append(PartitionInfo(name, offset, int(size)))
        offset += int(size)
    return partitions


def parse_bl1_partitions(part: PartitionInfo) -> list[SecurityPatitionInfo]:
    assert part.name == "bl1"
    assert part.size == 92
    bl1_partitions = [
        SecurityPatitionInfo(name="bl1_control", type="app", size="4K"),
        SecurityPatitionInfo(name="boot_flag", type="data", size="4K"),
        SecurityPatitionInfo(name="partition", type="data", size="4K"),
        SecurityPatitionInfo(name="primary_manifest", type="data", size="4K"),
        SecurityPatitionInfo(name="bl2", type="app", size="76K"),
    ]
    return bl1_partitions


def parse_normal_partitions(part: PartitionInfo, repeat: bool) -> SecurityPatitionInfo:
    part_type = "data"
    if part.name == "sys_its":
        subtype = "its"
    elif part.name == "sys_ps":
        subtype = "ps"
    else:
        subtype = ""
    name = part.name + "_B" if repeat else part.name
    return SecurityPatitionInfo(
        name, part_type, str(part.size) + "K", subtype=subtype
    )


def parse_bl2_partitions(part: PartitionInfo) -> list[SecurityPatitionInfo]:
    assert part.name == "bl2"
    assert part.size == 76
    bl2_partitions = [
        SecurityPatitionInfo(name="bl2_B", type="app", size="72K"),
        SecurityPatitionInfo(name="partition_B", type="data", size="4K"),
    ]
    return bl2_partitions


def parse_kernel_partitions(
    part: PartitionInfo, kernel_existed: bool
) -> list[SecurityPatitionInfo]:
    assert part.name == "kernel"
    assert part.size > 192
    if kernel_existed:
        prefix_name = "secondary_"
    else:
        prefix_name = "primary_"
    kernel_size = str((part.size - 192)) + "K"
    kernel_partitions = [
        SecurityPatitionInfo(name=prefix_name + "tfm_s", type="app", size="192K"),
        SecurityPatitionInfo(name=prefix_name + "cpu0_app", type="app", size=kernel_size),
    ]
    return kernel_partitions


def parse_security_partitions(
    partitions: list[PartitionInfo],
) -> SecurityPatition:
    security_partitions = SecurityPatition()
    parts_name = [part.name for part in partitions]
    repeat_list = [parts_name.count(item) > 1 and parts_name.index(item) != index for index, item in enumerate(parts_name)]
    for part, repeat in zip(partitions, repeat_list):
        if part.name.startswith("bl1"):
            bl1_partitions = parse_bl1_partitions(part)
            security_partitions.extend_partitions(bl1_partitions)
        elif part.name.startswith("bl2"):
            bl2_partitions = parse_bl2_partitions(part)
            security_partitions.extend_partitions(bl2_partitions)
        elif part.name == "kernel":
            kernel_partitions = parse_kernel_partitions(part, repeat)
            security_partitions.extend_partitions(kernel_partitions)
        else:
            noraml_part = parse_normal_partitions(part, repeat)
            security_partitions.add_partition(noraml_part)
    return security_partitions


def gen_partitions_csv(part_list: SecurityPatition, csv_path: Path):
    content = "Name,               Type, SubType, Offset, Size, Flags\n"
    for part in part_list.partitions:
        content += (
            f"{part.name + ',':<20}{part.type + ',':6}{part.subtype + ',':9}"
            + f"{part.offset + ',':10}{part.size + ',':6}{part.flag}\n"
        )
    csv_path.write_text(content)


def adapt_partitions(config_file: Path, csv_path: Path):
    assert config_file.is_file()
    assert csv_path.parent.exists()
    config = read_config(config_file)
    parts_name = config["CONFIG_FLASH_PART_NAME"]
    parts_size = config["CONFIG_FLASH_PART_SIZE"]
    parts = parse_partitions(parts_name, parts_size)
    part_list = parse_security_partitions(parts)
    gen_partitions_csv(part_list, csv_path)


if __name__ == "__main__":
    config_file = Path(sys.argv[1])
    csv_path = Path(sys.argv[2])
    adapt_partitions(config_file, csv_path)
