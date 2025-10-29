import logging
from abc import ABC, abstractmethod
from pathlib import Path

from .bk_packager_json import parse_packager_json
from .bk_packager_linker import bk_packager_linker

logger = logging.getLogger(__package__)


class bk_packager(ABC):
    """beken packager"""

    def __init__(
        self,
        workdir: Path,
        pack_json: Path,
        linker: bk_packager_linker,
        output_file_name: Path = Path("all-app.bin"),
    ):
        logger.info(f"bk_partition workdir: {workdir}")
        logger.info(f"load pack json: {pack_json}")
        self.workdir = workdir.absolute()

        # check workdir
        if not self.workdir.is_dir():
            raise RuntimeError(f"work directory {self.workdir} not exist.")

        self.part_info = parse_packager_json(workdir, pack_json)
        self.output_file = output_file_name

        if not output_file_name.is_absolute():
            self.output_file = self.workdir / output_file_name

        self.linker = linker

    def pack(self):
        self._pre_link()
        self.linker.link(self.part_info, self.output_file)
        self._post_link()

    @abstractmethod
    def _pre_link(self):
        pass

    @abstractmethod
    def _post_link(self):
        pass
