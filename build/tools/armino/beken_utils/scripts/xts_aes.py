#!/usr/bin/env python3

import os
# import ctypes
import logging
# from ctypes import *
import platform
from .common import *


class generator:
    def __init__(self, version, aes_cbc, outfile):
        self.version = version
        self.outfile = outfile
        self.aes_cbc = aes_cbc
        self.all_quick = False
        self.infile = ""
        self.start_address = ""
        self.keywords = ""
        self.keyfile = ""
        self.genkey = 1
        self.aes = 0

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

        # solib.xts_aes(self.genkey,self.aes,
        #             self.version.encode(),
        #               (int)(self.aes_cbc),
        #               (int)(self.all_quick),
        #               self.infile.encode(),
        #               self.start_address.encode(),
        #               self.keywords.encode(),
        #               self.keyfile.encode(),
        #               self.outfile.encode()
        #               )


class xts_aes:
    def __init__(self, infile, all_quick, start_address, keywords, keyfile, outfile):
        self.version = "0"
        self.aes_cbc = False
        if all_quick:
            self.version = "1"
            self.aes_cbc = True
            self.genkey = 1
        else:
            self.genkey = 0
        self.aes = 1
        self.outfile = outfile
        self.all_quick = all_quick
        self.infile = infile
        self.start_address = start_address
        if keywords:
            self.keywords = keywords
        else:
            self.keywords = ""
        
        if keyfile:
            self.keyfile = keyfile
        else:
            self.keyfile = ""

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

        # solib.xts_aes(self.genkey,self.aes,
        #             self.version.encode(),
        #               (int)(self.aes_cbc),
        #               (int)(self.all_quick),
        #               self.infile.encode(),
        #               self.start_address.encode(),
        #               self.keywords.encode(),
        #               self.keyfile.encode(),
        #               self.outfile.encode()
        #               )
