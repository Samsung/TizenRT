import logging

from .bk_packager import bk_packager
from .bk_packager_format import bk_packager_format
from .bk_packager_linear import bk_packager_linear

__version__ = "0.0.3"

__all__ = [
    "bk_packager",
    "bk_packager_format",
    "bk_packager_linear",
]


def set_debug_log():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.DEBUG)


def set_info_log():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)
