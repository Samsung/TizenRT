from __future__ import annotations

import sys
from pathlib import Path

from bk_crc import bk_crc16
from bk_packager import bk_packager_format
from partitions_parser import (
    PartitionsParser,
    read_config,
)


class BKFilBinPack:
    def __init__(self, topdir: Path, workdir: Path):
        self.topdir = topdir
        self.workdir = workdir
        self.config = self._get_config()
        self.board = self.config["CONFIG_ARCH_BOARD"]
        self.fs_bin_name = f"{self.board}_smartfs.bin"
        self.part_map = self._get_mapinfo()
        self.parser = self._get_parser()

    def _pre_handle(self):
        if self.board != "bk7236":
            return
        handler = bk_crc16()
        kernel_bin = self.workdir / self.part_map["kernel"]
        crc_kernel_bin = kernel_bin.parent / ("crc_" + kernel_bin.name)
        handler.crc_file(kernel_bin, crc_kernel_bin)
        kernel_bin.unlink()
        crc_kernel_bin.rename(kernel_bin)

    def _get_config(self) -> dict[str, str]:
        config_file = self.topdir / ".config"
        config = read_config(config_file)
        return config

    def _get_mapinfo(self) -> dict[str, str]:
        bininfo_file = self.topdir / ".bininfo"
        bininfo = read_config(bininfo_file)
        part_map = {
            "bl1": "bootloader.bin",
            "kernel": bininfo["KERNEL_BIN_NAME"],
            "app1": bininfo.get("APP1_BIN_NAME", "None"),
            "app2": bininfo.get("APP2_BIN_NAME", "None"),
            "bootparam": "bootparam.bin",
            "userfs": self.fs_bin_name
        }
        return part_map

    def _get_parser(self) -> PartitionsParser:
        parser = PartitionsParser(self.config)
        required = ["kernel", "bootparam"]
        optional = ["app1", "app2", "userfs"]
        if self.config.get("CONFIG_TFM", "n") != "y":
            required += ["bl1"]
        parser.set_required(required)
        parser.set_optional(optional)
        parser.set_part_map(self.part_map)
        return parser

    def pack(self):
        self._pre_handle()

        parser = self.parser
        pack_json = self.workdir / "partition.json"
        parser.gen_partitions_json(pack_json)
        fs_bin = self.workdir / self.fs_bin_name
        if not fs_bin.exists():
            parser.create_dummy_fs_bin(fs_bin)
        packager = bk_packager_format(self.workdir, pack_json)
        if self.config.get("CONFIG_TFM", "n") == "y":
            packager.set_magic_header(b'BKDLV10.')
        packager.pack()


if __name__ == "__main__":
    topdir = Path(sys.argv[1])
    workdir = Path(sys.argv[2])
    assert topdir.exists()
    assert workdir.exists()
    packer = BKFilBinPack(topdir, workdir)
    packer.pack()
