#!/usr/bin/env python3
import os
import logging

class revert:
    def __init__(self, infile, outfile):
        bin_size = os.path.getsize(infile)
        if (bin_size % 4) != 0:
            logging.error(f'{infile} is not 4 bytes aligned')
            exit(0)
        loop_cnt = bin_size >> 2
        addr = 0x0
        with open(outfile, 'wt+') as of:
            with open(infile, 'rb') as f:
                for i in range(loop_cnt):
                    bin_buf = f.read(4)
                    hex_buf = bin_buf.hex()
                    hex_int = int(hex_buf, base=16)

                    byte0 = (hex_int >> 24) & 0xFF
                    byte1 = (hex_int >> 16) & 0xFF
                    byte2 = (hex_int >> 8) & 0xFF
                    byte3 = (hex_int >> 0) & 0xFF
                    hex_int_le = (byte3 << 24) + (byte2 << 16) + \
                        (byte1 << 8) + byte0

                    hex_int_revert = 0xFFFFFFFF - hex_int_le
                    hex_str = str(hex_int_revert)
                    revert_buf = ''.join(
                        ['@{:0>8x} '.format(addr), '%08x' % (hex_int_revert), '\n'])
                    of.write(revert_buf)
                    addr += 1
                    revert_buf = ''.join(
                        ['@{:0>8x} '.format(addr), '%08x' % (hex_int_revert), '\n'])
                    of.write(revert_buf)
                    addr += 1
