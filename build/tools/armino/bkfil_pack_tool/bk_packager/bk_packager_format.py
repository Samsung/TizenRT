import logging
from pathlib import Path

from .bk_packager import bk_packager
from .bk_packager_format_linker import bk_packager_format_linker

logger = logging.getLogger(__package__)


class bk_packager_format(bk_packager):
    def __init__(
        self,
        workdir: Path,
        pack_json: Path,
        output_file_name: Path = Path("all-app.bin"),
    ):
        self._linker = bk_packager_format_linker()
        super().__init__(workdir, pack_json, self._linker, output_file_name)
        logger.info(f"use {self.__class__.__name__} to pack {output_file_name}")

    def _pre_link(self):
        pass

    def _post_link(self):
        pass

    def set_magic_header(self, magic_header: bytes):
        self._linker.set_magic_header(magic_header)
