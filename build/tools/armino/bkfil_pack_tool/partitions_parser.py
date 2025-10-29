from __future__ import annotations

import json
import logging
import re
from dataclasses import asdict, dataclass
from pathlib import Path

logger = logging.getLogger(__name__)


@dataclass
class PartitionInfo:
    name: str
    offset: int  # KB
    size: int  # KB


@dataclass
class PackPartition:
    partition: str
    firmware: str
    start_addr: int
    size: int


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


class PartitionsParser:
    def __init__(self, config: dict[str, str]):
        self.config = config
        self.part_map: dict[str, str] = {}
        self.required = ["bl1", "kernel", "app1", "bootparam"]
        self.optional = ["bl2", "app2", "smartfs"]

    def set_part_map(self, part_map: dict[str, str]):
        self.part_map = part_map

    def set_required(self, required: list[str]):
        self.required = required

    def set_optional(self, optional: list[str]):
        self.optional = optional

    def gen_partitions_json(self, output_json: Path):
        partitions_name = self.config["CONFIG_FLASH_PART_NAME"]
        partitions_size = self.config["CONFIG_FLASH_PART_SIZE"]
        partitions = self.parse_partitions(partitions_name, partitions_size)
        partitions = self.filter_partitions(partitions, self.required, self.optional)
        self.pack_partitions = self.get_pack_partitions(partitions, self.part_map)
        self.save_partitions(self.pack_partitions, output_json)

    def create_dummy_fs_bin(self, dummy_fs_bin: Path):
        for part in self.pack_partitions:
            if part.partition == "userfs":
                dummy_fs_bin.write_bytes(bytes([0xff]) * part.size)
                logger.info(f"Created dummy userfs binary file at {dummy_fs_bin} with size {part.size}")

    @staticmethod
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

    @staticmethod
    def save_partitions(partitions: list[PackPartition], output_file: Path):
        pack_json = {}
        pack_json["magic"] = "BEKEN"
        pack_json["count"] = len(partitions)
        pack_json["section"] = [asdict(p) for p in partitions]
        output_file.write_text(json.dumps(pack_json, indent=4))
        logger.info(f"Save partitions to {output_file}")

    @staticmethod
    def filter_partitions(
        partitions: list[PartitionInfo], required: list[str], optional: list[str]
    ) -> list[PartitionInfo]:
        for name in required:
            if name not in [p.name for p in partitions]:
                raise ValueError(f"Required partition {name} not found")
        all_partitions = required + optional
        checked_partitions: list[PartitionInfo] = []
        for p in partitions:
            if p.name in all_partitions:
                if p.name in [x.name for x in checked_partitions]:
                    continue
                checked_partitions.append(p)
        return checked_partitions

    @staticmethod
    def get_pack_partitions(
        partitions: list[PartitionInfo], part_map: dict[str, str]
    ) -> list[PackPartition]:
        pack_partitions: list[PackPartition] = []
        for p in partitions:
            if p.name in part_map:
                part_file = part_map[p.name]
            else:
                part_file = p.name + ".bin"
            pack_partitions.append(
                PackPartition(p.name, part_file, p.offset * 1024, p.size * 1024)
            )
        return pack_partitions
