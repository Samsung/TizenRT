from __future__ import annotations

from abc import ABC, abstractmethod
from pathlib import Path

from .bk_packager_json import PartitionInfo


class bk_packager_linker(ABC):
    @abstractmethod
    def link(self, part_info: list[PartitionInfo], output_file: Path):
        pass
