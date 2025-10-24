import logging

from .bk_misc import check_overlaps, format_size, parse_format_size

__version__ = "0.0.1"

__all__ = [
    "format_size",
    "check_overlaps",
    "parse_format_size",
]


def set_debug_log():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.DEBUG)


def set_info_log():
    logger = logging.getLogger(__name__)
    logger.setLevel(logging.INFO)
