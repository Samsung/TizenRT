#!/usr/bin/env python3
import os
import struct
import logging
from .common import *

class crc:
    def __init__(self, infile, outfile):
        b_success = False
        in_file_size = os.path.getsize(infile)
        num_of_crc = (int)(in_file_size / CRC_PAKEIT)
        last_block_len = in_file_size % 32
        logging.debug(f'process crc, num_of_crc=%x last_block_len=%x' %(num_of_crc, last_block_len))
        with open(outfile, 'wb+') as of:
            with open(infile, 'rb') as f:
                for i in range(num_of_crc):
                    raw = f.read(32)
                    crc = crc16(raw, 0, CRC_PAKEIT)
                    of.write(raw)
                    crc = struct.pack(">H", crc)
                    of.write(crc)
                if last_block_len == 0:
                    b_success = True
                if (last_block_len != 0):
                    pad_buf = []
                    raw = f.read(last_block_len)
                    pad_len = 32 - last_block_len
                    for c in raw:
                        pad_buf.append(c)
                    for i in range(pad_len):
                        pad_buf.append(0xff)
                    pad_buf_bytes = bytes(pad_buf)
                    crc = crc16(pad_buf_bytes, 0, CRC_PAKEIT)
                    of.write(pad_buf_bytes)
                    crc = struct.pack(">H", crc)
                    of.write(crc)
                    b_success = True
        if (b_success == True):
            logging.info(f'crc success {outfile}')
        else:
            logging.info(f'crc fail {outfile}')
