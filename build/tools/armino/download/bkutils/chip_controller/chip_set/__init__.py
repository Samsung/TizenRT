#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

from ...bk_log import BKLog
from .base_chip import BaseChip
from .bk7236 import BK7236
from .bk7231N import BK7231N
from .bk7259 import BK7259
from .bk7252N import BK7252N
from .bk7238 import BK7238
from .bk3633 import BK3633
from .bk3436 import BK3436
from .bk3437 import BK3437
from .bk7256 import BK7256
from .bk7236Q import BK7236Q
from .bk7239N import BK7239N

# 获取chip id的寄存器列表
#chip_id_reg_list = [0x44010004, 0x800000, 0x34010004]

def get_chip_set_with_chip_id(chip_id,sec_id=0x10001) -> BaseChip:
    ChipSetClass = None
    if chip_id == BK7236.CHIP_ID or chip_id >> 16 == BK7236.CHIP_ID:       
        if sec_id in BK7239N.SECOND_ID:
            ChipSetClass = BK7239N()
            BKLog.i("BK7239N")
        elif sec_id==BK7236Q.SECOND_ID:
            ChipSetClass = BK7236Q()
            BKLog.i("BK7236Q")
        else:
            ChipSetClass = BK7236()
    elif chip_id==BK7259.CHIP_ID or chip_id >> 16 == BK7259.CHIP_ID:
        ChipSetClass = BK7259()
    elif chip_id==BK7231N.CHIP_ID:
        ChipSetClass = BK7231N()
    elif chip_id==BK7252N.CHIP_ID:
        ChipSetClass = BK7252N()
    elif chip_id==BK7238.CHIP_ID:
        ChipSetClass = BK7238()
    elif chip_id==BK3633.CHIP_ID:
        ChipSetClass = BK3633()
    elif chip_id==BK3436.CHIP_ID:
        ChipSetClass = BK3436()
    elif chip_id==BK3437.CHIP_ID:
        ChipSetClass = BK3437()
    elif chip_id==BK7256.CHIP_ID or chip_id >> 16 == BK7256.CHIP_ID:
        ChipSetClass = BK7256()
    else:
        raise Exception('Current Chip is not support')
    return ChipSetClass
