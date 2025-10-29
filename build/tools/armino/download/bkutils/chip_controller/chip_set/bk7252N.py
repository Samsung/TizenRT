import time
from .bk7231N import BK7231N
from ...protocol.bootrom import v2_0 as V2_0_BOOTROM_PROTOCOL
from ...bk_log import BKLog
from ...common.common import safe_data
from ...chip_controller import BaseController
from ...common.common import LINKCHECK

class BK7252N(BK7231N):

    CHIP_ID = 0x7252A
    BOOTROM_PROTOCOL = V2_0_BOOTROM_PROTOCOL

    def __init__(self):
        super().__init__()
