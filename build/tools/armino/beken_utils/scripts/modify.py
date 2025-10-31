#!/usr/bin/env python3
import os


class modify:
    def __init__(self, infile, outfile, address):
        print(f'in={infile}, out={outfile}, address={address}')
        file_size = os.path.getsize(infile)
        first_part = 0x10
        second_part = file_size
        with open(outfile, 'wb+') as outfile:
            with open(infile, 'rb') as infile:
                first_part = infile.read(0x10)
                infile.seek(0x11)
                if file_size < 0x11:
                    print(f'file_size < 0x11')
                    exit(0)
                second_part = infile.read(file_size - 0x11)
                outfile.write(first_part)
                outfile.write(b'\x21')
                outfile.write(second_part)
                # infile.seek(0x10) #TODO fix me
                # infile.write(b'\x21')
