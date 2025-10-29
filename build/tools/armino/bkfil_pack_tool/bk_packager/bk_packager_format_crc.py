import logging
from pathlib import Path

from . import bk_packager_crc_decorator
from .bk_packager import bk_packager
from .bk_packager_format_linker import bk_packager_format_linker

logger = logging.getLogger(__package__)


class bk_packager_format_crc(bk_packager):
    def __init__(
        self,
        workdir: Path,
        pack_json: Path,
        output_file_name: Path = Path("all-app.bin"),
    ):
        linker = bk_packager_format_linker()
        super().__init__(workdir, pack_json, linker, output_file_name)
        logger.info(f"use {self.__class__.__name__} to pack {output_file_name}")

    @bk_packager_crc_decorator.pre_link
    def _pre_link(self):
        pass

    @bk_packager_crc_decorator.post_link
    def _post_link(self):
        pass
