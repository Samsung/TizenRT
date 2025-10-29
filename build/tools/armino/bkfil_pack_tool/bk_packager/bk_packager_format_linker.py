from __future__ import annotations

import logging
import struct
from pathlib import Path

from bk_crc import bk_crc32

from .bk_packager_json import PartitionInfo
from .bk_packager_linker import bk_packager_linker

logger = logging.getLogger(__package__)


class bk_packager_format_linker(bk_packager_linker):
    def __init__(self):
        self.crc_handler = bk_crc32()
        self.magic_header = b"BL1DLV10"

    def _gen_img_header(self, part_item: PartitionInfo, part_type: int):
        img_header = bytearray()
        img_header += struct.pack(">I", part_item.addr)  # partition_offset
        img_header += struct.pack(">I", part_item.size)  # partition_size
        img_header += struct.pack(">I", part_item.addr)  # flash_start_addr
        img_header += struct.pack(">I", self.part_offset[part_item.name])  # img_offset
        img_header += struct.pack(">I", len(part_item.data))  # img_len
        img_header += struct.pack(
            ">I", self.crc_handler.crc_data(part_item.data)
        )  # checksum
        img_header += struct.pack(">I", 0)  # version
        img_header += struct.pack(">H", part_type)  # type
        img_header += struct.pack(">H", 0)  # reserved
        logger.debug(f"{part_item.name} header len {len(img_header)}")
        return img_header

    def _gen_global_header(self):
        global_header = bytearray()
        global_header += struct.pack("8s", self.magic_header)

        header_info = bytearray()
        header_info += struct.pack(">I", 1)
        header_info += struct.pack(">H", 32)
        header_info += struct.pack(">H", len(self.part_info))
        header_info += struct.pack(">I", 0)
        header_info += struct.pack(">I", 0)
        header_info += struct.pack(">I", 0)

        global_header += struct.pack(
            ">I", self.crc_handler.crc_data(bytes(header_info))
        )
        global_header += header_info
        logger.debug(f"global header len {len(global_header)}")
        self.global_header = bytes(global_header)

    def _gen_img_table(self):
        img_table = bytearray()
        for part_item in self.part_info:
            img_table += self._gen_img_header(part_item, 0)

        self.img_table = bytes(img_table)

    def _read_all_part_bin_content(self):
        all_data_array = bytearray()
        img_offset = 32 + 32 * len(self.part_info)
        self.part_offset: dict[str, int] = dict()
        offset = 0
        for part_item in self.part_info:
            self.part_offset[part_item.name] = img_offset + offset
            all_data_array += part_item.data
            offset += len(part_item.data)

        self.all_data = bytes(all_data_array)

    def link(self, part_info: list[PartitionInfo], output_file: Path):
        self.part_info = part_info
        self._read_all_part_bin_content()
        self._gen_global_header()
        self._gen_img_table()

        with output_file.open("wb") as f:
            f.write(self.global_header)
            f.write(self.img_table)
            f.write(self.all_data)
        logger.info(f"save binary to {output_file}")

    def set_magic_header(self, magic_header: bytes):
        if len(magic_header) != 8:
            raise ValueError("magic header must be 8 bytes")
        self.magic_header = magic_header
