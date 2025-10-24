#!/usr/bin/env python3
import os
import logging

class dump:
    def __init__(self, infile, outfile):
        with open(outfile, "wt+") as of:
            with open(infile, "rb") as f:
                file_size = os.path.getsize(infile)
                for i in range(file_size):
                    byte = f.read(1)
                    hex_byte = byte.hex()
                    of.write(hex_byte)
