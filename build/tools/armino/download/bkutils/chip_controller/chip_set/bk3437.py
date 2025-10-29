import time
from .bk7231N import BK7231N
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...common.common import safe_data

class BK3437(BK7231N):

    CHIP_ID = 0x3437
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
