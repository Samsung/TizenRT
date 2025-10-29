#!/usr/bin/env python3

import struct
import logging

class create_fill_img:
    def __init__(self, outfile, size, fill):
        fill_value = 0
        if fill:
            fill_value = int(fill)

        for i in range(int(size)):
            a = struct.pack('B', fill_value)
            outfile.write(a)
            outfile.flush()
