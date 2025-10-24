from __future__ import annotations

import logging
from pathlib import Path
from typing import BinaryIO

from .bk_packager_json import PartitionInfo
from .bk_packager_linker import bk_packager_linker

logger = logging.getLogger(__package__)


class bk_packager_linear_linker(bk_packager_linker):
    def __init__(self):
        super().__init__()
        self.start_padding_mode = False

    def _write_binary(self, part_info: list[PartitionInfo], fp: BinaryIO):
        curr_pos = 0
        for part_item in part_info:
            part_addr = part_item.addr
            logger.debug(f"part name: {part_item.name}")
            logger.debug(f"part_addr: {part_addr}")
            if curr_pos > part_addr:
                raise RuntimeError("partitions layout error")
            if curr_pos or self.start_padding_mode:
                padding_len = part_addr - curr_pos
            else:
                padding_len = 0
            logger.debug(f"padding_len: {padding_len}")
            if padding_len:
                fp.write(bytes([0xFF] * padding_len))
            curr_pos = part_addr
            part_bin_content = part_item.data
            fp.write(part_bin_content)
            curr_pos += len(part_bin_content)

    def link(self, part_info: list[PartitionInfo], output_file: Path):
        if output_file.exists():
            output_file.unlink()
        with output_file.open("wb") as f:
            self._write_binary(part_info, f)
        logger.info(f"save binary to {output_file}")

    def set_start_padding_mode(self, mode: bool):
        self.start_padding_mode = mode
