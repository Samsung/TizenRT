#!/usr/bin/env python3

import os
# import ctypes
import logging
# from ctypes import *
import platform
from .common import *


class encrypt:
    def __init__(self, infile, keywords, start_address, un_skip, outfile, debug):
        self.key1, self.key2, self.key3, self.key4 = keywords
        self.infile = infile
        self.start_address = start_address
        self.un_skip = un_skip
        self.outfile = outfile
        self.debug = debug

    def func_enc(self):
        raise RuntimeError("current python not support ctypes")
        # try:
        #     if platform.system().lower() == 'windows':
        #         solib = CDLL('./scripts/common.dll')
        #     else:
        #         solib = ctypes.CDLL('./scripts/libcommon.so')
        # except OSError as e:
        #     logging.error(f"load common.dll/so fail")
        #     return

        # # Step 1: 将bin文件结尾补充FF到4Byte的整数倍
        # solib.encrypt(self.infile.encode(),
        #               self.start_address.encode(),
        #               self.key1.encode(),
        #               self.key2.encode(),
        #               self.key3.encode(),
        #               self.key4.encode(),
        #               (int)(self.un_skip),
        #               self.outfile.encode(),
        #               (int)(self.debug)
        #               )
