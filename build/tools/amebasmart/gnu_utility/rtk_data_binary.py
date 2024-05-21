#!/usr/bin/env python

import os
import sys

dirpath = os.path.dirname(__file__)

def text_to_binary(input_file, output_file):
    with open(input_file, 'r') as infile, open(output_file, 'wb') as outfile:
        for line in infile:
            # Split the line by whitespace and remove '0x' prefix
            hex_values = line.strip().split()
            # Convert hex values to binary data
            binary_data = bytearray(int(value, 16) for value in hex_values)
            # Write binary data to the output file
            outfile.write(binary_data)

if __name__ == "__main__":
    input_file = dirpath + "/rtk_data_binary.txt"
    output_file = dirpath + "/rtk_ext_flash_data.bin"

    text_to_binary(input_file, output_file)
